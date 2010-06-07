/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL:$
  Language:  C++
  Date:      $Date:$
  Version:   $Revision:$

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkImageConverterCommand.h"

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <itkImageIOBase.h>
#include <itkAnalyzeImageIOFactory.h>

#include "ttkConfigure.h"

#ifdef TTK_USE_MIPS
#include "mipsInrimageImageIOFactory.h"
#endif

// macro for templated ITK read and write
#define ITK_READ_WRITE \
typedef itk::ImageFileReader<RealImageType> RealReaderType; \
RealReaderType::Pointer RealReader = RealReaderType::New(); \
RealReader->SetFileName(filename); \
try { RealReader->Update(); } \
catch(itk::ExceptionObject &e)  { \
std::cerr << e; \
return -1; \
} \
typedef itk::ImageFileWriter<RealImageType> WriterType; \
WriterType::Pointer writer = WriterType::New(); \
writer->SetFileName(arg[2]); \
writer->SetInput(RealReader->GetOutput()); \
try { writer->Update(); } \
catch(itk::ExceptionObject &e) { \
std::cerr << e; \
return -1; \
}


namespace itk
{

  ImageConverterCommand::ImageConverterCommand()
  {
    m_ShortDescription = "Convert an image file into another format";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<input image> <output image>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  ImageConverterCommand::~ImageConverterCommand()
  {}

  int ImageConverterCommand::Execute(int narg, const char* arg[])
  {
  
    if(narg!=3)
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
  
    const char* filename = arg[1];
    
    itk::AnalyzeImageIOFactory::RegisterOneFactory();
#ifdef TTK_USE_MIPS
    itk::InrimageImageIOFactory::RegisterOneFactory();
#endif
  
    // dummy reading to get type and dimension
    typedef itk::Image<float,3> ImageType;  
    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(filename);
    try { reader->Update(); }
    catch(itk::ExceptionObject &e) {
      std::cerr << e;
      return -1;
    }
    
    std::cerr << "Component Type: " << reader->GetImageIO()->GetComponentTypeAsString(reader->GetImageIO()->GetComponentType()) << std::endl;
    std::cerr << "Pixel Type: " << reader->GetImageIO()->GetPixelTypeAsString(reader->GetImageIO()->GetPixelType()) << std::endl;
    if(reader->GetImageIO()->GetPixelType() != itk::ImageIOBase::SCALAR) {
      std::cerr << "Only scalar images can be converted" << std::endl;
      return -1;
    }
    
    // real reading with dimension and type
    unsigned int dim = reader->GetImageIO()->GetNumberOfDimensions();
    
    if (dim==3) {
      if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::UNKNOWNCOMPONENTTYPE) {
	std::cerr << "Unknown type, exiting" << std::endl;
	return -1;
      }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::UCHAR) {
	typedef itk::Image<unsigned char,3> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() ==  itk::ImageIOBase::CHAR) {
	typedef itk::Image<char,3> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::USHORT) {
	typedef itk::Image<unsigned short,3> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::SHORT) {
	typedef itk::Image<short,3> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::UINT) {
	typedef itk::Image<unsigned int,3> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::INT){
	typedef itk::Image<int,3> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::ULONG) {
	typedef itk::Image<unsigned long,3> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::LONG) {
	typedef itk::Image<long,3> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::FLOAT) {
	typedef itk::Image<float,3> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::DOUBLE) {
	typedef itk::Image<double,3> RealImageType;
	ITK_READ_WRITE
	  }
      else {
	typedef itk::Image<float,3> RealImageType;
	ITK_READ_WRITE
	  }
    }
    
    else if (dim==2) {
      if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::UNKNOWNCOMPONENTTYPE) {
	std::cerr << "Unknown type, exiting" << std::endl;
	return -1;
      }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::UCHAR) {
	typedef itk::Image<unsigned char,2> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::CHAR) {
	typedef itk::Image<char,2> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::USHORT) {
	typedef itk::Image<unsigned short,2> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::SHORT) {
	typedef itk::Image<short,2> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::UINT) {
	typedef itk::Image<unsigned int,2> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::INT) {
	typedef itk::Image<int,2> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::ULONG) {
	typedef itk::Image<unsigned long,2> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::LONG) {
	typedef itk::Image<long,2> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::FLOAT) {
	typedef itk::Image<float,2> RealImageType;
	ITK_READ_WRITE
	  }
      else if (reader->GetImageIO()->GetComponentType() == itk::ImageIOBase::DOUBLE) {
	typedef itk::Image<double,2> RealImageType;
	ITK_READ_WRITE
	  }
      else {
	typedef itk::Image<float,2> RealImageType;
	ITK_READ_WRITE
	  }
    }
    
    return 0;
    
  }
  
}
