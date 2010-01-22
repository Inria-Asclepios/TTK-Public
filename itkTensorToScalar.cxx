#include "itkTensorToScalarTensorImageFilter.h"
#include "itkTensorToScalarFunction.h"
#include "itkTensorToFAFunction.h"
#include "itkTensorToColorFAFunction.h"
#include "itkTensorToLogFAFunction.h"
#include "itkTensorToADCFunction.h"
#include "itkTensorToClFunction.h"
#include "itkTensorToCpFunction.h"
#include "itkTensorToCsFunction.h"
#include "itkTensorToRAFunction.h"
#include "itkTensorToVRFunction.h"
#include "itkTensorToVolumeFunction.h"
#include "itkTensorToLambdaFunction.h"
#include "itkTensorImageIO.h"
#include <itkImage.h>
#include <itkRGBAPixel.h>
#include <itkImageFileWriter.h>

#include "ttkConfigure.h"

#ifdef TTK_USE_MIPS
#include <mipsInrimageImageIOFactory.h>
#endif

#include "GetPot.h"

void PrintHelp (const char* exec)
{

  std::cout << exec << std::endl;
  std::cout << "-i [fileIn]" << std::endl;
  std::cout << "-f [Function: FA / LFA / CFA / ADC / VOL / CL / CP / CS / RA / VR / L1 / L2 / L3]" << std::endl;
  std::cout << "-o [fileOut]" << std::endl;    
  exit (0);
  
}


int main(int narg, char* arg[])
{

	itk::Object::GlobalWarningDisplayOff();

  GetPot cl (narg, arg);
  if( cl.size() == 1 || cl.search(2, "--help", "-h") ) 
    PrintHelp(cl[0]);

  const bool IsInputPresent = cl.search(2,"-i","-I");
  const bool IsOutputPresent = cl.search(2,"-o","-O");
  
  if(!IsInputPresent || !IsOutputPresent)
    {
      std::cerr << "Input file and/or output not set" << std::endl;
      return -1;
    }

  const bool IsFunctionPresent = cl.search (2, "-f","-F");
  if (!IsFunctionPresent)
  {
    std::cerr << "Error: Function not specified." << std::endl;
    return -1;    
  }


  const char* file_in = cl.follow("NoFile", 2, "-i","-I");
  const char* file_out = cl.follow("NoFile", 2, "-o","-O");
  const char* function = cl.follow ("NoFunction", 2, "-f", "-F");

  if(strcmp(file_in,"NoFile")==0 || strcmp(file_out,"NoFile")==0)
  {
    std::cerr << "Input file and/or output not set" << std::endl;
    return -1;
  }

  if (strcmp (function, "NoFunction")==0)
  {
    std::cerr << "Error: Function not specified." << std::endl;
    return -1;
  }


  // Read in a tensor field:
  typedef double                               ScalarType;
  typedef itk::RGBAPixel<unsigned char>        ColorType;
  typedef itk::TensorImageIO<ScalarType, 3, 3> IOType;
  typedef IOType::TensorImageType              TensorImageType;
  typedef itk::Image<ScalarType, 3>            ScalarImageType;
  typedef itk::Image<ColorType, 3>             ColorImageType;
    
  IOType::Pointer io = IOType::New();
  io->SetFileName(file_in);

  std::cout << "Reading: " << file_in;
  std::cout << std::flush;  
  try
  {
    io->Read();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e;
    exit(-1);
  }
  std::cout << "Done." << std::endl;


  // choose the function
  typedef  itk::TensorToScalarFunction<TensorImageType::PixelType, ScalarImageType::PixelType>
    FunctionType;
  typedef  itk::TensorToScalarFunction<TensorImageType::PixelType, ColorImageType::PixelType>
    ColorFunctionType;
  FunctionType::Pointer myFunction;
  ColorFunctionType::Pointer myColorFunction;
  bool foundFunction = false;
  bool foundColorFunction = false;
  
  if( strcmp (function, "fa")==0 || strcmp (function, "FA")==0 )
  {
    std::cout << "Computing the FA...";
    myFunction = itk::TensorToFAFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::New();
    foundFunction = true;    
  }

  if( strcmp (function, "lfa")==0 || strcmp (function, "LFA")==0 )
  {
    std::cout << "Computing the Log FA...";
    myFunction = itk::TensorToLogFAFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::New();
    foundFunction = true;    
  }
  
  if( strcmp (function, "cfa")==0 || strcmp (function, "CFA")==0 )
  {
    std::cout << "Computing the Color FA...";
    myColorFunction = itk::TensorToColorFAFunction<TensorImageType::PixelType, ColorImageType::PixelType>::New();
    foundColorFunction = true;
  }
  

  if( strcmp (function, "adc")==0 || strcmp (function, "ADC")==0 )
  {
    std::cout << "Computing the ADC...";
    myFunction = itk::TensorToADCFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::New();
    foundFunction = true;    
  }

  if( strcmp (function, "vol")==0 || strcmp (function, "VOL")==0 )
  {
    std::cout << "Computing the volume...";
    myFunction = itk::TensorToVolumeFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::New();
    foundFunction = true;    
  }

  if( strcmp (function, "cl")==0 || strcmp (function, "CL")==0 )
  {
    std::cout << "Computing cl...";
    myFunction = itk::TensorToClFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::New();
    foundFunction = true;    
  }

  if( strcmp (function, "cp")==0 || strcmp (function, "CP")==0 )
  {
    std::cout << "Computing cp...";
    myFunction = itk::TensorToCpFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::New();
    foundFunction = true;    
  }

  if( strcmp (function, "cs")==0 || strcmp (function, "CS")==0 )
  {
    std::cout << "Computing cs...";
    myFunction = itk::TensorToCsFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::New();
    foundFunction = true;    
  }

  if( strcmp (function, "ra")==0 || strcmp (function, "RA")==0 )
  {
    std::cout << "Computing the RA...";
    myFunction = itk::TensorToRAFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::New();
    foundFunction = true;    
  }

  if( strcmp (function, "vr")==0 || strcmp (function, "VR")==0 )
  {
    std::cout << "Computing the VR...";
    myFunction = itk::TensorToVRFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::New();
    foundFunction = true;    
  }

  if( strcmp (function, "l1")==0 || strcmp (function, "L1")==0 )
  {
    std::cout << "Computing L1...";
	itk::TensorToLambdaFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::Pointer newFunc = itk::TensorToLambdaFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::New();
	newFunc->SetLambdaIndex( 2 );
	myFunction = newFunc;
    foundFunction = true;    
  }

  if( strcmp (function, "l2")==0 || strcmp (function, "L2")==0 )
  {
    std::cout << "Computing L2...";
	itk::TensorToLambdaFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::Pointer newFunc = itk::TensorToLambdaFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::New();
	newFunc->SetLambdaIndex( 1 );
	myFunction = newFunc;
    foundFunction = true;    
  }

  if( strcmp (function, "l3")==0 || strcmp (function, "L3")==0 )
  {
    std::cout << "Computing L3...";	
	itk::TensorToLambdaFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::Pointer newFunc = itk::TensorToLambdaFunction<TensorImageType::PixelType, ScalarImageType::PixelType>::New();
	newFunc->SetLambdaIndex( 0 );
	myFunction = newFunc;
    foundFunction = true;    
  }

  std::cout << std::flush;
  
  if( foundFunction )
  {
    
    // compute the function
    typedef itk::TensorToScalarTensorImageFilter<TensorImageType, ScalarImageType>
      FilterType;
    
    FilterType::Pointer myFilter = FilterType::New();
    myFilter->SetTensorToScalarFunction (myFunction);
    myFilter->SetInput (io->GetOutput());
    
    
    try
    {
      myFilter->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;    
    }
    std::cout << "Done." << std::endl;
    
    
    
    // write the output
#ifdef TTK_USE_MIPS
    itk::InrimageImageIOFactory::RegisterOneFactory();
#endif
    
    typedef itk::ImageFileWriter<ScalarImageType> WriterType;
    
    WriterType::Pointer myWriter = WriterType::New();
    myWriter->SetFileName(file_out);
    myWriter->SetInput(myFilter->GetOutput());
    
    std::cout << "Writing: " << file_out;
    std::cout << std::flush;
    try
    {
      myWriter->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;  

  }
  
    
  if( foundColorFunction )
  {

    typedef itk::TensorToScalarTensorImageFilter<TensorImageType, ColorImageType>
      FilterType;
    FilterType::Pointer myFilter = FilterType::New();
    myFilter->SetTensorToScalarFunction (myColorFunction);
    myFilter->SetInput (io->GetOutput());
    
    
    try
    {
      myFilter->Update();
    }
    catch (itk::ExceptionObject &e)
    {
    std::cerr << e;
    return -1;    
    }
    std::cout << "Done." << std::endl;

    // write the output
    typedef itk::ImageFileWriter<ColorImageType> WriterType;
    
    WriterType::Pointer myWriter = WriterType::New();
    myWriter->SetFileName(file_out);
    myWriter->SetInput(myFilter->GetOutput());
    
    std::cout << "Writing: " << file_out;
    std::cout << std::flush;
    try
    {
      myWriter->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl; 
    
  }
  
  return 0;
  
}
