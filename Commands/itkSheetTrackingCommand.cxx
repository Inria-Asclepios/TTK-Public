
#include "itkSheetTrackingCommand.h"

#include "itkLogTensorImageFilter.h"
#include "itkTensorImageIO.h"
#include "itkTensor.h"
#include "itkSheet.h"
#include "itkProlateSheetTrackingImageFilter.h"
#include "itkSheetImageToVtkPolyData.h"
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <vtkPolyData.h>
#include <vtkPolyDataWriter.h>

#include "GetPot.h"

namespace itk
{
  

  SheetTrackingCommand::SheetTrackingCommand()
  {
    m_ShortDescription = "Perform streamline sheet tracking using a Log-Euclidean advection-diffusion scheme";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "-i [tensor field]\n";
    m_LongDescription += "-fa1 [fa threshold #1]\n";
    m_LongDescription += "-fa2 [fa threshold #2]\n";
    m_LongDescription += "-s [smoothness]\n";
    m_LongDescription += "-f [normal steefness ( > 1.0) ]\n";
    m_LongDescription += "-samp [sampling]\n";
    m_LongDescription += "-ss [sheet sampling]\n";
    m_LongDescription += "-t [time step]\n";
    m_LongDescription += "-d [maximum depth (default: 30)]\n";
    m_LongDescription += "-m [integration method 0/1/2]\n";
    m_LongDescription += "-min [minimum surface (in mm2)]\n";
    m_LongDescription += "-max [maximum surface (in mm2)]\n";
    m_LongDescription += "-seed [filename]\n";
    m_LongDescription += "-l [tri-linear interpolation]\n";
    m_LongDescription += "-n [number of threads]\n";
    m_LongDescription += "-o [output]\n";
    m_LongDescription += "-c [correct for tensor image orientation]\n\n";
    m_LongDescription += "-g1 [g-1 image]\n\n";
    m_LongDescription += "-g3 [g-3 image]\n\n";
    
    m_LongDescription += m_ShortDescription;
  }

  SheetTrackingCommand::~SheetTrackingCommand()
  {}
 
  int SheetTrackingCommand::Execute (int narg, const char* arg[])
  {
  
    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }

    const char* input  = cl.follow("NoFile",2,"-i","-I");
    const char* output = cl.follow("NoFile",2,"-o","-O");
    const char* seed_name = cl.follow("NoFile",2,"-seed","-SEED");
    const double fa1    = cl.follow(0.2,2,"-fa1","-FA1");
    const double fa2    = cl.follow(0.2,2,"-fa2","-FA2");
    const double dt    = cl.follow(1.0,2,"-t","-T");
    const double f    = cl.follow(1.0,2,"-f","-F");
    const double d    = cl.follow(30,2,"-d","-D");
    const double s    = cl.follow(0.0,2,"-s","-S");
    const double sheetSampling = cl.follow(0.0,2,"-ss","-SS");
    const double min   = cl.follow(0.0,2,"-min","-MIN");
    const double max   = cl.follow(1000.0,2,"-max","-MAX");
    const int sampling  = cl.follow(1,2,"-samp","-SAMP");
    const int threads = cl.follow (1, 2, "-n","-N");
    const bool trili  = cl.search (2, "-l","-L");
    const char* g1image  = cl.follow ("NoFile", 2,"-g1","-G1");
    const char* g3image  = cl.follow ("NoFile", 2,"-g3","-G3");
    const bool correction  = cl.search (1, "-c","-C");
    
    const int method = cl.follow (0, 2, "-m", "-M");
    
    
    if( strcmp (input, "NoFile")==0 || strcmp (output, "NoFile")==0 )
    {
      std::cerr << "Error: Input or output not set." << std::endl;
      return -1;
    }
    

    // typedefs
    typedef float                                 ScalarType;
    typedef itk::TensorImageIO<ScalarType, 3 ,3>  IOType;
    typedef IOType::TensorImageType               TensorImageType;
    typedef TensorImageType::PixelType            TensorType;
    typedef itk::Sheet<ScalarType, 3>             SheetType;
    typedef itk::Image<SheetType, 3>              SheetImageType;
    typedef itk::Image<unsigned int, 3>          ImageType;
    
    
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
      return -1;
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

    typedef itk::ProlateSheetTrackingImageFilter<TensorImageType, SheetImageType>
      SheetTrackingFilterType;
    typedef SheetTrackingFilterType::VectorImageType VectorImageType;
    typedef itk::ImageFileReader<VectorImageType> GReaderType;
    
    GReaderType::Pointer g1reader = GReaderType::New();
    g1reader->SetFileName(g1image);
    g1reader->Update();
    
    GReaderType::Pointer g3reader = GReaderType::New();
    g3reader->SetFileName(g3image);
    g3reader->Update();
    
    
    SheetTrackingFilterType::Pointer mySheetTracking = SheetTrackingFilterType::New();
    mySheetTracking->SetInput (tensors);
    mySheetTracking->SetFAThreshold (fa1);
    mySheetTracking->SetFAThreshold2 (fa2);
    mySheetTracking->SetSmoothness (s);
    mySheetTracking->SetNormalSteefness (f);
    mySheetTracking->SetTimeStep (dt);
    mySheetTracking->SetOutputSheetSampling (sheetSampling);
    mySheetTracking->SetSampling (sampling);
    mySheetTracking->SetNumberOfWorkUnits(threads);
    mySheetTracking->SetUseTriLinearInterpolation (trili);
    mySheetTracking->SetLogTensorImage ( loger->GetOutput() );
    mySheetTracking->SetIntegrationMethod ( method );
    mySheetTracking->SetMinSurface (min);
    mySheetTracking->SetMaxSurface (max);
    mySheetTracking->SetMaxDepth (d);
    mySheetTracking->SetTransformTensorWithImageDirection (correction);
    mySheetTracking->SetG1Image (g1reader->GetOutput());
    mySheetTracking->SetG3Image (g3reader->GetOutput());
    
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
	return -1;
      }
      
      mySheetTracking->SetSeedImage( reader->GetOutput() );
    }
    
    std::cout << "Tracking in progress..."<< std::endl<<std::flush;

    CommandIterationUpdate::Pointer callback = CommandIterationUpdate::New();
    mySheetTracking->AddObserver (itk::ProgressEvent(), callback);
    try
    {
      mySheetTracking->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;
    
    std::cout << "Converting..." << std::flush;
    typedef itk::SheetImageToVtkPolyData<SheetImageType> ConverterType;
    ConverterType::Pointer myConverter = ConverterType::New();
    myConverter->SetInput (mySheetTracking->GetOutput());
    myConverter->Update();
    vtkPolyData* sheets = myConverter->GetOutput();
    std::cout << "Done." << std::endl;

    std::cout << *sheets << std::endl;

    std::cout << "Writing..."<< std::flush;
    vtkPolyDataWriter* myWriter = vtkPolyDataWriter::New();
    myWriter->SetFileName(output);
    myWriter->SetInputData (sheets);
    // myWriter->SetFileTypeToBinary();
    myWriter->Write();
    myWriter->Delete();
    std::cout << "Done." << std::endl;

    return 0;
  }
  
}
