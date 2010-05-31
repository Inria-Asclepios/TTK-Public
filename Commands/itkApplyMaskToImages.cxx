#include <iostream>

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>
#include <itkImage.h>

#include <itkTensorImageIO.h>

#include "GetPot.h"

void PrintHelp( const char* exec )
{
	std::cout << "Usage: " << std::endl;
	std::cout << exec << " <-i input> <-m mask image> <-o output> <-t type (0: image / 1: tensors)>" << std::endl;
	exit (0);
}

using namespace itk;

int main (int narg, char* arg[])
{

	itk::Object::GlobalWarningDisplayOff();

	GetPot cl(narg, arg); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") )
  {
    PrintHelp(cl[0]);
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
	  exit (-1);
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
	  exit (-1);
  }
  std::cout << " Done." << std::endl;

  ImageType::Pointer maskImage = reader1->GetOutput();


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
      if( itMask.Value() )
      {
	itOut.Set( itIn.Get() );
      }
      else
      {
	itOut.Set( zero );
      }
      
      ++itOut;
      ++itIn;
      ++itMask;
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
      exit (-1);
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
    finalImage->Allocate();
    
    itk::ImageRegionIterator<ImageType> itOut (finalImage, finalImage->GetLargestPossibleRegion() );
    while( !itOut.IsAtEnd() )
    {
      if( itMask.Value() )
      {
	itOut.Set( itIn.Get() );
      }
      else
      {
	itOut.Set( static_cast<IntegerType>(0.0) );
      }
      
      ++itOut;
      ++itIn;
      ++itMask;
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
      exit (-1);
    }
    
    std::cout << " Done." << std::endl;
  }
  

	return 0;
}
