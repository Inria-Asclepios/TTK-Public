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
#include "itkWarpImageCommand.h"

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkResampleImageFilter.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkImage.h>
#include <itkAffineTransform.h>
#include <itkVector.h>
#include <itkWarpImageFilter.h>

#include <iostream>
#include "GetPot.h"

#include "ttkConfigure.h"

#ifdef TTK_USE_MIPS
#include "mipsInrimageImageIOFactory.h"
#endif

namespace itk
{

  WarpImageCommand::WarpImageCommand()
  {
    m_ShortDescription = "Warp an image give a deformation field";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i input> <-d defomation field> <-o output>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  WarpImageCommand::~WarpImageCommand()
  {}
  
  int WarpImageCommand::Execute( int narg, const char* arg[])
  {

    itk::Object::GlobalWarningDisplayOff();
    
    GetPot cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
    
    const char* input    = cl.follow("NoFile",2,"-I","-i");
    const char* output   = cl.follow("NoFile",2,"-O","-o");
    const char* deffield = cl.follow( "NoFile", 2, "-d", "-D");

    using ScalarType = double;
    using ImageType  = Image<ScalarType, 3>;
    
#ifdef TTK_USE_MIPS
    itk::InrimageImageIOFactory::RegisterOneFactory();
#endif
    
    using ImageReaderType = itk::ImageFileReader<ImageType>;
    using WriterType      = itk::ImageFileWriter<ImageType>;
    
    
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
    
    
    // read the deformation field
    std::cout << "Reading: " << deffield;
    using VectorType      = Vector<ScalarType, 3>;
    using VectorImageType = Image<VectorType, 3>;
    ImageFileReader<VectorImageType>::Pointer readerDefField = ImageFileReader<VectorImageType>::New();
    readerDefField->SetFileName( deffield );
    try
    {
      readerDefField->Update();
    }
    catch( itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
    
    using WarperType = itk::WarpImageFilter < ImageType, ImageType, VectorImageType >;
    WarperType::Pointer warper = WarperType::New();
    warper->SetInput( reader->GetOutput() );
    warper->SetOutputSpacing( readerDefField->GetOutput()->GetSpacing() );
    warper->SetOutputOrigin( readerDefField->GetOutput()->GetOrigin() );
    warper->SetOutputDirection( readerDefField->GetOutput()->GetDirection() );
    warper->SetDisplacementField( readerDefField->GetOutput() );
    
    std::cout << "Warping..." << std::endl;
    try
    {
      warper->Update();
    }
    catch( itk::ExceptionObject& err )
    {      
      std::cout << err << std::endl;
      return -1;
    }
    std::cout << "Done." << std::endl;

    
    WriterType::Pointer writer = WriterType::New();
    writer->SetInput( warper->GetOutput() );
    writer->SetFileName( output );
    
    std::cout << "Writing: " << output;
    try
    {
      writer->Update();
    }
    catch (itk::ExceptionObject &e )
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
    return 0;
  }
  
}
