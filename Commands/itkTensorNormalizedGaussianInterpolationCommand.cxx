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
#include "itkTensorNormalizedGaussianInterpolationCommand.h"

#include "itkNormalizedGaussianInterpolationTensorImageFilter.h"
#include "itkTensorImageIO.h"
#include <itkImageFileReader.h>
#include <itkPoint.h>

#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>

#include "GetPot.h"


namespace itk
{

  TensorNormalizedGaussianInterpolationCommand::TensorNormalizedGaussianInterpolationCommand()
  {
    m_ShortDescription = "Extrapolate sparse tensors on a regular grid using a normalized Gaussian";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "-i    [Input Scalar Image = domain of diffusion]\n";
    m_LongDescription += "-tens [vtkUnstructuredGrid File]\n";
    m_LongDescription += "-tan  [Tangent File]\n";
    m_LongDescription += "-map  [Auxiliary Distance Map File]\n";
    m_LongDescription += "-s    [Sigma]\n";
    m_LongDescription += "-o    [Output Tensor Image]\n\n";
    m_LongDescription += m_ShortDescription;
  }


  TensorNormalizedGaussianInterpolationCommand::~TensorNormalizedGaussianInterpolationCommand()
  {}


  int TensorNormalizedGaussianInterpolationCommand::Execute(int narg, const char* arg[])
  {
  
    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
  
    const bool IsInputPresent    = cl.search(2,"-i","-I"); // == domain of diffusion
    const bool AreSourcesPresent = cl.search(1,"-tens");
    const bool IsMapPresent = cl.search(1,"-map");
    const bool IsOutputPresent   = cl.search(1,"-o");
    const bool IsTangentPresent  = cl.search(1,"-tan");
    
    if(!IsInputPresent || !AreSourcesPresent || !IsOutputPresent)
    {
      std::cerr << "Error: Input and (or) sources and (or) output not set." << std::endl;
      return -1;
    }

    const char* filemask    = cl.follow("NoFile",2,"-i","-I");
    const char* tensorsFile = cl.follow("NoFile", 1, "-tens");
    const char* mapFile = cl.follow("NoFile", 1, "-map");
    const char* output      = cl.follow("NoFile", 2, "-o","-O");
    const char* tangentFile = cl.follow("NoFile",1,"-tan");
    
    
    if( strcmp(tensorsFile,"NoFile")==0 || strcmp(output,"NoFile")==0
	|| strcmp(filemask,"NoFile")==0)
    {
      std::cerr << "Error: Input file and (or) mask and (or) output not set." << std::endl;
      return -1;
    }
    
    const double ox = cl.follow(0.0,1,"-or");
    const double oy = cl.next(0.0);
    const double oz = cl.next(0.0);
    const double sigma = cl.follow(20.0,1,"-s");
    
    std::cout << "Processing NG interpolation of file: " << tensorsFile << std::endl;
    std::cout << "Sigma: " << sigma << std::endl;
    if( IsTangentPresent )
      std::cout << "Tangent: " << tangentFile << std::endl;
    std::cout << "Origin is: " << ox << " " << oy << " " << oz << std::endl;
    std::cout << "Mask: " << filemask << std::endl;
    std::cout << "Output: " << output << std::endl;
    std::cout << std::flush;
    
    
    // typedefs
    typedef double                                          ScalarType;
    typedef itk::TensorImageIO<ScalarType, 3, 3>            IOType;
    typedef IOType::TensorImageType                         TensorImageType;
    typedef itk::Image<double,3>                             ImageType;
    typedef TensorImageType::PixelType                      TensorType;
    typedef itk::NormalizedGaussianInterpolationTensorImageFilter<ImageType, TensorImageType>
      FilterType;
    typedef FilterType::VectorOfPixelsType                  VectorOfTensorsType;
    typedef FilterType::PointType                           PointType;
    typedef FilterType::VectorOfPointsType                  VectorOfPointsType;
    typedef itk::ImageFileReader<ImageType>                 ImageFileReaderType;
    
    
    
    // read the input image
    ImageFileReaderType::Pointer imReader = ImageFileReaderType::New();
    imReader->SetFileName(filemask);
    std::cout << "Reading: " << filemask << std::flush;
    try
    {
      imReader->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e << std::endl;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
    ImageType::Pointer input = imReader->GetOutput();
    
    // read the input image
    ImageFileReaderType::Pointer imReader2 = ImageFileReaderType::New();
    if (IsMapPresent)
    {
      
      imReader2->SetFileName(mapFile);
      std::cout << "Reading: " << mapFile << std::flush;
      try
      {
	imReader2->Update();
      }
      catch(itk::ExceptionObject &e)
      {
	std::cerr << e << std::endl;
	return -1;
      }
      std::cout << " Done." << std::endl;
    }
    ImageType::Pointer secondmap = imReader2->GetOutput();
    
    // read the source tensors  
    std::cout << "Reading: " << tensorsFile;
    vtkUnstructuredGridReader* tensReader = vtkUnstructuredGridReader::New();
    tensReader->SetFileName(tensorsFile);
    vtkUnstructuredGrid* tensors = tensReader->GetOutput();
    tensReader->Update();
    std::cout << " Done." << std::endl;
    
    std::cout << "Converting...";
    // convert the model to a vector of tensors + points
    VectorOfTensorsType vecT;
    VectorOfPointsType  vecP;
    int numPoints = tensors->GetNumberOfPoints();
    
    for(int i=0; i<numPoints; i++)
    {      
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
      
      if (T.GetTrace() > 0.1)
      {
	vecT.push_back(T);
	vecP.push_back(p);
      }
      
    }
    tensReader->Delete();
    std::cout << "Done." << std::endl;
    
    
    VectorOfPointsType vecTangent;
    if(IsTangentPresent)
    {
      std::cout << "Reading tangent file...";
      vtkUnstructuredGridReader* tanReader = vtkUnstructuredGridReader::New();
      tanReader->SetFileName(tangentFile);
      vtkUnstructuredGrid* tangent = tanReader->GetOutput();
      tanReader->Update();
      
      int nTan = tangent->GetNumberOfPoints();
      
      for(int i=0;i<nTan;i++)
      {
	double pt[3];
	tangent->GetPoint(i,pt);
	PointType p;
	for(int m=0;m<3;m++)
	  p[m]=pt[m];
	vecTangent.push_back(p);
      }
      
      tangent->Delete();
    }
    
    
    FilterType::Pointer myFilter = FilterType::New();  
    myFilter->SetTensors(vecT);
    myFilter->SetTangents(vecTangent);
    if (IsMapPresent)
    {
      myFilter->UseAuxiliaryMapOn();
      myFilter->SetAuxiliaryMap (secondmap.GetPointer());
    }
    
    myFilter->SetPoints(vecP);
    myFilter->SetSigma(sigma);
    myFilter->SetInput(input);
    
    std::cout << "Filtering..." << std::flush;
    try
    {
      myFilter->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e << std::endl;
      return -1;
    }
    std::cout << "Done." << std::endl;
    
    
    
    IOType::Pointer writer = IOType::New();
    writer->SetFileName(output);
    writer->SetInput(myFilter->GetOutput());
    
    std::cout << "Wrting..." << std::flush;
    try
    {
      writer->Write();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;
    
    return 0;
  }
  
}
