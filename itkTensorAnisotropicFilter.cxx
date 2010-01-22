#include "itkAnisotropicDiffusionTensorImageFilter.h"
#include "itkLogTensorImageFilter.h"
#include "itkExpTensorImageFilter.h"
#include "itkTensorImageIO.h"

#include "GetPot.h"

void PrintHelp(const char* exec)
{

  std::cout << exec << ": " << std::endl;
  std::cout << "-i    [fileIn]" << std::endl;
  std::cout << "-o    [fileOut]" << std::endl;
  std::cout << "-k    [kappa]" << std::endl;
  std::cout << "-l    [lambda]" << std::endl;
  std::cout << "-dt   [DeltatT]" << std::endl;
  std::cout << "-nite [NumberOfIterations]" << std::endl;
  std::cout << "-rms  [RMS change: value below = convergence]" << std::endl;
  exit(0);

}


int main (int argc, char *argv[])
{
  
  
  GetPot   cl(argc, argv); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") ) 
    PrintHelp(cl[0]);
  
  const bool IsInputPresent = cl.search(2,"-i","-I");
  const bool IsOutputPresent = cl.search(2,"-o","-O");
  
  if(!IsInputPresent || !IsOutputPresent)
    {
      std::cerr << "Input file and/or output not set" << std::endl;
      return -1;
    }

  // parse the arguments
  const double kappa = cl.follow(1.0, 2, "-k", "-K");
  const double lambda = cl.follow(1.0,2,"-l","-L");
  const double deltat =  cl.follow(1.0, 2, "-dt", "-DT");
  const int nite = cl.follow(10,2,"-nite","-NITE");
  const char* file_in = cl.follow("NoFile", 2, "-i","-I");
  const char* file_out = cl.follow("NoFile", 2, "-o","-O");
  const double rms = cl.follow (0.0,2,"-rms","-RMS");

  
  if(strcmp(file_in,"NoFile")==0 || strcmp(file_out,"NoFile")==0)
  {
    std::cerr << "Input file and/or output not set" << std::endl;
    return -1;
  }
  
  std::cout << "Processing anisotropic filtering of file: " << file_in << " with: " << std::endl;
  std::cout << "Kappa= " << kappa << std::endl;
  std::cout << "Lambda = " << lambda << std::endl;
  std::cout << "delta t= " << deltat <<std::endl;
  std::cout << "Nite= " << nite << std::endl;  
  std::cout << "Output: " << file_out << std::endl;

  // Read in a tensor field:
  typedef double                               ScalarType;  
  typedef itk::TensorImageIO<ScalarType, 3, 3>  IOType;
  typedef IOType::TensorImageType              TensorImageType;
  
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
    
  
  // log
  typedef itk::LogTensorImageFilter<TensorImageType,TensorImageType> LogFilterType;
  LogFilterType::Pointer myLog = LogFilterType::New();

  //TensorImageType::Pointer tens = io->GetOutput();  
  myLog->SetInput(io->GetOutput());


  // Anisotropic filtering:
  typedef itk::AnisotropicDiffusionTensorImageFilter<TensorImageType,TensorImageType>
    FilterType;
  FilterType::Pointer myFilter = FilterType::New();
  
  
  myFilter->SetNumberOfIterations(nite);
  myFilter->SetKappa(kappa);
  myFilter->SetLambda(lambda);
  myFilter->SetTimeStep(deltat);
  myFilter->SetRMSChange (rms);
  

  myFilter->SetInput (myLog->GetOutput() );

  
  // exp
  typedef itk::ExpTensorImageFilter<TensorImageType,TensorImageType> ExpFilterType;
  ExpFilterType::Pointer myExp = ExpFilterType::New();
  
  myExp->SetInput(myFilter->GetOutput());

  
  std::cout << "Pipeline started." << std::endl;
  std::cout << std::flush;  
  try
  {
    myExp->Update();
  }
  catch(itk::ExceptionObject&e)
  {     
    std::cerr << e;
    exit(-1);
  }
  std::cout << "Pipeline finished." << std::endl;
    

  // Write
  io->SetFileName(file_out);
  io->SetInput(myExp->GetOutput());

  std::cout << "Writing file: " << file_out;
  std::cout << std::flush;
  try
  {
    io->Write();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e;
    exit(-1);
  }
  std::cout << " Done." << std::endl;
  
  
  return 0;

}
