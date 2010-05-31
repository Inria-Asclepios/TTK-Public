#include <iostream>

#include "GetPot.h"

#include "itkTensorImageIO.h"
#include "itkAverageStdTensorImageFilter.h"

void PrintHelp(const char* exec)
{

  std::cout << "Usage: " << std::endl;
  std::cout << exec << "-i <file list of tensor fields> -o <output file (without extension)>" << std::endl;
  std::cout << "It produces 2 files: output_file_mean.inr.gz and output_file_covariance_matrix.inr.gz" << std::endl;

  exit (0);
}


int main (int narg, char* arg[])
{

	itk::Object::GlobalWarningDisplayOff();

  GetPot   cl(narg, arg); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    PrintHelp(cl[0]);
	
  const bool IsInputPresent    = cl.search(2,"-i","-I");
  const bool IsOutputPresent   = cl.search(2,"-o","-O");
  
  if(!IsInputPresent || !IsOutputPresent)
    {
      std::cerr << "Error: Input and (or) output not set." << std::endl;
      exit (-1);
    }

  const char* fileIn = cl.follow("NoFile",2,"-i","-I");
  const char* fileOut = cl.follow("NoFile",2,"-o","-O");
  
  
  typedef double ScalarType;
  
  typedef itk::TensorImageIO<ScalarType, 3, 3> TensorIOType;
  typedef TensorIOType::TensorImageType        TensorImageType;
  typedef itk::TensorImageIO<ScalarType, 6, 3> CovMatrixIOType;
  typedef CovMatrixIOType::TensorImageType     CovMatrixImageType;
  
  typedef itk::AverageStdTensorImageFilter<TensorImageType, TensorImageType> AverageFilterType;
  
  TensorImageType::Pointer mean = 0;
  CovMatrixImageType::Pointer covMatrix = 0;
  
  {
  AverageFilterType::Pointer myAverage = AverageFilterType::New();
  
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
      TensorIOType::Pointer myReader = TensorIOType::New();
      myReader->SetFileName(filename);
      std::cout << "Reading " << filename << std::flush;
      try
	{
	  myReader->Read();
	}
      catch(itk::ExceptionObject &e)
	{
	  std::cerr << e;
	  exit(-1);
	}
      std::cout << " Done." << std::endl;
      myAverage->SetInput(i, myReader->GetOutput());
    }
	
	
	std::cout << "Averaging..." << std::flush;
	try
	{
	  myAverage->Update();
	}
	catch(itk::ExceptionObject &e)
	{
	  std::cerr << e;
	  exit(-1);
	}
	std::cout << "Done." << std::endl;
	
	mean = myAverage->GetOutput();
	covMatrix = myAverage->GetCovarianceMatrixImage();
	
	}
	
	std::cout << "Writing resuts..." << std::flush;
		std::string s_output = fileOut;
	{
	TensorIOType::Pointer writer1 = TensorIOType::New();
	writer1->SetInput( mean );
	std::string mean_output = s_output + "_mean.inr.gz";
	writer1->SetFileName( mean_output.c_str() );
	try
	{
	  writer1->Write();
	}
	catch(itk::ExceptionObject &e)
	{
		std::cerr << e;
		exit(-1);
	}
	}
	
	
	{
	CovMatrixIOType::Pointer writer2 = CovMatrixIOType::New();
	writer2->SetInput( covMatrix );
	std::string std_output = s_output + "_covariance_matrix.inr.gz";
	writer2->SetFileName( std_output.c_str() );
	try
	{
	  writer2->Write();
	}
	catch( itk::ExceptionObject &e)
	{
	  std::cerr << e;
	  exit(-1);
	}
	}
  std::cout << "Done." << std::endl;
  
  return 0;
}
