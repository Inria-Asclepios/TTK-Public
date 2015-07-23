/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkSparseTensorsExtrapolationCommand.h"

#include "itkSparseTensorsDiffusionTensorImageFilter.h"
#include "itkLogTensorImageFilter.h"
#include "itkExpTensorImageFilter.h"
#include "itkTensorImageIO.h"
#include "itkPoint.h"

#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>

#include "GetPot.h"


namespace itk
{

  SparseTensorsExtrapolationCommand::SparseTensorsExtrapolationCommand()
  {
    m_ShortDescription = "Extrapolate a set of tensors on a regular grid using a diffusion equation";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "-i    [Input Tensor Image]\n";
    m_LongDescription += "-tens [vtkUnstructuredGrid File]\n";
    m_LongDescription += "-tan  [Release tangent information 0/1]\n";
    m_LongDescription += "-s    [Sigma Diffusion]\n";
    m_LongDescription += "-l    [Lambda = amount of data attach]\n";
    m_LongDescription += "-dt   [Delta T]\n";
    m_LongDescription += "-nite [Number of Iterations]\n";
    m_LongDescription += "-rms  [RMS]\n";
    m_LongDescription += "-t    [Threads]\n";
    m_LongDescription += "-o    [Structured Tensor Output]\n\n";
    m_LongDescription += m_ShortDescription;
  }

  SparseTensorsExtrapolationCommand::~SparseTensorsExtrapolationCommand()
  {}

  int SparseTensorsExtrapolationCommand::Execute (int argc, const char *argv[])
  {
    
    GetPot   cl(argc, const_cast<char**>(argv)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }


    const bool IsInputPresent    = cl.search(2,"-i","-I");
    const bool IsOutputPresent   = cl.search(2,"-o","-O");
    const bool AreSourcesPresent = cl.search(1,"-tens");
    
    if(!IsInputPresent || !IsOutputPresent || !AreSourcesPresent)
    {
      std::cerr << "Error: Input and (or) output and (or) sources not set." << std::endl;
      return -1;
    }

    const char* mask = cl.follow("NoFile", 2, "-i","-I");
    const char* output = cl.follow("NoFile", 2, "-o","-O");
    const char* tensorsFile = cl.follow("NoFile",1,"-tens");
    const bool tan = cl.follow(0,1,"-tan");
    
    if(strcmp(tensorsFile,"NoFile")==0 || strcmp(output,"NoFile")==0 
       || strcmp(mask,"NoFile")==0)
    {
      std::cerr << "Error: Input and (or) output and (or) sources not set." << std::endl;
      return -1;
    }


    const double deltat =  cl.follow(1.0, 1, "-dt");
    const int    nite = cl.follow(100,1,"-nite");
    const double lambda = cl.follow(1.0,2,"-l","-L");
    const double sigma = cl.follow(2.0,2,"-s","-S");
    const double rms = cl.follow(1.0,1,"-rms");
    const int threads = cl.follow (1, 2, "-t","-T");  
    
    
    std::cout << "Processing diffusion of file: " << tensorsFile << ", with: " << std::endl;
    std::cout << "Sigma = " << sigma << std::endl;
    std::cout << "Lambda =  " << lambda << std::endl;
    std::cout << "Delta t = " << deltat << std::endl;  
    std::cout << "Nite = " << nite << std::endl;
    std::cout << "RMS = " << rms << std::endl;  
    std::cout << "Output: " << output << std::endl;
    std::cout << std::flush;
    
    
    // typedefs
    typedef double                                ScalarType;  
    typedef itk::TensorImageIO<ScalarType, 3 ,3>  IOType;
    typedef IOType::TensorImageType               TensorImageType;
    typedef TensorImageType::PixelType            TensorType;
    typedef itk::SparseTensorsDiffusionTensorImageFilter<TensorImageType, TensorImageType>
      FilterType;  
    typedef FilterType::VectorOfTensorsType VectorOfTensorsType;
    typedef FilterType::PointType           PointType;  
    typedef FilterType::VectorOfPointsType  VectorOfPointsType;
    
    
    // read the tensor field
    IOType::Pointer tensReader = IOType::New();  
    tensReader->SetFileName(mask);
    std::cout << "Reading..." << std::flush;
    try
    {
      tensReader->Read();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;
    
    TensorImageType::Pointer input = tensReader->GetOutput();
    
    // read the model  
    std::cout << "Reading: " << tensorsFile;
    vtkUnstructuredGridReader* reader = vtkUnstructuredGridReader::New();
    reader->SetFileName(tensorsFile);
    vtkUnstructuredGrid* tensors = reader->GetOutput();
    reader->Update();
    std::cout << " Done." << std::endl;
    
  
    std::cout << "Converting...";
    // convert the model to a vector of tensors + points
    VectorOfTensorsType vecT;
    VectorOfPointsType  vecP;
    VectorOfPointsType  vecTangent;
    int numPoints = tensors->GetNumberOfPoints();
    
    for( int i=0; i<numPoints; i++)
    {
      // get the position
      double pt[3];
      tensors->GetPoint(i,pt);
      PointType p;
      for(int m=0;m<3;m++)
	p[m]=pt[m];
      
      
      double tensorCoefs[9];
      tensors->GetPointData()->GetTensors()->GetTuple(i,tensorCoefs);
      
      TensorType T;
      T.SetNthComponent ( 0, tensorCoefs[0] );
      T.SetNthComponent ( 1, tensorCoefs[3] );
      T.SetNthComponent ( 2, tensorCoefs[4] );
      T.SetNthComponent ( 3, tensorCoefs[6] );
      T.SetNthComponent ( 4, tensorCoefs[7] );
      T.SetNthComponent ( 5, tensorCoefs[8] );
      
      if (T.GetTrace() > 0.01)
      {
	vecT.push_back(T);
	vecP.push_back(p);
      }

      if (tan)
      {
	// get the tangent which is stored as the normal
	double tangent[3];
	tensors->GetPointData()->GetNormals()->GetTuple (i, tangent);
	PointType TAN;
	for(int m=0;m<3;m++)
	  TAN[m]=tangent[m];
	vecTangent.push_back (TAN);
      }      
      
    }
    reader->Delete();  
    std::cout << "Done." << std::endl;
    
    
    // set up the pipeline
    typedef itk::LogTensorImageFilter<TensorImageType,TensorImageType>
      LogFilterType;
    LogFilterType::Pointer logFilter = LogFilterType::New();
    logFilter->SetInput(input);
    logFilter->SetNumberOfThreads (threads);
    
    // set up the filter
    FilterType::Pointer myFilter = FilterType::New();  
    myFilter->SetTensors(vecT);
    myFilter->SetPoints(vecP);
    myFilter->SetTangents (vecTangent);
    myFilter->SetDoNotDiffuseTangents (tan);
    myFilter->SetSigma(sigma);
    myFilter->SetLambda(lambda);
    myFilter->SetTimeStep(deltat);
    myFilter->SetNumberOfIterations(nite);
    myFilter->SetRMSChange(rms);
    myFilter->SetNumberOfThreads (threads);  
    myFilter->SetInput(logFilter->GetOutput());
    
    
    
    typedef itk::ExpTensorImageFilter<TensorImageType,TensorImageType>
      ExpFilterType;
    ExpFilterType::Pointer expFilter = ExpFilterType::New();
    expFilter->SetInput(myFilter->GetOutput());
    expFilter->SetNumberOfThreads (threads);
    
    std::cout << "Pipeline started."<< std::endl;
    try
    {
      expFilter->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    std::cout << "Pipeline finished." << std::endl;
    
    
    tensReader->SetFileName(output);
    tensReader->SetInput(expFilter->GetOutput());
    try
    {
      tensReader->Write();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }   
    
    return 0;
    
  }
  
}
