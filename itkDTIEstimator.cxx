#include "itkDTIEstimatorTensorImageFilter.h"
#include "itkRemoveNonPositiveTensorsTensorImageFilter.h"

#include "itkTensorImageIO.h"


#include "GetPot.h"

#include <itkImageFileReader.h>
#include <itkImage.h>

void PrintHelp(const char* exec)
{

  std::cout << exec << ": " << std::endl;
  std::cout << "-i [List of DTIs]" << std::endl;
  std::cout << "-g [List of gradients]" << std::endl;
  std::cout << "-b [Background Suppression Threshold]" << std::endl;
  std::cout << "-r [NPT removing type: 0: null tensor / 1: mean of neighbors]" << std::endl;
  std::cout << "-o [Output file]" << std::endl;
  exit(0);

}


int main(int narg, char *arg[])
{

  GetPot   cl(narg, arg); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    PrintHelp(cl[0]);
  
  const bool IsInputPresent    = cl.search(2,"-i","-I");
  const bool IsOutputPresent   = cl.search(2,"-o","-O");
  const bool IsGradientPresent = cl.search(2,"-g","-G");
  
  if(!IsInputPresent || !IsOutputPresent || !IsGradientPresent)
    {
      std::cerr << "Error: Input and (or) output and (or) gradients not set." << std::endl;
      exit (-1);
    }


  const char* fileIn = cl.follow("NoFile",2,"-i","-I");
  const char* fileOut = cl.follow("NoFile",2,"-o","-O");
  const char* fileGrad = cl.follow("NoFile",2,"-g","-G");
  const double bst = cl.follow(0.0,2,"-b","-B");
  const int RemovingType = cl.follow (0,2,"-r","-R");

  typedef double ScalarType;  
  typedef itk::Image<ScalarType, 3>                      ImageType;
  typedef itk::TensorImageIO<ScalarType, 3, 3>           IOType;
  typedef IOType::TensorImageType                        TensorImageType;    
  typedef itk::ImageFileReader<ImageType>                ReaderType;
  typedef itk::DTIEstimatorTensorImageFilter<ImageType, TensorImageType>
    EstimatorType;  
  typedef EstimatorType::GradientType GradientType;
  typedef itk::RemoveNonPositiveTensorsTensorImageFilter<TensorImageType,TensorImageType>
    RemoveNPTFilterType;
  

  
  // Create the filter
  EstimatorType::Pointer myEstimator = EstimatorType::New();
  myEstimator->SetBST(bst);
  
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
      myEstimator->SetInput(i,myReader->GetOutput());
      
    }

  
  // read the fileGrad
  std::ifstream fileg(fileGrad,std::ifstream::in);
  // first: must be the number of gradients
  int numOfGrad = 0;
  fileg >> numOfGrad;
  
  for(int i=0;i<numOfGrad;i++)
    {
      GradientType g;
      fileg >> g[0];
      fileg >> g[1];
      fileg >> g[2];
      std::cout << g << std::endl;
      myEstimator->AddGradient(g);
    }
  

  RemoveNPTFilterType::Pointer myRemover = RemoveNPTFilterType::New();
  myRemover->SetInput (myEstimator->GetOutput());
  myRemover->SetRemovingType (RemovingType);
  
  // now: filter
  std::cout << "Estimating..." << std::flush;
  try
    {
      myRemover->Update();
    }
  catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      exit(-1);
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
      exit(-1);
    }
  std::cout << "Done." << std::endl;
  
  return 0;

}
