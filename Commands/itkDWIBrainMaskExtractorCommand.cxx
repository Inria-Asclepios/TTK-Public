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

  typedef double ScalarType;
  typedef unsigned short IntegerType;
  typedef Image<IntegerType, 3>    ImageType;
  typedef Image<unsigned char, 3>  MaskImageType;


  DWIBrainMaskExtractorCommand::DWIBrainMaskExtractorCommand()
  {
    m_ShortDescription = "Extract a brain mask from a DWI";
    m_LongDescription  = "Usage:\n";
    m_LongDescription += "<-i b0 file> <-m output mask (optional)> <-o output masked image>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  DWIBrainMaskExtractorCommand::~DWIBrainMaskExtractorCommand()
  {}


  int DWIBrainMaskExtractorCommand::Execute( int narg, const char* arg[])
  {
    itk::Object::GlobalWarningDisplayOff();
    
    GetPot cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
    
    const char* input  = cl.follow("",2,"-I","-i");
    const char* output = cl.follow("",2,"-O","-o");

    bool writeMask = false;
    const char* mask = 0;
    if (cl.search ("-m"))
    {
      writeMask = true;
      mask = cl.follow ("", "-m");
    }

    typedef itk::ImageFileReader<ImageType>     ImageReaderType;
    typedef itk::ImageFileWriter<ImageType>     ImageWriterType;
    typedef itk::ImageFileWriter<MaskImageType> MaskImageWriterType;


    ImageType::Pointer image = 0;
    {
      ImageReaderType::Pointer reader = ImageReaderType::New();
      reader->SetFileName( input );
    
      std::cout << "Reading: " << input << std::flush;
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
    MaskImageType::Pointer maskImage = 0;
    {
      BrainExtractionFilterType::Pointer extractor = BrainExtractionFilterType::New();
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
      MaskFilterType::Pointer masker = MaskFilterType::New();
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

    
    // write the image
    ImageWriterType::Pointer writer = ImageWriterType::New();
    writer->SetFileName( output );
    writer->SetInput   ( image );
    
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

    if (writeMask)
    {
      MaskImageWriterType::Pointer maskWriter = MaskImageWriterType::New();
      maskWriter->SetInput ( maskImage );
      maskWriter->SetFileName (mask);
      try
      {
	maskWriter->Update();
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
  
}
