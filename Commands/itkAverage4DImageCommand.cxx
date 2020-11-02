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
#include "itkAverage4DImageCommand.h"

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImage.h>
#include <itkGetAverageSliceImageFilter.h>
#include <itkExtractImageFilter.h>

#include <iostream>
#include "GetPot.h"


namespace itk
{

    template<class TImage>
    int Average4DCommandImplementation(const char *input, const char *output, const int &offset)
    {
        using InputImageType  = TImage;
        using ScalarType      = typename InputImageType::PixelType;
        using OutputImageType = Image<ScalarType, 3>;    
        using ImageReaderType = itk::ImageFileReader<InputImageType>;
        using ImageWriterType = itk::ImageFileWriter<OutputImageType>;
    
        typename InputImageType::Pointer image = nullptr;
        {
          typename ImageReaderType::Pointer reader = ImageReaderType::New();
          reader->SetFileName( input );
          try
          {
	    reader->Update();
          }
          catch (itk::ExceptionObject &e)
          {
	    std::cerr << e;
	    return -1;
          }

          image = reader->GetOutput();
          image->DisconnectPipeline();
        }


        if (offset>0)
        {
          using Extract4DFilterType = ExtractImageFilter<InputImageType, InputImageType>;
          typename InputImageType::RegionType region = image->GetLargestPossibleRegion();
          typename InputImageType::SizeType size = region.GetSize();
          size[3] -= offset;
          typename InputImageType::IndexType index = region.GetIndex();
          index[3] = offset;
          region.SetSize  (size);
          region.SetIndex (index);
          typename Extract4DFilterType::Pointer extractor = Extract4DFilterType::New();
          extractor->SetInput (image);
          extractor->SetExtractionRegion (region);
            extractor->SetDirectionCollapseToGuess();
          try
          {
	    extractor->Update();
          }
          catch (itk::ExceptionObject &e)
          {
	    std::cerr << e;
	    return -1;
          }

          image = extractor->GetOutput();
          image->DisconnectPipeline();
        }    


    
    using FilterType = GetAverageSliceImageFilter<InputImageType, InputImageType>;
    {
      typename FilterType::Pointer filter = FilterType::New();
      filter->SetInput (image);
      filter->SetAveragedOutDimension (3);
      try
      {
	filter->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }

      filter->GetOutput()->SetDirection (image->GetDirection());

      image = filter->GetOutput();
      image->DisconnectPipeline();
        image->SetDirection(filter->GetInput(0)->GetDirection());
    }


    typename OutputImageType::Pointer outImage = nullptr;
    using ExtractFilterType = ExtractImageFilter<InputImageType, OutputImageType>;
    {
      typename InputImageType::RegionType region = image->GetLargestPossibleRegion();
      typename InputImageType::SizeType size = region.GetSize();
      size[3] = 0;
      typename InputImageType::IndexType index = region.GetIndex();
      index[3] = 0;
      region.SetSize  (size);
      region.SetIndex (index);
      typename ExtractFilterType::Pointer extractor = ExtractFilterType::New();
      extractor->SetInput (image);
      extractor->SetExtractionRegion (region);
        extractor->SetDirectionCollapseToGuess();
      try
      {
	extractor->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }

      outImage = extractor->GetOutput();
      outImage->DisconnectPipeline();
    }
    
    // write the image
    typename ImageWriterType::Pointer writer = ImageWriterType::New();
    writer->SetFileName( output );
    writer->SetInput( outImage );
    
    std::cout << "Writing: " << output << std::flush;
    try
    {
      writer->Update();
    }
    catch( itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    std::cout << " Done." << std::endl;
    
    return 0;
    }

  Average4DImageCommand::Average4DImageCommand()
  {
    m_ShortDescription = "Average a 4D image";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i input> <-n offset> <-o output>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  Average4DImageCommand::~Average4DImageCommand()
  {}

  int Average4DImageCommand::Execute (int narg, const char* arg[])
  {
    itk::Object::GlobalWarningDisplayOff();
    
    GetPot cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }

    const char* input = cl.follow("","-i");
    const char* output = cl.follow("output.nii.gz","-o");
    int offset = cl.follow (0, "-n");
    
    itk::ImageIOBase::Pointer io = itk::ImageIOFactory::CreateImageIO(input, IOFileModeEnum::ReadMode);
    if (io.IsNull())
    {
        return EXIT_FAILURE;
    }
    io->SetFileName(input);
    try
    {
    io->ReadImageInformation();
    }
    catch(itk::ExceptionObject &e)
    {
        std::cerr << e;
        return EXIT_FAILURE;
    }

    switch( io->GetComponentType())
    {
    case IOComponentEnum::UCHAR:
        return Average4DCommandImplementation< itk::Image<unsigned char, 4> >(input, output, offset);

    case IOComponentEnum::CHAR:
        return Average4DCommandImplementation< itk::Image<char, 4> >(input, output, offset);

        case IOComponentEnum::USHORT:
        return Average4DCommandImplementation< itk::Image<unsigned short, 4> >(input, output, offset);

        case IOComponentEnum::SHORT:
        return Average4DCommandImplementation< itk::Image<short, 4> >(input, output, offset);

        case IOComponentEnum::UINT:
        return Average4DCommandImplementation< itk::Image<unsigned int, 4> >(input, output, offset);

        case IOComponentEnum::INT:
        return Average4DCommandImplementation< itk::Image<int, 4> >(input, output, offset);

        case IOComponentEnum::ULONG:
        return Average4DCommandImplementation< itk::Image<unsigned long, 4> >(input, output, offset);

        case IOComponentEnum::LONG:
        return Average4DCommandImplementation< itk::Image<long, 4> >(input, output, offset);

        case IOComponentEnum::FLOAT:
        return Average4DCommandImplementation< itk::Image<float, 4> >(input, output, offset);

        case IOComponentEnum::DOUBLE:
        return Average4DCommandImplementation< itk::Image<double, 4> >(input, output, offset);

    default:
        std::cerr << "unsupported component type: " << io->GetComponentTypeAsString( io->GetComponentType() );
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
  }
  
}
