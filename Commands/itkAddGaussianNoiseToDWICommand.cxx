#include "itkAddGaussianNoiseToDWICommand.h"

#include "itkGradientFileReader.h"
// #include "itkAddGaussianNoiseImageFilter.h"
#include "itkAddRicianNoiseImageFilter.h"
#include "itkTensorImageIO.h"
#include "itkTensorToScalarTensorImageFilter.h"
#include "itkTensorToL2NormFunction.h"
#include "itkTensorsToDWITensorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkDTIEstimatorTensorImageFilter.h"
#include "itkRemoveNonPositiveTensorsTensorImageFilter.h"
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImage.h>

#include "GetPot.h"


namespace itk
{


  AddGaussianNoiseToDWICommand::AddGaussianNoiseToDWICommand()
  {
    m_ShortDescription = "Generate DWI, add Gaussian noise on them, and re-estimate tensors";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "-i [tensor field]\n";
    m_LongDescription += "-v [variance]\n";
    m_LongDescription += "-o [Output file]\n";
    m_LongDescription += "-g [gradients]\n";
    m_LongDescription += "-b [b-0 image]\n";
    m_LongDescription += "-c toggle random generator (0, default) or reproducible generator (1)\n";
    m_LongDescription += "-r [0/1] toggle the noise type (0: Gaussian / 1: Rician) \n\n";    
    m_LongDescription += m_ShortDescription;
  }

  AddGaussianNoiseToDWICommand::~AddGaussianNoiseToDWICommand()
  {}

  int AddGaussianNoiseToDWICommand::Execute(int narg, const char *arg[])
  {

    bool debugmode = 0;
    
    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
  
    const bool IsInputPresent    = cl.search(2,"-i","-I");
    const bool IsB0Present       = cl.search(2,"-b","-B");
    const bool IsOutputPresent   = cl.search(2,"-o","-O");
    
  
    if(!IsInputPresent || !IsOutputPresent )
    {
      std::cerr << "Error: Input and (or) output not set." << std::endl;
      return -1;
    }


    const char* fileIn   = cl.follow("NoFile",2,"-i","-I");
    const char* fileOut  = cl.follow("NoFile",2,"-o","-O");
    const char* gradients_file = cl.follow ("NoFile",2,"-g","-G");
    const double variance = cl.follow(1.0, 2,"-v","-V");
    const bool IsReproducible = cl.follow(false, 2,"-c","-C");
    const bool IsNoiseRician = cl.follow(false, 2,"-r","-R");
    
    
    typedef double ScalarType;  
    typedef itk::Image<ScalarType, 3>                      ImageType;
    typedef itk::Image<ScalarType, 3>                      ImageType;
    typedef itk::ImageFileWriter<ImageType>                ImageWriterType;
    typedef itk::ImageFileReader<ImageType>                ImageReaderType;
    
    typedef itk::TensorImageIO<ScalarType, 3, 3>           IOType;
    typedef IOType::TensorImageType                        TensorImageType;
    typedef TensorImageType::PixelType                     TensorType;
    typedef itk::GradientFileReader                        GradientReaderType;
    typedef itk::AddGaussianNoiseImageFilter<ImageType,ImageType>            GaussianFilterType;
    typedef itk::AddRicianNoiseImageFilter<ImageType,ImageType>              RicianFilterType;
    typedef itk::TensorToScalarTensorImageFilter<TensorImageType, ImageType> TensorToScalarFilterType;
    typedef itk::TensorToL2NormFunction<TensorType, ScalarType>              TensorFunctionType;
    typedef itk::DTIEstimatorTensorImageFilter<ImageType, TensorImageType>   EstimatorType;
    typedef EstimatorType::GradientType                                      GradientType;
    typedef itk::RemoveNonPositiveTensorsTensorImageFilter<TensorImageType,TensorImageType> RemoveNPTFilterType;

    typedef itk::TensorsToDWITensorImageFilter<TensorImageType, ImageType>   TensorsToDWIFilter;
    typedef TensorsToDWIFilter::GradientType                                 GradientType;
    typedef TensorsToDWIFilter::GradientListType                             GradientListType;

    std::cout<<"reading "<<fileIn<<"..."<<std::flush;
    IOType::Pointer myReader = IOType::New();
    myReader->SetFileName (fileIn);
    try
    {
      myReader->Read();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    
    TensorImageType::Pointer myTensorImage = myReader->GetOutput();
    std::cout<<"\t OK."<<std::endl;

    ImageType::Pointer myB0 = nullptr;
    if (IsB0Present)
    {
      const char* fileb0   = cl.follow("NoFile",2,"-b","-B");
      std::cout<<"reading "<<fileb0<<"..."<<std::flush;
      ImageReaderType::Pointer imreader = ImageReaderType::New();
      imreader->SetFileName (fileb0);
      imreader->Update();
      myB0 = imreader->GetOutput();
      std::cout<<"\t OK."<<std::endl;
    }
    else
    { 
      TensorToScalarFilterType::Pointer myFilter1 = TensorToScalarFilterType::New();
      TensorFunctionType::Pointer myFunction = TensorFunctionType::New();
      myFilter1->SetTensorToScalarFunction ( myFunction );
      myFilter1->SetInput (myTensorImage);
      
      try
      {
    	myFilter1->Update();
      }
      catch (itk::ExceptionObject &e)
      {
    	std::cerr << e;
    	return EXIT_FAILURE;
      }
      
      myB0 = myFilter1->GetOutput();
    }

    std::cout<<"reading "<<gradients_file<<"..."<<std::flush;
    GradientReaderType::Pointer gradientreader = GradientReaderType::New();
    gradientreader->SetFileName (gradients_file);
    
    try
    {
      gradientreader->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return EXIT_FAILURE;
    }
    
    GradientListType myGradients = gradientreader->GetGradientList();
    std::cout<<"\t OK."<<" size : "<<myGradients.size()<<std::endl;
    

    std::cout << "Transforming tensors to DWIs ..." << std::flush;
    TensorsToDWIFilter::Pointer myFilter2 = TensorsToDWIFilter::New();
    myFilter2->SetBaselineImage (myB0);
    myFilter2->SetBValue (1);
    myFilter2->SetGradientList (myGradients);
    myFilter2->SetInput (myTensorImage);
    
    try
    {
      myFilter2->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return EXIT_FAILURE;
    }
    std::cout<<"\t OK."<<std::endl;

    // ImageWriterType::Pointer imagewriter = ImageWriterType::New();
    // for (unsigned int i=0; i<myFilter2->GetNumberOfOutputs(); i++)
    // {
    //   imagewriter->SetInput (myFilter2->GetOutput (i) );
    //   std::ostringstream os;
    //   os << "test-" <<i<<".mha";
    //   imagewriter->SetFileName (os.str().c_str());
    //   imagewriter->Update();
    // }
    // exit (0);
    
    std::cout << "Adding noise to dwis ..." << std::flush << std::endl;

    
    // Create the estimator
    EstimatorType::Pointer myEstimator = EstimatorType::New();
    
    // seed correctly according to IsReproducible switch
    itk::Statistics::NormalVariateGenerator::Pointer generator = itk::Statistics::NormalVariateGenerator::New();
    unsigned int seed = 0;
    if (IsReproducible)
      seed = 5323;
    else
      seed = (int) time(NULL);

    generator->Initialize (seed);

    GradientListType myNewGradients;
    
    for (unsigned int i=0; i<myFilter2->GetNumberOfOutputs(); i++)
    {
      // if (i != 0)
      myNewGradients.push_back (myGradients[i]);
      
      GaussianFilterType::Pointer myFilter;

      if (IsNoiseRician)
	myFilter = RicianFilterType::New();
      else
	myFilter = GaussianFilterType::New();

      myFilter->SetInput ( myFilter2->GetOutput (i) );
      myFilter->SetVariance (variance);
      myFilter->SetNormalGenerator(generator);
      
      try
      {
	myFilter->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return EXIT_FAILURE;
      }

      ImageType::Pointer image = myFilter->GetOutput();
      image->Update();
      
      myEstimator->SetInput(i,image);

      
      if (debugmode)
      {
      	ImageWriterType::Pointer tmpwriter = ImageWriterType::New();
	std::ostringstream os1, os2;
	os1 << "filter-input-"<<i<<".mha";
      	tmpwriter->SetInput (myFilter->GetInput ());
      	tmpwriter->SetFileName (os1.str().c_str());
      	tmpwriter->Update();
	os2 << "filter-output-"<<i<<".mha";
      	tmpwriter->SetInput (myFilter->GetOutput ());
      	tmpwriter->SetFileName (os2.str().c_str());
      	tmpwriter->Update();
      }
    }
    std::cout << "Done." << std::endl;

    
    // now: filter
    std::cout << "re-estimate tensors ..." << std::flush << std::endl;

    myEstimator->SetBST(0);
    myEstimator->SetGradientList (myNewGradients);


    
    RemoveNPTFilterType::Pointer myRemover = RemoveNPTFilterType::New();
    myRemover->SetInput (myEstimator->GetOutput());
    myRemover->SetRemovingType (1);
    
    // now: filter
    std::cout << "Estimating..." << std::flush;
    try
    {
      myEstimator->Update();
      myRemover->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return EXIT_FAILURE;
    }
    std::cout << "Done." << std::endl;
    
    // write the output
    IOType::Pointer myWriter = IOType::New();
    
    myWriter->SetFileName(fileOut);
    myWriter->SetInput (myRemover->GetOutput());
    
    std::cout << "Writing..." << std::flush;
    try
    {
      myWriter->Write();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return EXIT_FAILURE;
    }
    std::cout << "Done." << std::endl;
    
    return EXIT_SUCCESS;
    
  }
  
}
