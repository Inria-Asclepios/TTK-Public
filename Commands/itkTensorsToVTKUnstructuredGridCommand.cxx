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
#include "itkTensorsToVTKUnstructuredGridCommand.h"

#include "itkTensorImageIO.h"
#include <itksys/SystemTools.hxx>

#include <vtkUnstructuredGrid.h>
#include <vtkDataSetWriter.h>
#include <vtkDataSetReader.h>
#include <vtkPoints.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>

#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include "GetPot.h"

namespace itk
{

  TensorsToVTKUnstructuredGridCommand::TensorsToVTKUnstructuredGridCommand()
  {
    m_ShortDescription = "Convert tensor images into a vtk unstructured grid";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "-i    [input tensor images (default : input.list)]\n";
    m_LongDescription += "-o    [output vtk file (default : output.vtk)]\n\n";
    m_LongDescription += m_ShortDescription;
  }
  
  TensorsToVTKUnstructuredGridCommand::~TensorsToVTKUnstructuredGridCommand()
  {}

  int TensorsToVTKUnstructuredGridCommand::Execute(int narg, const char* arg[])
  {
  
    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }

    const char* inputfile = cl.follow("input.list",2,"-i","-I");
    const char* outputfile = cl.follow("output.vtk",2,"-o","-O");

    std::cout << "Processing bandwidth extraction with following arguments: " << std::endl;
    std::cout << "inputfile: " << inputfile << std::endl;
    std::cout << "outputfile: " << outputfile << std::endl;
    std::cout << std::flush;
  
    // typedefs
    typedef double                                    ScalarType;
    typedef itk::TensorImageIO<ScalarType, 3, 3>      TensorIOType;
    typedef TensorIOType::TensorImageType             TensorImageType;
    typedef TensorImageType::PixelType                TensorType;
    typedef itk::ImageRegionIterator<TensorImageType> TensorIteratorType;
    typedef TensorImageType::PointType                PointType;
    typedef std::vector<TensorType>                   TensorArrayType;
    typedef std::vector<PointType>                    PointArrayType;
    
    // instantiation  
    
    // read the input tensors and put tham into a vtkUnstructuredGrid
    // they come from a text file listing all files to read, either vtk or itk...
    
    std::cout << "Reading input tensors: " << inputfile << std::endl;
    
    TensorArrayType vecT;
    PointArrayType vecP;
    itk::ContinuousIndex<double, 3> index;
    
    std::cout<<"reading list : "<<inputfile<<std::endl;
    
    std::ifstream inputliststream (inputfile);
    if(inputliststream.fail())
    {
      std::cerr << "Unable to open file: " << inputfile << std::endl;
      return -1;
    }
    unsigned int NumberOfImageFiles = 0;
    inputliststream >> NumberOfImageFiles;
    std::cout<<"encountered : "<<NumberOfImageFiles<<std::endl;
    
    std::string sline = "";
    itksys::SystemTools::GetLineFromStream(inputliststream, sline);
    
    for (unsigned int N=0; N<NumberOfImageFiles; N++)
    {
      std::string line = "";
      itksys::SystemTools::GetLineFromStream(inputliststream, line);
      std::cout << "Reading tensors: " << line.c_str() << std::flush;
      TensorIOType::Pointer reader = TensorIOType::New();
      reader->SetFileName(line.c_str());
      bool success = false;
      
      try
      {
	reader->Read();
	success = true;
      }
      catch(itk::ExceptionObject &)
      {
	std::cerr << "cannot read with TensorIO"<< std::endl;
      }
      
      
      if (!success)
      {
	vtkDataSetReader* vtkreader = vtkDataSetReader::New();
	vtkreader->SetFileName (line.c_str());
	vtkreader->Update();
	vtkPointSet* dataset = vtkPointSet::SafeDownCast (vtkreader->GetOutput());
	
	if (!dataset || !dataset->GetNumberOfPoints() || !dataset->GetPointData()->GetTensors())
	{
	  std::cerr << "cannot read file "<<line.c_str()<< std::endl;
	  std::cerr << "skipping line..."<< std::endl;
	  vtkreader->Delete();
	  continue;
	}
	
	for (int i=0; i<dataset->GetNumberOfPoints(); i++)
	{
	  PointType x;
	  for (unsigned int j=0; j<3; j++)
	    x[j] = dataset->GetPoint (i)[j];
	  TensorType T;
	  double* vals = dataset->GetPointData()->GetTensors()->GetTuple (i);
	  
	  T[0] = vals[0];
	  T[1] = vals[1];
	  T[2] = vals[4];
	  T[3] = vals[2];
	  T[4] = vals[5];
	  T[5] = vals[8];
	  
	  vecT.push_back(T);
	  vecP.push_back(x);
	}
	
	vtkreader->Delete();
	
      }
      
      TensorImageType::Pointer tensorimage = reader->GetOutput();
      TensorIteratorType it(tensorimage, tensorimage->GetLargestPossibleRegion());
      
      while( !it.IsAtEnd() )
      {
	PointType x;
	tensorimage->TransformIndexToPhysicalPoint(it.GetIndex(), x);
	
	TensorType T = it.Get();	
	
	if ((T.GetTrace() > 0.0) && !T.HasNans())
	{	
	  T = T.ApplyMatrix(tensorimage->GetDirection());
	  vecT.push_back(T);
	  vecP.push_back(x);
	}
	++it;
      }
      std::cout<<" done."<<std::endl; 
    }
    
    vtkUnstructuredGrid* crossvalidation = vtkUnstructuredGrid::New();
    vtkDoubleArray* data = vtkDoubleArray::New();
    vtkPoints* points = vtkPoints::New();
    points->SetNumberOfPoints (vecP.size());
    data->SetNumberOfComponents (9);
    data->SetNumberOfTuples (vecP.size());
    
    for (unsigned int i=0; i<vecP.size(); i++)
    {
      double pt[3] = {vecP[i][0],
		      vecP[i][1],
		      vecP[i][2]};
      
      points->SetPoint (i, pt);
      double vals[9];
      vals[0] = vecT[i][0];
      vals[1] = vecT[i][1];
      vals[2] = vecT[i][3];
      vals[3] = vecT[i][1];
      vals[4] = vecT[i][2];
      vals[5] = vecT[i][4];
      vals[6] = vecT[i][3];
      vals[7] = vecT[i][4];
      vals[8] = vecT[i][5];
      
      data->SetTuple (i, vals);
    }
    
    crossvalidation->SetPoints (points);
    crossvalidation->GetPointData()->SetTensors (data);
    
    vtkDataSetWriter* writer = vtkDataSetWriter::New();
    writer->SetFileName (outputfile);
    writer->SetInputData (crossvalidation);
    writer->Update();
    
    
    writer->Delete();
    points->Delete();
    data->Delete();
    crossvalidation->Delete();
    
    std::cout<<" done."<<std::endl;
    
  return EXIT_SUCCESS;
  }
  
}
