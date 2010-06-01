#include "itkLogEuclideanDTIEstimatorCommand.h"

#include "itkLogEuclideanDTIEstimatorTensorImageFilter.h"
#include "itkExpTensorImageFilter.h"
#include "itkLogTensorImageFilter.h"
#include "itkDTIEstimatorTensorImageFilter.h"
#include "itkRemoveNonPositiveTensorsTensorImageFilter.h"
#include "itkTensorImageIO.h"
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImage.h>

#include "GetPot.h"
#include <ctime>

namespace itk
{

  LogEuclideanDTIEstimatorCommand::LogEuclideanDTIEstimatorCommand()
  {
    m_ShortDescription = "Estimate tensors using a Log-Gaussian / Gaussian / Rician noise model and Log-Euclidean metrics";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "-i     [list of file]\n";
    m_LongDescription += "-g     [list of gradients]\n";
    m_LongDescription += "-mode  [Mode. 0: ML log-Gaussian, 1: ML Gaussian, 2: ML Rician. Default: 1.]\n";
    m_LongDescription += "-var   [Variances for estimation mode 2]\n";
    m_LongDescription += "-me    [M-Estimator: 0: Least Square, 1: Geman-McLure, 2: Huber]\n";
    m_LongDescription += "-init  [0/1/2] 0: identity, 1: classic, 2: file provided\n";
    m_LongDescription += "-ifile [Filename]\n";
    m_LongDescription += "-bst   [Background Suppression Threshold]\n";
    m_LongDescription += "-c     [Geman McLure estimator threshold]\n";
    m_LongDescription += "-l     [Lambda]\n";
    m_LongDescription += "-k     [Kappa]\n";
    m_LongDescription += "-rms   [RMS]\n";
    m_LongDescription += "-dt    [delta t]\n";
    m_LongDescription += "-dtb0  [delta t for the b0]\n";
    m_LongDescription += "-nite  [Number of Iterations]\n";
    m_LongDescription += "-t     [Number of threads]\n";
    m_LongDescription += "-o     [output file]\n\n";
    m_LongDescription += m_ShortDescription;
  }
  
  LogEuclideanDTIEstimatorCommand::~LogEuclideanDTIEstimatorCommand()
  {}


  int LogEuclideanDTIEstimatorCommand::Execute(int narg, const char *arg[])
  {

    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
  
    const bool IsInputPresent = cl.search(2,"-i","-I");
    const bool IsOutputPresent = cl.search(2,"-o","-O");
    const bool IsGradientPresent = cl.search(2,"-g","-G");
    
    if(!IsInputPresent || !IsOutputPresent || !IsGradientPresent)
    {
      std::cerr << "Error: Input and (or) output and (or) gradients not set." << std::endl;
      return -1;
    }


    const char* fileIn = cl.follow("NoFile",2,"-i","-I");
    const char* fileOut = cl.follow("NoFile",2,"-o","-O");
    const char* fileGrad = cl.follow("NoFile",2,"-g","-G");
    const double bst = cl.follow(0.0,2,"-bst","-BST");
    const double lambda = cl.follow(0.0,2,"-l","-L");
    const double kappa = cl.follow(1.0,2,"-k","-K");
    const double rms = cl.follow(1.0,2,"-rms","-RMS");
    const double dt = cl.follow(1.0,2,"-dt","-DT");
    const double dtb0 = cl.follow(1.0,2,"-dtb0","-DTB0");
    const double c = cl.follow (1.0, 2, "-c","-C");  
    const int nite = cl.follow(100,2,"-nite","-NITE");
    const int init = cl.follow(0,2,"-init","-INIT");
    const int mode = cl.follow(1,2,"-mode","-MODE");
    const char* var_file = cl.follow("NoFile",2,"-var","-VAR");  
    const double me = cl.follow(0, 2,"-me","-ME");  
    const double numthreads = cl.follow (1,2,"-t","-T");
    const char* ifile = cl.follow ("NoFile",2,"-ifile","-IFILE");
        

    typedef double                                          ScalarType;  
    typedef itk::Image<ScalarType,3>                        ImageType;
    typedef ImageType::RegionType                           RegionType;
    typedef ImageType::SizeType                             SizeType;
    typedef itk::TensorImageIO<ScalarType, 3, 3>            IOType;
    typedef IOType::TensorImageType                         TensorImageType;    
    typedef TensorImageType::PixelType                      TensorType;
    typedef itk::ImageFileReader<ImageType>                 ReaderType;
    typedef itk::LogEuclideanDTIEstimatorTensorImageFilter<TensorImageType, TensorImageType>
      EstimatorType;
    typedef EstimatorType::GradientType                     GradientType;
    typedef EstimatorType::VarianceType                     VarianceType;
    
    typedef itk::DTIEstimatorTensorImageFilter<ImageType, TensorImageType> ClassicEstimatorType;    
    typedef itk::RemoveNonPositiveTensorsTensorImageFilter<TensorImageType,TensorImageType> PositiveFilterType;
    typedef itk::LogTensorImageFilter<TensorImageType,TensorImageType>     LogFilterType;
    
    // Create the filters
    EstimatorType::Pointer myEstimator = EstimatorType::New();
    

    std::cout << "########################################################" << std::endl;
    std::cout << "# Estimating Tensor Field with the following parameters:" << std::endl;
    std::cout << "# Mode   = " << mode << std::endl;
    std::cout << "# bst    = " << bst << std::endl;
    std::cout << "# me     = " << me << std::endl;
    std::cout << "# c      = " << c << std::endl;  
    std::cout << "# lambda = " << lambda << std::endl;
    std::cout << "# kappa  = " << kappa << std::endl;
    std::cout << "# rms    = " << rms << std::endl;
    std::cout << "# dt     = " << dt  << std::endl;
    std::cout << "# dtb0   = " << dt  << std::endl;
    std::cout << "# nite   = " << nite << std::endl;
    std::cout << "########################################################" << std::endl;
    std::cout << std::flush;
    



    // read the fileIn
    std::ifstream file(fileIn);
    if (file.fail() )
    {
      std::cerr << "Error: cannot open file: " << fileIn << std::endl;
      return -1;
    }
  
  
    // first number must be the num. of images:
    int numOfImages=0;
    file >> numOfImages;
    std::cout << "Number of Images: " << numOfImages << std::endl;
    
    for(int i=0;i<numOfImages;i++)
    {
      char filename[256];
      file >> filename;
      ReaderType::Pointer myReader = ReaderType::New();
      myReader->SetFileName(filename);
      std::cout << "Reading " << filename << std::flush;
      try
      {
	myReader->Update();
      }
      catch(itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << " Done." << std::endl;
      myEstimator->AddDTI( myReader->GetOutput() );
    }

    
    // read the fileGrad
    std::ifstream fileg(fileGrad,std::ifstream::in);
    if (fileg.fail() )
    {
      std::cerr << "Error: cannot open file: " << fileGrad << std::endl;
      return -1;
    }
    // first: must be the number of gradients
    int numOfGrad=0;
    fileg >> numOfGrad;
    
    for(int i=0;i<numOfGrad;i++)
    {
      GradientType g;
      fileg >> g[0];
      fileg >> g[1];
      fileg >> g[2];
      if( g!=0.0 )
      {
	g.Normalize();
      }
      
      std::cout << g << std::endl;
      myEstimator->AddGradient(g);
    }
    
    // read in variances if it is requested:
    if( strcmp ( var_file, "NoFile") != 0 )
    {
      VarianceType var;    
      std::ifstream filev(var_file);
      if (filev.fail() )
      {
	std::cerr << "Error: cannot open file: " << var_file << std::endl;
	return -1;
      }
      for ( int i=0;i<numOfGrad+1;i++)
      {
	ScalarType v;
	filev >> v;
	std::cout << v << std::endl;
	
	var.push_back (v);
      }
      
      myEstimator->SetVariances (var);
      
    }
    
    
    // options
    myEstimator->SetEstimationMode (mode);
    myEstimator->SetMEstimator (int(me));
    myEstimator->SetBST(bst);
    myEstimator->SetC (c);  
    myEstimator->SetLambda(lambda);
    myEstimator->SetKappa(kappa);
    myEstimator->SetRMSChange(rms);
    myEstimator->SetTimeStep(dt);
    myEstimator->SetB0TimeStep(dtb0);
    myEstimator->SetNumberOfIterations(nite);
    myEstimator->SetNumberOfThreads (int(numthreads));
    
    
    TensorImageType::Pointer input = 0;
    SizeType size;
    RegionType region;
    TensorType zero;
    ClassicEstimatorType::Pointer myClassicEstimator;
    PositiveFilterType::Pointer myPositive;
    LogFilterType::Pointer myLog;
    IOType::Pointer myReader;  
    
    
    switch( init )
    {
      
	case 0:
	  
	  // allocate an image of null matrices
	  std::cout << "Allocating..." << std::flush;
	  input = TensorImageType::New();
	  size = (myEstimator->GetDTI()[0])->GetLargestPossibleRegion().GetSize();
	  region = (myEstimator->GetDTI()[0])->GetLargestPossibleRegion();
	  input->SetLargestPossibleRegion(region);
	  input->SetBufferedRegion(region);
	  input->SetRequestedRegion(region);
	  input->SetSpacing(myEstimator->GetDTI()[0]->GetSpacing());
	  input->Allocate();
	  
	  zero = TensorType(static_cast<ScalarType>( 0.0 ));
	  input->FillBuffer(zero);
	  std::cout << "Done." << std::endl;    
	  
	  break;
	  
	  
	case 1:
	  
	  // classic estimation + remove negative tensors + log        
	  myClassicEstimator = ClassicEstimatorType::New();
	  
	  for(int i=0; i<numOfImages; i++)
	    myClassicEstimator->SetInput (i, myEstimator->GetDTI()[i]);
	  myClassicEstimator->SetGradientList (myEstimator->GetGradientList());
	  myClassicEstimator->SetBST(bst);  
	  
	  myPositive = PositiveFilterType::New();
	  myPositive->SetInput( myClassicEstimator->GetOutput() );
	  myPositive->SetRemovingType (1);
	  
	  myLog = LogFilterType::New();    
	  myLog->SetInput ( myPositive->GetOutput() );
	  
	  
	  std::cout << "Classic estimation pipeline started." << std::flush;
	  try
	  {
	    myLog->Update();
	  }
	  catch(itk::ExceptionObject &e)
	  {
	    std::cerr << e;
	    return -1;
	  }
	  std::cout << "Classic estimation pipeline finished." << std::endl;
	  input = myLog->GetOutput();
	  
	  break;
	  
	case 2:
	  
	  // initialize with a previously saved tf
	  if( strcmp (ifile,"NoFile")==0 )
	  {
	    std::cerr << "Please specify a init file." << std::endl;
	    return -1;          
	  }
	  
	  myReader = IOType::New();
	  myReader->SetFileName (ifile);
	  
	  std::cout << "Reading: " << ifile << std::flush;
	  std::cout << std::endl;        
	  try
	  {
	    myReader->Read();
	  }
	  catch (itk::ExceptionObject &e)
	  {
	    std::cerr << e;
	    return -1;
	  }
	  std::cout << "Done." << std::endl;
	  

	  myLog = LogFilterType::New();    
	  myLog->SetInput ( myReader->GetOutput() );
	  input = myLog->GetOutput();
	  
	  
	  break;


	default:
	  std::cerr << "Init mode not recognized."  << std::endl;
	  return -1;
	  break;
	  
    }
    
    
    // estimation + exponential
    typedef itk::ExpTensorImageFilter<TensorImageType, TensorImageType>  ExpFilterType;
    ExpFilterType::Pointer myExp = ExpFilterType::New();
    
    myExp->SetInput ( myEstimator->GetOutput() );
    myEstimator->SetInput( input );

    clock_t start, finish;
    std::cout << "Log-euclidean estimation pipeline started." << std::flush;
    start = clock();
    try
    {
      myExp->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    finish = clock();
    std::cout << "Log-euclidean estimation pipeline finished." << std::endl;

    std::cout << "Time elapsed: " << (double(finish)-double(start))/CLOCKS_PER_SEC << std::endl;
    
    // write the output
    IOType::Pointer myWriter = IOType::New();
    myWriter->SetFileName(fileOut);
    myWriter->SetInput( myExp->GetOutput() );
  
    std::cout << "Writing: " << fileOut << std::flush;
    try
    {
      myWriter->Write();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;


    if( mode == 2 )
    {
      
      //write B0
      
      typedef itk::ImageFileWriter<ImageType> WriterType;
      WriterType::Pointer writer = WriterType::New();
      writer->SetFileName("B0.hdr");
      writer->SetInput (myEstimator->GetB0());
      std::cout << "Writing the B0..." << std::flush;
      try
      {
        writer->Update();    
      }
      catch(itk::ExceptionObject &e)
      {
        std::cerr << e;
        return -1;
        
      }
      std::cout << "Done." << std::endl;
      
    }
    
    return 0;
    
  }
  
}
