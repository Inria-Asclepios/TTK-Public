#include <iostream>

#include "GetPot.h"

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkAverageStdImageFilter.h"

void PrintHelp(const char* exec)
{

  std::cout << "Usage: " << std::endl;
  std::cout << exec << " -i <file list of images> -o <output file (without extension)>" << std::endl;
  std::cout << "It produces 2 files: output_file_mean.hdr and output_file_variance.hdr" << std::endl;

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
  
  typedef itk::Image<ScalarType, 3>   ImageType;
  
  typedef itk::AverageStdImageFilter<ImageType, ImageType> AverageFilterType;

  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageFileWriter<ImageType> WriterType;
  
  ImageType::Pointer mean = 0;
  ImageType::Pointer variance = 0;
  
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
	variance = myAverage->GetVarianceImage();
	
	}
	
	std::cout << "Writing resuts..." << std::flush;
		std::string s_output = fileOut;
	{
	WriterType::Pointer writer1 = WriterType::New();
	writer1->SetInput( mean );
	std::string mean_output = s_output + "_mean.hdr";
	writer1->SetFileName( mean_output.c_str() );
	try
	{
	  writer1->Update();
	}
	catch(itk::ExceptionObject &e)
	{
		std::cerr << e;
		exit(-1);
	}
	}
	
	
	{
	WriterType::Pointer writer2 = WriterType::New();
	writer2->SetInput( variance );
	std::string std_output = s_output + "_variance.hdr";
	writer2->SetFileName( std_output.c_str() );
	try
	{
	  writer2->Update();
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
