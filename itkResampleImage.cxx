#include <iostream>

#include "GetPot.h"

#include "ttkConfigure.h"

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkResampleImageFilter.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkImage.h>
#include <itkAffineTransform.h>

#ifdef TTK_USE_MIPS
#include "mipsInrimageImageIOFactory.h"
#endif

void PrintHelp( const char* exec)
{
	std::cout << "Usage: " << std::endl;
	std::cout << exec << " <-i input> <-m matrix> <-r reference> <-o output>" << std::endl;
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


  const char* input = cl.follow("NoFile",2,"-I","-i");
  const char* output = cl.follow("NoFile",2,"-O","-o");
  const char* mat    = cl.follow( "NoFile", 2, "-m", "-M");
  const char* ref    = cl.follow( "NoFile", 2, "-r", "-R");

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


  ImageReaderType::Pointer reader2 = ImageReaderType::New();
  reader2->SetFileName( ref );

  std::cout << "Reading: " << ref << std::flush;
  try
  {
	  reader2->Update();
  }
  catch( itk::ExceptionObject &e)
  {
	  std::cerr << e;
	  exit (-1);
  }
  std::cout << " Done." << std::endl;


  
  // read the affine matrix
  std::cout << "Reading: " << mat;
  typedef itk::AffineTransform< ScalarType, 3 >  TransformType;
  TransformType::Pointer transform = TransformType::New();

  TransformType::MatrixType       matrix;
  
  TransformType::OutputVectorType translation (0.0);

  std::ifstream buffer (mat);
  if( buffer.fail() )
  {
    std::cerr << "Error: Cannot open file " << mat << std::endl;
    matrix.SetIdentity();
  }
  else
  {
    
    
    // skip the first 12 floats
    char junk [512];
    for( unsigned int i=0; i<12; i++)
    {
      buffer >> junk;
    }
    
    for( unsigned int i=0 ;i<3; i++)
    {
      buffer >> matrix (i,0);
      buffer >> matrix (i,1);
      buffer >> matrix (i,2);
    }
    
    for( unsigned int i=0; i<3; i++)
    {
      buffer >> translation[i];
    }
  }
  
  transform->SetMatrix (matrix);
  transform->SetTranslation (translation);
  
  TransformType::Pointer inv_transform = TransformType::New();
  transform->GetInverse(inv_transform);
  
  transform = inv_transform;
  
  std::cout << " Done." << std::endl;
  
  
  typedef itk::ResampleImageFilter<ImageType, ImageType> FilterType;
  FilterType::Pointer filter = FilterType::New();
  
  typedef itk::LinearInterpolateImageFunction<ImageType, ScalarType>  InterpolatorType;
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  
  filter->SetInterpolator( interpolator );
  filter->SetInput ( reader->GetOutput() );
  filter->SetOutputOrigin( reader2->GetOutput()->GetOrigin() );
  filter->SetOutputSpacing( reader2->GetOutput()->GetSpacing() );
  filter->SetOutputDirection (reader2->GetOutput()->GetDirection());
  filter->SetSize( reader2->GetOutput()->GetLargestPossibleRegion().GetSize() );
  
  filter->SetTransform( transform );

  std::cout << "Resampling...";
  try
  {
    filter->Update();
  }
  catch (itk::ExceptionObject &e )
  {
    std::cerr << e;
    exit (-1);
  }
  std::cout << "Done." << std::endl;



  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
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
