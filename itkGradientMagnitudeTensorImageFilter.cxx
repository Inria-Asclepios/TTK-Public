#include "itkTensorImageIO.h"

#include "GetPot.h"

#include <itkImage.h>
#include <itkImageFileWriter.h>
#include <itkVector.h>


#include "itkLogTensorImageFilter.h"
#include "itkTensorImageToVectorImage.h"
#include "itkVectorGradientMagnitudeImageFilter.h"

//#include "itkGradientMagnitudeTensorImageFilter.h"

#include <itkDiscreteGaussianImageFilter.h>
#include <itkImageRegionIteratorWithIndex.h>



void PrintHelp (const char* exec)
{
  std::cout << exec << ": " << std::endl;
  std::cout << "-i [fileIn]" << std::endl;
  std::cout << "-s [sigma]" << std::endl;
  std::cout << "-euc" << std::endl;
  std::cout << "-o [fileOut]" << std::endl;
  exit(0);
}




int main (int narg, char* arg[])
{

  itk::Object::GlobalWarningDisplayOff();
    
  // parse arguments
  GetPot cl (narg, arg);
  if( cl.size() == 1 || cl.search (2,"--help","-h") )
    PrintHelp (cl[0]);

  
  const bool IsInputPresent = cl.search(2,"-i","-I");
  const bool IsOutputPresent = cl.search(2,"-o","-O");
  const bool euclidean = cl.search (1,"-euc");
  const double sigma = cl.follow (1.0, 2, "-s", "-S");
  
  if(!IsInputPresent || !IsOutputPresent)
  {
    std::cerr << "Input file and/or output not set" << std::endl;
    return -1;
  }
  
  
  const char* file_in = cl.follow("NoFile", 2, "-i","-I");
  const char* file_out = cl.follow("NoFile", 2, "-o","-O");
  if(strcmp(file_in,"NoFile")==0 || strcmp(file_out,"NoFile")==0)
  {
    std::cerr << "Input file and/or output not set" << std::endl;
    exit (-1);
  }


  // Read the input:
  typedef double  ScalarType;
  const unsigned int ImageDimension  = 3;
  const unsigned int TensorDimension = 3;
  
  typedef itk::TensorImageIO<ScalarType, TensorDimension, ImageDimension> IOType;
  typedef IOType::TensorImageType                                         TensorImageType;


  TensorImageType::Pointer tensorImage = 0;
  {
    IOType::Pointer myReader = IOType::New();
    myReader->SetFileName (file_in);
    
    std::cout << "Reading: " << file_in;
    std::cout << std::flush;  
    try
    {
      myReader->Read();    
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      exit (-1);    
    }
    std::cout << " Done." << std::endl;

    tensorImage = myReader->GetOutput();
    
  }
  

  /** Specific typedefs */
  typedef itk::LogTensorImageFilter<TensorImageType,TensorImageType>  LogFilterType;
  typedef itk::Image<ScalarType, ImageDimension>                      ScalarImageType;

  typedef itk::VectorGradientMagnitudeImageFilter<TensorImageType, ScalarType, ScalarImageType>  GradientFilterType;
  typedef itk::DiscreteGaussianImageFilter<TensorImageType, TensorImageType>                     GaussianFilterType;

  
  if( !euclidean )
  {
    LogFilterType::Pointer myLog = LogFilterType::New();
    myLog->SetInput ( tensorImage );

    try
    {
      myLog->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      exit (-1);    
    }

    tensorImage = myLog->GetOutput();
  }



  GaussianFilterType::Pointer gaussian = GaussianFilterType::New();
  gaussian->SetInput ( tensorImage );
  gaussian->SetVariance ( sigma );
 
  GradientFilterType::Pointer gradient = GradientFilterType::New();
  gradient->SetInput ( gaussian->GetOutput() );

  try
  {
    gradient->Update();
  } catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }

  
  
  
  // save the image:
  typedef itk::ImageFileWriter<ScalarImageType> WriterType;
  WriterType::Pointer myWriter = WriterType::New();
  myWriter->SetFileName(file_out);
  myWriter->SetInput( gradient->GetOutput() );
  
  std::cout << "Writing: " << file_out;
  std::cout << std::flush;
  try
  {
    myWriter->Update();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e;
    exit(-1);
  }
  std::cout << " Done." << std::endl;  
  
  
  return 0;  
}
