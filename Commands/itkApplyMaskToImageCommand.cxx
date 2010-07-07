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
#include "itkApplyMaskToImageCommand.h"

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>
#include <itkImage.h>
#include <itkTensorImageIO.h>

#include <iostream>
#include "GetPot.h"


namespace itk
{

  ApplyMaskToImageCommand::ApplyMaskToImageCommand()
  {
    m_ShortDescription = "Apply a mask to a (tensor) image";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i input> <-m mask image> <-o output> <-t type (0: image / 1: tensors)>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  ApplyMaskToImageCommand::~ApplyMaskToImageCommand()
  {}

  int ApplyMaskToImageCommand::Execute (int narg, const char* arg[])
  {

    itk::Object::GlobalWarningDisplayOff();
    
    GetPot cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }

    const char* input = cl.follow("NoFile",2,"-I","-i");
    const char* output = cl.follow("NoFile",2,"-O","-o");
    const char* mask = cl.follow( "NoFile", 2, "-m", "-M");
    const int type = cl.follow(0, 2, "-t", "-T");
    
    typedef double                        ScalarType;
    
    typedef unsigned short                IntegerType;
    typedef Image<IntegerType, 3>         ImageType;
    typedef Image<IntegerType, 3>         ImageMaskType;
    
    typedef TensorImageIO<ScalarType,3,3> TensorIOType;
    typedef TensorIOType::TensorImageType TensorImageType;
    typedef TensorIOType::TensorType      TensorType;
    
    typedef itk::ImageFileReader<ImageType> ImageReaderType;
    typedef itk::ImageFileReader<ImageMaskType> ImageMaskReaderType;
    typedef itk::ImageFileWriter<ImageType> ImageWriterType;
    
    ImageReaderType::Pointer reader = ImageReaderType::New();
    reader->SetFileName( input );
    
    TensorIOType::Pointer tensorreader = TensorIOType::New();
    tensorreader->SetFileName( input );
    
    std::cout << "Reading: " << input << std::flush;
    try
    {
      if (type)
	tensorreader->Read();
      else
	reader->Update();
    }
    catch( itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
    ImageType::Pointer image = reader->GetOutput();
    TensorImageType::Pointer tensors = tensorreader->GetOutput();
    
    ImageMaskReaderType::Pointer reader1 = ImageMaskReaderType::New();
    reader1->SetFileName( mask );
  
    std::cout << "Reading: " << mask << std::flush;
    try
    {
      reader1->Update();
    }
    catch( itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
    ImageType::Pointer maskImage = reader1->GetOutput();
    ImageMaskType::IndexType index;
    ImageMaskType::PointType x;
    
    if (type)
    {
      itk::ImageRegionConstIterator<TensorImageType> itIn (tensors, tensors->GetLargestPossibleRegion() );
      itk::ImageRegionConstIterator<ImageMaskType> itMask (maskImage, maskImage->GetLargestPossibleRegion() );
      
      TensorImageType::Pointer finalImage = TensorImageType::New();
      finalImage->SetRegions( tensors->GetLargestPossibleRegion() );
      finalImage->SetOrigin( tensors->GetOrigin() );
      finalImage->SetSpacing( tensors->GetSpacing() );
      finalImage->SetDirection( tensors->GetDirection() );
      finalImage->Allocate();
      TensorType zero = TensorType(static_cast<ScalarType>( 0.0 ));
	
      itk::ImageRegionIterator<TensorImageType> itOut (finalImage, finalImage->GetLargestPossibleRegion() );
      
      while( !itOut.IsAtEnd() )
      {
	tensors->TransformIndexToPhysicalPoint (itOut.GetIndex(), x);
	bool isinside = maskImage->TransformPhysicalPointToIndex (x, index);
	if (!isinside)
	{
	  itOut.Set( zero );
	  ++itOut;
	  ++itIn;
	  continue;
	}
	itMask.SetIndex (index);					
	
	if( itMask.Value() ) itOut.Set( itIn.Get() );
	else                 itOut.Set( zero );
	++itOut;
	++itIn;
      }
      
      
      // write the image
      TensorIOType::Pointer writer = TensorIOType::New();
      writer->SetFileName( output );
      writer->SetInput( finalImage );
      
      std::cout << "Writing: " << output << std::flush;
      try
      {
	writer->Write();
      }
      catch( itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      
      std::cout << " Done." << std::endl;
      
    }
    else
    {
      
      itk::ImageRegionConstIterator<ImageType> itIn (image, image->GetLargestPossibleRegion() );
      itk::ImageRegionConstIterator<ImageMaskType> itMask (maskImage, maskImage->GetLargestPossibleRegion() );
      
      ImageType::Pointer finalImage = ImageType::New();
      finalImage->SetRegions( image->GetLargestPossibleRegion() );
      finalImage->SetOrigin( image->GetOrigin() );
      finalImage->SetSpacing( image->GetSpacing() );
      finalImage->SetDirection( image->GetDirection() );
      
      finalImage->Allocate();
    
      itk::ImageRegionIterator<ImageType> itOut (finalImage, finalImage->GetLargestPossibleRegion() );
      while( !itOut.IsAtEnd() )
      {
	image->TransformIndexToPhysicalPoint (itOut.GetIndex(), x);
	bool isinside = maskImage->TransformPhysicalPointToIndex (x, index);
	if (!isinside)
	{
	  itOut.Set( static_cast<ScalarType>(0.0) );
	  ++itOut;
	  ++itIn;
	  continue;
	}
	itMask.SetIndex (index);				
	
	if( itMask.Value() ) itOut.Set( itIn.Get() );
	else                 itOut.Set( static_cast<ScalarType>(0.0) );
	++itOut;
	++itIn;
      }
  
      // write the image
      ImageWriterType::Pointer writer = ImageWriterType::New();
      writer->SetFileName( output );
      writer->SetInput( finalImage );
      
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
    }
    
    
    return 0;
  }
  
}
