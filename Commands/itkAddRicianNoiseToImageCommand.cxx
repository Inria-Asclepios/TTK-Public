/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: https://scm.gforge.inria.fr/svn/ttk/trunk/Commands/itkAddRicianNoiseToImageCommand.cxx $
  Language:  C++
  Date:      $Date: 2010-12-10 20:55:58 +0000 (Fri, 10 Dec 2010) $
  Version:   $Revision: 122 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkAddRicianNoiseToImageCommand.h"

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImage.h>
#include "itkAddRicianNoiseImageFilter.h"

#include <iostream>
#include "GetPot.h"


namespace itk
{

  AddRicianNoiseToImageCommand::AddRicianNoiseToImageCommand()
  {
    m_ShortDescription = "Add Rician noise to an image";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i input>\n";
    m_LongDescription += "<-v noise variance>\n";
    m_LongDescription += "<-o output>\n";
    m_LongDescription += "-c toggle random generator (0, default) or reproducible generator (1) \n\n";
    
    m_LongDescription += m_ShortDescription;
  }

  AddRicianNoiseToImageCommand::~AddRicianNoiseToImageCommand()
  {}

  int AddRicianNoiseToImageCommand::Execute (int narg, const char* arg[])
  {
    
    GetPot cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }

    const char* input = cl.follow("NoFile",2,"-I","-i");
    const char* output = cl.follow("NoFile",2,"-O","-o");
    const double variance = cl.follow( 0.005, 2, "-V", "-v");
    const bool IsReproducible = cl.follow( false, 2, "-C", "-c");
    
    using ScalarType      = double;    
    using ImageType       = Image<ScalarType, 3>;
    using ImageReaderType = itk::ImageFileReader<ImageType>;
    using ImageWriterType = itk::ImageFileWriter<ImageType>;
    using FilterType      = itk::AddRicianNoiseImageFilter<ImageType,ImageType>;
    
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
      return EXIT_FAILURE;
    }
    std::cout << " Done." << std::endl;
    
    std::cout << " Adding noise of variance "<<variance<<" and seeding to "<<IsReproducible<< "."<< std::endl;
    ImageType::Pointer image = reader->GetOutput();
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput ( image );
    filter->SetVariance (variance);
    unsigned int seed = 0;
    if (IsReproducible)
      seed = 5323;
    else
      seed = (int) time(nullptr);

    filter->GetNormalGenerator()->Initialize (seed);
    
    try
    {
      filter->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return EXIT_FAILURE;
    }
    std::cout << " Done." << std::endl;
    
    std::cout << "Writing: " << output << std::flush;
    ImageType::Pointer noisyimage = filter->GetOutput();
    ImageWriterType::Pointer writer = ImageWriterType::New();
    writer->SetFileName( output );
    writer->SetInput( noisyimage );
    try
    {
      writer->Update();
    }
    catch( itk::ExceptionObject &e)
    {
      std::cerr << e;
      return EXIT_FAILURE;
    }
    std::cout << " Done." << std::endl;
    
    return EXIT_SUCCESS;
  }
  
}
