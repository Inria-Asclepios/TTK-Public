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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkVTKFibersToITKGroupSpatialObjectCommand.h"

#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"

#include <itkImage.h>
#include "itkTensor.h"
#include "itkTensorImageIO.h"
#include "itkLogTensorImageFilter.h"
#include "itkVTKFibersToITKFibers.h"
#include <itkGroupSpatialObject.h>
#include <itkSpatialObjectWriter.h>

#include <vector>
#include <list>
#include <string>
#include "GetPot.h"

namespace itk
{

  VTKFibersToITKGroupSpatialObjectCommand::VTKFibersToITKGroupSpatialObjectCommand()
  {
    m_ShortDescription = "Convert a vtkPolyData fiber representation to an ITK GroupSpatialObject file";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "-i [vtk fiber file]\n";
    m_LongDescription += "-t [tensor file]\n";
    m_LongDescription += "-b [b-value]\n";
    m_LongDescription += "-o [itk fiber file - output]\n\n";
    m_LongDescription += m_ShortDescription;
  }

  VTKFibersToITKGroupSpatialObjectCommand::~VTKFibersToITKGroupSpatialObjectCommand()
  {}

  int VTKFibersToITKGroupSpatialObjectCommand::Execute(int narg, const char* arg[])
  {

    GetPot cl (narg, const_cast<char**>(arg));
    if( cl.size() == 1  || cl.search (2,"--help","-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }

    const char* vtkFiberFile  = cl.follow ("NoFile",2,"-i","-I");
    const char* inrTensorFile = cl.follow ("NoFile",2,"-t","-T");
    const double bvalue       = cl.follow (1.0, 2, "-b","-B");
    const char* itkFiberFile  = cl.follow ("NoFile", 2, "-o","-O");
  

    // Read the input:
    typedef double  ScalarType;
    const unsigned int ImageDimension  = 3;
    const unsigned int TensorDimension = 3;
    
    typedef itk::TensorImageIO<ScalarType, TensorDimension, ImageDimension> IOType;
    typedef IOType::TensorImageType     TensorImageType;
    typedef TensorImageType::PixelType  TensorType;
    
    // read the vtk fiber file
    std::cout << "Reading: " << vtkFiberFile << std::endl;
    vtkPolyDataReader* reader = vtkPolyDataReader::New();
    reader->SetFileName( vtkFiberFile );
    reader->Update();
    
    vtkPolyData* vtkFibers = reader->GetOutput();
    reader->Update();
    

    // read the inrimage tensor file
    IOType::Pointer myReader = IOType::New();
    myReader->SetFileName (inrTensorFile);
  
    std::cout << "Reading: " << inrTensorFile;
    std::cout << std::flush;
    try
    {
      myReader->Read();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
    // log the input
    typedef itk::LogTensorImageFilter<TensorImageType,TensorImageType>
      LogFilterType;
    LogFilterType::Pointer myLog = LogFilterType::New();
    myLog->SetInput (myReader->GetOutput() );
    std::cout << "Logarithming... " << std::flush;
    try
    {
      myLog->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;
    TensorImageType::Pointer myTensors = myLog->GetOutput();


    // convert
    typedef itk::VTKFibersToITKFibers<TensorImageType> ConverterType;
    typedef ConverterType::GroupSpatialObjectType      GroupSpatialObjectType;
    ConverterType::Pointer myConverter = ConverterType::New();
    myConverter->SetInput (vtkFibers);
    myConverter->SetTensorImage (myTensors);
    myConverter->SetBVal (bvalue);
    
    std::cout << "Converting...";
    std::cout << std::flush;
    try
    {
      myConverter->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;
    
    GroupSpatialObjectType::Pointer output = myConverter->GetOutput();
    
    
    std::cout << "Wrting...";
    itk::SpatialObjectWriter<3>::Pointer fibWriter  = itk::SpatialObjectWriter<3>::New();
    fibWriter->SetInput(output);
    fibWriter->SetFileName(itkFiberFile);
    fibWriter->Update();
    std::cout << "Done." << std::endl;
    
    reader->Delete();
    
    return 0;
  }
  
}
