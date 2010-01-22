#include "itkExpTensorImageFilter.h"
#include "itkLogTensorImageFilter.h"
#include "itkDTIEstimatorWithBFGSTensorImageFilter.h"
#include "itkTensorImageIO.h"
#include "GetPot.h"

#include <itkImageFileReader.h>
#include <itkImage.h>

void PrintHelp(const char* exec)
{

  std::cout << exec << ": " << std::endl;
  std::cout << "-i     [list of file]" << std::endl;
  std::cout << "-g     [list of gradients]" << std::endl;
  std::cout << "-mode  [Mode. 0: log of DWI, 1: DWI, 2: Square of DWI. Default: 1.]" << std::endl;
  std::cout << "-var   [Variances for estimation mode 2]" << std::endl;
  std::cout << "-me    [M-Estimator: 0: Least Square, 1: Huber]" << std::endl;
  std::cout << "-init  [file]" << std::endl;
  //std::cout << "-ifile [Filename]" << std::endl;  
  std::cout << "-bst   [Background Suppression Threshold]" << std::endl;
  std::cout << "-c     [Geman McLure estimator threshold]" << std::endl;  
  //std::cout << "-l     [Lambda]" << std::endl;
  //std::cout << "-k     [Kappa]" << std::endl;
  //std::cout << "-rms   [RMS]" << std::endl; 
  std::cout << "-gd    [gradient descent type: 0/1]" << std::endl;
  std::cout << "-dt    [delta t]" << std::endl;
  //std::cout << "-dtb0  [delta t for the b0]" << std::endl;
  //std::cout << "-nite  [Number of Iterations]" << std::endl;
  std::cout << "-t     [Number of threads]" << std::endl;  
  std::cout << "-o     [output file]" << std::endl;
  exit(0);

}


int main(int narg, char *arg[])
{

  GetPot   cl(narg, arg); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    PrintHelp(cl[0]);
  
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
  //const double lambda = cl.follow(0.0,2,"-l","-L");
  //const double kappa = cl.follow(1.0,2,"-k","-K");
  //const double rms = cl.follow(1.0,2,"-rms","-RMS");
  const double dt = cl.follow(1.0,2,"-dt","-DT");
  //const double dtb0 = cl.follow(1.0,2,"-dtb0","-DTB0");
  const double c = cl.follow (1.345, 2, "-c","-C");  
  //const int nite = cl.follow(100,2,"-nite","-NITE");
  //const int init = cl.follow(0,2,"-init","-INIT");
  const int mode = cl.follow(1,2,"-mode","-MODE");
  const int gd = cl.follow (0,2,"-gd","-GD");
  const char* var_file = cl.follow("NoFile",2,"-var","-VAR");  
  const int me = cl.follow(0, 2,"-me","-ME");  
  const double numthreads = cl.follow (1,2,"-t","-T");
  const char* ifile = cl.follow ("NoFile",2,"-init","-INIT");
        

  typedef double                                          ScalarType;  
  typedef itk::Image<ScalarType,3>                        ImageType;
  typedef ImageType::RegionType                           RegionType;
  typedef ImageType::SizeType                             SizeType;
  typedef itk::TensorImageIO<ScalarType, 3, 3>            IOType;
  typedef IOType::TensorImageType                         TensorImageType;    
  typedef TensorImageType::PixelType                      TensorType;
  typedef itk::ImageFileReader<ImageType>                 ReaderType;
  typedef itk::DTIEstimatorWithBFGSTensorImageFilter<ImageType, TensorImageType>
    EstimatorType;
  typedef EstimatorType::GradientType                     GradientType;
  typedef EstimatorType::VarianceType                     VarianceType;

  typedef itk::LogTensorImageFilter<TensorImageType,TensorImageType>     LogFilterType;
  typedef itk::ExpTensorImageFilter<TensorImageType, TensorImageType>    ExpFilterType;
  
  // Create the filters
  EstimatorType::Pointer myEstimator = EstimatorType::New();



  /*
  // wanna test the Bessel values:
  std::ofstream testBuff ("bessel.log");
  std::ofstream testBuff2 ("besselratio.log");
  for( double t=0.0; t<100.0; t+=0.1)
  {
    testBuff << t << " " << EstimatorType::Bessel (t) << std::endl;
    testBuff2 << t << " " << EstimatorType::BesselRatio (t) << std::endl;
  }
  testBuff.close();
  testBuff2.close();
  */
  
  

  std::cout << "########################################################" << std::endl;
  std::cout << "# Estimating Tensor Field with the following parameters:" << std::endl;
  std::cout << "# Mode   = " << mode << std::endl;
  std::cout << "# bst    = " << bst << std::endl;
  std::cout << "########################################################" << std::endl;
  std::cout << std::flush;



  // read the fileIn
  std::ifstream file(fileIn,std::ifstream::in);
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
      exit(-1);
    }
    std::cout << " Done." << std::endl;
    myEstimator->SetInput (i, myReader->GetOutput() );
  }

  
  // read the fileGrad
  std::ifstream fileg(fileGrad,std::ifstream::in);
  // first: must be the number of gradients
  int numOfGrad=0;
  fileg >> numOfGrad;
  
  for(int i=0;i<numOfGrad;i++)
    {
      GradientType g;
      fileg >> g[0];
      fileg >> g[1];
      fileg >> g[2];
      g.Normalize();
      std::cout << g << std::endl;
      myEstimator->AddGradient(g);
    }

  // read in variances if it is requested:
  if( strcmp ( var_file, "NoFile") != 0 )
  {
    VarianceType var;    
    std::ifstream filev(var_file,std::ifstream::in);
    for ( int i=0;i<numOfGrad+1;i++)
    {
      ScalarType v;
      filev >> v;
      std::cout << v << std::endl;
      
      var.push_back (v);
    }

    myEstimator->SetVariances (var);
    
  }



  
  // initialize with a previously saved tf
  if( strcmp (ifile,"NoFile") != 0 )
  {

    IOType::Pointer myReader = IOType::New();
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
    
    
    LogFilterType::Pointer myLog = LogFilterType::New();    
    myLog->SetInput ( myReader->GetOutput() );

    try
    {
      myLog->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    myEstimator->SetInitialization ( myLog->GetOutput() );
  }
  
  // options
  myEstimator->SetEstimationType (mode);
  myEstimator->SetBST(bst);
  myEstimator->SetTinit (dt);
  myEstimator->SetKappa (c);
  myEstimator->SetNumberOfThreads (int(numthreads));
  myEstimator->SetGradientDescentType (gd);
  myEstimator->SetMEstimatorType (me);
  
    

  // estimation + exponential  
  ExpFilterType::Pointer myExp = ExpFilterType::New();
  
  myExp->SetInput ( myEstimator->GetOutput() );

  std::cout << "Log-euclidean estimation pipeline started." << std::flush;    
  try
  {
    myExp->Update();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e;
    exit(-1);
  }
  std::cout << "Log-euclidean estimation pipeline finished." << std::endl;
  
  
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
    exit(-1);
  }
  std::cout << " Done." << std::endl;
  
  
  return 0;
      
}
