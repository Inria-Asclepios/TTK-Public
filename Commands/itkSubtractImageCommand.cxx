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
#include "itkSubtractImageCommand.h"

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImage.h>
#include <itkSubtractImageFilter.h>

#include <iostream>
#include "GetPot.h"


namespace itk
{

  SubtractImageCommand::SubtractImageCommand()
  {
    m_ShortDescription = "Subtract two images";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i1 input2> <-i2 input2> <-o output>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  SubtractImageCommand::~SubtractImageCommand()
  {}

  int SubtractImageCommand::Execute (int narg, const char* arg[])
  {

    itk::Object::GlobalWarningDisplayOff();
    
    GetPot cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }

    const char* input1 = cl.follow("","-i1");
    const char* input2 = cl.follow("","-i2");
    const char* output = cl.follow("output.nii.gz","-o");
    

    typedef double                        ScalarType;
    typedef Image<ScalarType, 3>          ImageType;
    
    typedef itk::ImageFileReader<ImageType>     ImageReaderType;
    typedef itk::ImageFileWriter<ImageType>     ImageWriterType;
    

    ImageType::Pointer image1 = nullptr;
    {
      ImageReaderType::Pointer reader = ImageReaderType::New();
      reader->SetFileName( input1 );
      try
      {
	reader->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return EXIT_FAILURE;
      }

      image1 = reader->GetOutput();
    }

    
    ImageType::Pointer image2 = nullptr;
    {
      ImageReaderType::Pointer reader = ImageReaderType::New();
      reader->SetFileName( input2 );
      try
      {
	reader->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return EXIT_FAILURE;
      }

      image2 = reader->GetOutput();
    }

    typedef SubtractImageFilter<ImageType, ImageType> FilterType;
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput1 (image1);
    filter->SetInput2 (image2);
    try
    {
      filter->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return EXIT_FAILURE;
    }
    
    // write the image
    ImageWriterType::Pointer writer = ImageWriterType::New();
    writer->SetFileName( output );
    writer->SetInput( filter->GetOutput() );
    
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
  
}
