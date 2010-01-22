#include "GetPot.h"
#include "itkLogTensorImageFilter.h"
#include "itkTensorImageIO.h"
#include "itkTensor.h"
#include "itkFiber.h"
#include "itkFiberTrackingImageFilter.h"
#include "itkFiberImageToVtkPolyData.h"
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <vtkPolyData.h>
#include <vtkPolyDataWriter.h>

void PrintHelp (const char* exec)
{
  std::cout << exec << std::endl;
  std::cout << "-i [tensor field]" << std::endl;
  std::cout << "-fa1 [fa threshold #1]" << std::endl;
  std::cout << "-fa2 [fa threshold #2]" << std::endl;
  std::cout << "-s [smoothness]" << std::endl;
  std::cout << "-samp [sampling]" << std::endl;
  std::cout << "-t [time step]" << std::endl;
  std::cout << "-m [integration method 0/1/2]" << std::endl;
  std::cout << "-min [minimum length (in mm)]" << std::endl;
  std::cout << "-max [maximum length (in mm)]" << std::endl;
  std::cout << "-seed [filename]" << std::endl;
  std::cout << "-fiberseeds [filename]" << std::endl;
  std::cout << "-l [tri-linear interpolation]" << std::endl;
  std::cout << "-n [number of threads]" << std::endl;
  std::cout << "-o [output]" << std::endl;
  exit (0);
}


int main (int narg, char* arg[])
{
  
  GetPot   cl(narg, arg); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    PrintHelp(cl[0]);


  const char* input  = cl.follow("NoFile",2,"-i","-I");
  const char* output = cl.follow("NoFile",2,"-o","-O");
  const char* seed_name = cl.follow("NoFile",2,"-seed","-SEED");
  const char* fiberseeds_name = cl.follow("NoFile",2,"-fiberseeds","-FIBERSEEDS");
  const double fa1    = cl.follow(0.2,2,"-fa1","-FA1");
  const double fa2    = cl.follow(0.2,2,"-fa2","-FA2");
  const double dt    = cl.follow(1.0,2,"-t","-T");
  const double s    = cl.follow(0.0,2,"-s","-S");
  const double min   = cl.follow(0.0,2,"-min","-MIN");
  const double max   = cl.follow(1000.0,2,"-max","-MAX");
  const int sampling  = cl.follow(1,2,"-samp","-SAMP");
  const int threads = cl.follow (1, 2, "-n","-N");
  const bool trili  = cl.follow (0, 2, "-l","-L");
  const int method = cl.follow (0, 2, "-m", "-M");
  

  if( strcmp (input, "NoFile")==0 || strcmp (output, "NoFile")==0 )
  {
    std::cerr << "Error: Input or output not set." << std::endl;
    return -1;
  }


  // typedefs
  typedef double                                ScalarType;  
  typedef itk::TensorImageIO<ScalarType, 3 ,3>  IOType;
  typedef IOType::TensorImageType               TensorImageType;
  typedef TensorImageType::PixelType            TensorType;
  typedef itk::Fiber<ScalarType, 3>             FiberType;
  typedef itk::Image<FiberType, 3>              FiberImageType;
  typedef itk::Image<ScalarType, 3>             ImageType;
  

  // read in the tensor field
  
  // read the tensor field
  IOType::Pointer tensReader = IOType::New();  
  tensReader->SetFileName(input);
  std::cout << "Reading..." << std::flush;
  try
  {
    tensReader->Read();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e;
    exit(-1);
    }
  std::cout << "Done." << std::endl;
  
  TensorImageType::Pointer tensors = tensReader->GetOutput();
  
  


  typedef itk::LogTensorImageFilter<TensorImageType, TensorImageType> LogFilterType;
  LogFilterType::Pointer loger = LogFilterType::New();
  loger->SetInput ( tensors );
  try
  {
    loger->Update();
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }
  
  

  typedef itk::FiberTrackingImageFilter<TensorImageType, FiberImageType>
    FiberTrackingFilterType;

  FiberTrackingFilterType::Pointer myFiberTracking = FiberTrackingFilterType::New();
  myFiberTracking->SetInput (tensors);
  myFiberTracking->SetFAThreshold (fa1);
  myFiberTracking->SetFAThreshold2 (fa2);
  myFiberTracking->SetSmoothness (s);
  myFiberTracking->SetTimeStep (dt);
  myFiberTracking->SetSampling (sampling);
  myFiberTracking->SetNumberOfThreads (threads);
  myFiberTracking->SetUseTriLinearInterpolation (trili);
  myFiberTracking->SetLogTensorImage ( loger->GetOutput() );
  myFiberTracking->SetIntegrationMethod ( method );
  myFiberTracking->SetMinLength (min);
  myFiberTracking->SetMaxLength (max);


  if( strcmp(seed_name, "NoFile")!=0 )
  {
    typedef itk::ImageFileReader<ImageType> ImageReaderType;
    ImageReaderType::Pointer reader = ImageReaderType::New();
    reader->SetFileName(seed_name);
    try
    {
      reader->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      exit (-1);
    }
    
    myFiberTracking->SetSeedImage( reader->GetOutput() );
  }

  std::cout << "Tracking in progress..."<< std::flush;
  try
  {
    myFiberTracking->Update();
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }
  std::cout << "Done." << std::endl;

  std::cout << "Converting..." << std::flush;
  typedef itk::FiberImageToVtkPolyData<FiberImageType, TensorImageType> ConverterType;
  ConverterType::Pointer myConverter = ConverterType::New();
  myConverter->SetInput (myFiberTracking->GetOutput());
  myConverter->SetTensorImage (tensors);
  myConverter->SetLogTensorImage (loger->GetOutput() );
  myConverter->Update();
  vtkPolyData* fibers = myConverter->GetOutput();
  std::cout << "Done." << std::endl;


  std::cout << "Writing..."<< std::flush;
  vtkPolyDataWriter* myWriter = vtkPolyDataWriter::New();
  myWriter->SetFileName(output);
  myWriter->SetInput (fibers);
  myWriter->SetFileTypeToBinary();
  myWriter->Write();
  myWriter->Delete();
  std::cout << "Done." << std::endl;


  std::cout << "Writing seeds..."<< std::flush;
  itk::ImageFileWriter<ImageType>::Pointer writer = itk::ImageFileWriter<ImageType>::New();
  writer->SetFileName (fiberseeds_name);
  writer->SetInput ( myFiberTracking->GetFibersSeededImage() );
  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }
  std::cout << "Done." << std::endl;
  
  
  
  return 0;
}
