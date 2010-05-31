#include <iostream>

#include "GetPot.h"

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkResampleImageFilter.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkImage.h>
#include <itkAffineTransform.h>
#include <itkVector.h>
#include <itkWarpImageFilter.h>

#include "ttkConfigure.h"

#ifdef TTK_USE_MIPS
#include "mipsInrimageImageIOFactory.h"
#endif

void PrintHelp( const char* exec)
{
	std::cout << "Usage: " << std::endl;
	std::cout << exec << " <-i input> <-d defomation field> <-o output>" << std::endl;
	exit(0);
}

using namespace itk;

int main( int narg, char* arg[])
{

  itk::Object::GlobalWarningDisplayOff();

  GetPot cl(narg, arg); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") )
  {
    PrintHelp(cl[0]);
  }


  const char* input    = cl.follow("NoFile",2,"-I","-i");
  const char* output   = cl.follow("NoFile",2,"-O","-o");
  const char* deffield = cl.follow( "NoFile", 2, "-d", "-D");

  typedef double ScalarType;
  typedef Image<ScalarType, 3>         ImageType;

#ifdef TTK_USE_MIPS
  itk::InrimageImageIOFactory::RegisterOneFactory();
#endif

  typedef itk::ImageFileReader<ImageType> ImageReaderType;
  typedef itk::ImageFileWriter<ImageType> WriterType;


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
	  exit (-1);
  }
  std::cout << " Done." << std::endl;

  
  // read the deformation field
  std::cout << "Reading: " << deffield;
  typedef Vector<ScalarType, 3> VectorType;
  typedef Image<VectorType, 3> VectorImageType;
  ImageFileReader<VectorImageType>::Pointer readerDefField = 
	ImageFileReader<VectorImageType>::New();
  readerDefField->SetFileName( deffield );
  try
  {
	  readerDefField->Update();
  }
  catch( itk::ExceptionObject &e)
  {
	  std::cerr << e;
	  exit (-1);
  }
  std::cout << " Done." << std::endl;


  typedef itk::WarpImageFilter < ImageType, ImageType, VectorImageType >  WarperType;
    WarperType::Pointer warper = WarperType::New();
    warper->SetInput( reader->GetOutput() );
    warper->SetOutputSpacing( readerDefField->GetOutput()->GetSpacing() );
    warper->SetOutputOrigin( readerDefField->GetOutput()->GetOrigin() );
    warper->SetOutputDirection( readerDefField->GetOutput()->GetDirection() );
    warper->SetDeformationField( readerDefField->GetOutput() );

	std::cout << "Warping..." << std::endl;
    try
    {
      warper->Update();
    }
    catch( itk::ExceptionObject& err )
    {      
      std::cout << err << std::endl;
      exit( -1);
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
    exit (-1);
  }
  std::cout << " Done." << std::endl;

	return 0;
}
