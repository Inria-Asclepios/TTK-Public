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


#ifdef ITK_USE_REVIEW
#include <itkPhilipsRECImageIOFactory.h>
#endif

// macro for templated ITK read and write
#define ReadWriteImageMacro(dimension, type)				\
  itk::ImageFileReader< itk::Image<type, dimension> >::Pointer Reader = itk::ImageFileReader< itk::Image<type, dimension> >::New(); \
  Reader->SetFileName ( filename1 );					\
  try									\
  {									\
    Reader->Update();							\
  }									\
  catch (itk::ExceptionObject &e) {					\
    std::cerr << e;							\
    return -1;								\
  }									\
  itk::ImageFileWriter< itk::Image<type, dimension> >::Pointer Writer = itk::ImageFileWriter< itk::Image<type, dimension> >::New(); \
  Writer->SetFileName ( filename2 );					\
  Writer->SetInput(Reader->GetOutput());				\
  try									\
  {									\
    Writer->Update();							\
  }									\
  catch (itk::ExceptionObject &e) {					\
    std::cerr << e;							\
    return -1;								\
  }									\

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
  
    const char* filename1 = arg[1];
    const char* filename2 = arg[2];
    
    itk::AnalyzeImageIOFactory::RegisterOneFactory();
#ifdef TTK_USE_MIPS
    itk::InrimageImageIOFactory::RegisterOneFactory();
#endif
#ifdef ITK_USE_REVIEW
  itk::PhilipsRECImageIOFactory::RegisterOneFactory();
#endif
  
    // dummy reading information to get type and dimension
    typedef itk::Image<float,3> ImageType;
    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer informationreader = ReaderType::New();
    informationreader->SetFileName(filename1);
    try
    {
      informationreader->GenerateOutputInformation();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    std::cerr << "Dimension: " << informationreader->GetImageIO()->GetNumberOfDimensions() << std::endl;
    std::cerr << "Component Type: " << informationreader->GetImageIO()->GetComponentTypeAsString(informationreader->GetImageIO()->GetComponentType()) << std::endl;
    std::cerr << "Pixel Type: " << informationreader->GetImageIO()->GetPixelTypeAsString(informationreader->GetImageIO()->GetPixelType()) << std::endl;

    if(informationreader->GetImageIO()->GetPixelType() != itk::ImageIOBase::SCALAR)
    {
      std::cerr << "Only scalar images can be converted" << std::endl;
      return -1;
    }
    
    // real reading with dimension and type
    unsigned int dim = informationreader->GetImageIO()->GetNumberOfDimensions();
    itk::ImageIOBase::IOComponentType componenttype = informationreader->GetImageIO()->GetComponentType();

    if (dim > 6)
    { std::cerr << "Dimension Too High." << dim<< std::endl; return -1; }

    switch (componenttype)
    {
	case itk::ImageIOBase::UCHAR:
	  if      (dim == 2) { ReadWriteImageMacro (2, unsigned char); }
	  else if (dim == 3) { ReadWriteImageMacro (3, unsigned char); }
	  else if (dim == 4) { ReadWriteImageMacro (4, unsigned char); }
	  else if (dim == 5) { ReadWriteImageMacro (5, unsigned char); }
	  else if (dim == 6) { ReadWriteImageMacro (6, unsigned char); }
	  break;
	case itk::ImageIOBase::CHAR:
	  if      (dim == 2) { ReadWriteImageMacro (2, char); }
	  else if (dim == 3) { ReadWriteImageMacro (3, char); }
	  else if (dim == 4) { ReadWriteImageMacro (4, char); }
	  else if (dim == 5) { ReadWriteImageMacro (5, char); }
	  else if (dim == 6) { ReadWriteImageMacro (6, char); }
	  break;
    	case itk::ImageIOBase::USHORT:
	  if      (dim == 2) { ReadWriteImageMacro (2, unsigned short); }
	  else if (dim == 3) { ReadWriteImageMacro (3, unsigned short); }
	  else if (dim == 4) { ReadWriteImageMacro (4, unsigned short); }
	  else if (dim == 5) { ReadWriteImageMacro (5, unsigned short); }
	  else if (dim == 6) { ReadWriteImageMacro (6, unsigned short); }
	  break;
    	case itk::ImageIOBase::SHORT:
	  if      (dim == 2) { ReadWriteImageMacro (2, short); }
	  else if (dim == 3) { ReadWriteImageMacro (3, short); }
	  else if (dim == 4) { ReadWriteImageMacro (4, short); }
	  else if (dim == 5) { ReadWriteImageMacro (5, short); }
	  else if (dim == 6) { ReadWriteImageMacro (6, short); }
	  break;
    	case itk::ImageIOBase::UINT:
	  if      (dim == 2) { ReadWriteImageMacro (2, unsigned int); }
	  else if (dim == 3) { ReadWriteImageMacro (3, unsigned int); }
	  else if (dim == 4) { ReadWriteImageMacro (4, unsigned int); }
	  else if (dim == 5) { ReadWriteImageMacro (5, unsigned int); }
	  else if (dim == 6) { ReadWriteImageMacro (6, unsigned int); }
	  break;
    	case itk::ImageIOBase::INT:
	  if      (dim == 2) { ReadWriteImageMacro (2, int); }
	  else if (dim == 3) { ReadWriteImageMacro (3, int); }
	  else if (dim == 4) { ReadWriteImageMacro (4, int); }
	  else if (dim == 5) { ReadWriteImageMacro (5, int); }
	  else if (dim == 6) { ReadWriteImageMacro (6, int); }
	  break;
    	case itk::ImageIOBase::ULONG:
	  if      (dim == 2) { ReadWriteImageMacro (2, unsigned long); }
	  else if (dim == 3) { ReadWriteImageMacro (3, unsigned long); }
	  else if (dim == 4) { ReadWriteImageMacro (4, unsigned long); }
	  else if (dim == 5) { ReadWriteImageMacro (5, unsigned long); }
	  else if (dim == 6) { ReadWriteImageMacro (6, unsigned long); }
	  break;
	case itk::ImageIOBase::LONG:
	  if      (dim == 2) { ReadWriteImageMacro (2, long); }
	  else if (dim == 3) { ReadWriteImageMacro (3, long); }
	  else if (dim == 4) { ReadWriteImageMacro (4, long); }
	  else if (dim == 5) { ReadWriteImageMacro (5, long); }
	  else if (dim == 6) { ReadWriteImageMacro (6, long); }
	  break;
	case itk::ImageIOBase::FLOAT:
	  if      (dim == 2) { ReadWriteImageMacro (2, float); }
	  else if (dim == 3) { ReadWriteImageMacro (3, float); }
	  else if (dim == 4) { ReadWriteImageMacro (4, float); }
	  else if (dim == 5) { ReadWriteImageMacro (5, float); }
	  else if (dim == 6) { ReadWriteImageMacro (6, float); }
	  break;
    	case itk::ImageIOBase::DOUBLE:
	  if      (dim == 2) { ReadWriteImageMacro (2, double); }
	  else if (dim == 3) { ReadWriteImageMacro (3, double); }
	  else if (dim == 4) { ReadWriteImageMacro (4, double); }
	  else if (dim == 5) { ReadWriteImageMacro (5, double); }
	  else if (dim == 6) { ReadWriteImageMacro (6, double); }
	  break;
    	default:
    	  std::cerr << "Unrecognized Component Type : " << informationreader->GetImageIO()->GetComponentTypeAsString(componenttype) << std::endl;
    	  return -1;
    	  break;  
    }
    
    return 0;
    
  }
  
}
