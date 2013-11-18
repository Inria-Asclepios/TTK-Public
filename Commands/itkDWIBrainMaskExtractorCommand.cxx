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
#include "itkDWIBrainMaskExtractorCommand.h"

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkMaskImageFilter.h>
#include <itkBrainExtractionImageFilter.h>

#include <iostream>
#include "GetPot.h"

//#define DEBUG_ME

namespace itk
{

    struct argument
    {
        const char *input;
        const char *output;
        const char *mask;
    };

    template<class TImage>
    int DWIBrainMaskExtractorCommandImplementation(const argument &arg)
    {
        typedef TImage ImageType;
        typedef Image<unsigned char, 3>  MaskImageType;

        typedef itk::ImageFileReader<ImageType>     ImageReaderType;
        typedef itk::ImageFileWriter<ImageType>     ImageWriterType;
        typedef itk::ImageFileWriter<MaskImageType> MaskImageWriterType;


        typename ImageType::Pointer image = 0;
        {
            typename ImageReaderType::Pointer reader = ImageReaderType::New();
            reader->SetFileName( arg.input );
    
      std::cout << "Reading: " << arg.input << std::flush;
      try
      {
	reader->Update();
      }
      catch( itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << " Done." << std::endl;
    
      image = reader->GetOutput();
      image->DisconnectPipeline();
    }
    

    typedef itk::BrainExtractionImageFilter<ImageType, MaskImageType> BrainExtractionFilterType;
    typename MaskImageType::Pointer maskImage = 0;
    {
      typename BrainExtractionFilterType::Pointer extractor = BrainExtractionFilterType::New();
      extractor->SetInput ( image );
      try
      {
	extractor->Update();
      }
      catch( itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }

      maskImage = extractor->GetOutput();
      maskImage->DisconnectPipeline();
    }

    
    typedef itk::MaskImageFilter<ImageType, MaskImageType, ImageType> MaskFilterType;
    {
      typename MaskFilterType::Pointer masker = MaskFilterType::New();
      masker->SetInput1 ( image );
      masker->SetInput2 ( maskImage );
      masker->SetOutsideValue ( 0.0 );
      
      try
      {
	masker->Update();
      }
      catch( itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      
      image = masker->GetOutput();
      image->DisconnectPipeline();
    }

    
    // write the mask
    std::cout << "Writing: " << arg.mask << std::flush;
     typename MaskImageWriterType::Pointer maskWriter = MaskImageWriterType::New();
      maskWriter->SetInput ( maskImage );
      maskWriter->SetFileName (arg.mask);
      try
      {
	maskWriter->Update();
      }
      catch( itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }

    

    if (strcmp(arg.output, "")!=0)
    {
     // write the image
    typename ImageWriterType::Pointer writer = ImageWriterType::New();
    writer->SetFileName( arg.output );
    writer->SetInput   ( image );
    
    std::cout << "Writing: " << arg.output << std::flush;

    try
    {
      writer->Update();
    }
    catch( itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    }
    
    std::cout << " Done." << std::endl;
    
    return 0;
    
    }


  DWIBrainMaskExtractorCommand::DWIBrainMaskExtractorCommand()
  {
    m_ShortDescription = "Extract a brain mask from a DWI";
    m_LongDescription  = "Usage:\n";
    m_LongDescription += "<-i image file> <-m output mask> <-o output masked image (optional)>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  DWIBrainMaskExtractorCommand::~DWIBrainMaskExtractorCommand()
  {}


  int DWIBrainMaskExtractorCommand::Execute( int argc, const char* argv[])
  {
    itk::Object::GlobalWarningDisplayOff();
    
    GetPot cl(argc, const_cast<char**>(argv)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
    
    argument arg;

    arg.input  = cl.follow("",2,"-I","-i");
    arg.mask = cl.follow("",2,"-M","-m");

    if (cl.search ("-o"))
    {
      arg.output= cl.follow ("", "-o");
    }
    else
        arg.output = "";

    itk::ImageIOBase::Pointer io = itk::ImageIOFactory::CreateImageIO(arg.input, itk::ImageIOFactory::ReadMode);
    if (io.IsNull())
    {
        return EXIT_FAILURE;
    }
    io->SetFileName(arg.input);
    try
    {
    io->ReadImageInformation();
    }
    catch(itk::ExceptionObject &e)
    {
        std::cerr << e;
        return EXIT_FAILURE;
    }

    if (io->GetNumberOfDimensions()>3)
    {
        std::cerr << "only images of dimension 3 are suported while " << io->GetNumberOfDimensions() << " was requested";
        return EXIT_FAILURE;
    }

    switch( io->GetComponentType())
    {
    case itk::ImageIOBase::UCHAR:
        return DWIBrainMaskExtractorCommandImplementation< itk::Image<unsigned char, 3> >(arg);

    case itk::ImageIOBase::CHAR:
        return DWIBrainMaskExtractorCommandImplementation< itk::Image<char, 3> >(arg);

        case itk::ImageIOBase::USHORT:
        return DWIBrainMaskExtractorCommandImplementation< itk::Image<unsigned short, 3> >(arg);

        case itk::ImageIOBase::SHORT:
        return DWIBrainMaskExtractorCommandImplementation< itk::Image<short, 3> >(arg);

        case itk::ImageIOBase::UINT:
        return DWIBrainMaskExtractorCommandImplementation< itk::Image<unsigned int, 3> >(arg);

        case itk::ImageIOBase::INT:
        return DWIBrainMaskExtractorCommandImplementation< itk::Image<int, 3> >(arg);

        case itk::ImageIOBase::ULONG:
        return DWIBrainMaskExtractorCommandImplementation< itk::Image<unsigned long, 3> >(arg);

        case itk::ImageIOBase::LONG:
        return DWIBrainMaskExtractorCommandImplementation< itk::Image<long, 3> >(arg);

        case itk::ImageIOBase::FLOAT:
        return DWIBrainMaskExtractorCommandImplementation< itk::Image<float, 3> >(arg);

        case itk::ImageIOBase::DOUBLE:
        return DWIBrainMaskExtractorCommandImplementation< itk::Image<double, 3> >(arg);

    default:
        std::cerr << "unsupported component type: " << io->GetComponentTypeAsString( io->GetComponentType() );
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
    
  }
  
}
