#include <iostream>

#include "GetPot.h"

#include "itkTensorImageIO.h"
#include "itkZScoreTensorImageFilter.h"
#include <itkImageFileWriter.h>
#include <itkAnalyzeImageIOFactory.h>

void PrintHelp(const char* exec)
{

  std::cout << "Usage: " << std::endl;
  std::cout << exec << "-i <input tensor field> -a <average tensor field> -c <field of covariance matrices> -o <zscore map>" << std::endl;
  exit (0);
}


int main (int narg, char* arg[])
{

	itk::Object::GlobalWarningDisplayOff();

  GetPot   cl(narg, arg); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    PrintHelp(cl[0]);
	

  const char* fileIn = cl.follow("NoFile",2,"-i","-I");
  const char* fileAverage = cl.follow("NoFile",2,"-a","-A");
  const char* fileCovMat = cl.follow("NoFile",2,"-c","-C");
  const char* fileOut = cl.follow("NoFile",2,"-o","-O");
  
  
  typedef double ScalarType;
  
  typedef itk::Image<ScalarType, 3> ImageType;
  typedef itk::TensorImageIO<ScalarType, 3, 3> TensorIOType;
  typedef TensorIOType::TensorImageType        TensorImageType;
  typedef itk::TensorImageIO<ScalarType, 6, 3> CovMatrixIOType;
  typedef CovMatrixIOType::TensorImageType     CovMatrixImageType;
  
  typedef itk::ZScoreTensorImageFilter<TensorImageType, ImageType> ZScoreFilterType;


  TensorImageType::Pointer tensors = 0;
  TensorImageType::Pointer average = 0;
  CovMatrixImageType::Pointer covMatrix = 0;
  ImageType::Pointer result = 0;

  {

	  std::cout << "Reading: " << fileIn << std::flush;
  TensorIOType::Pointer reader1 = TensorIOType::New();
  reader1->SetFileName(fileIn);
  try
  {
    reader1->Read();
  }
  catch(itk::ExceptionObject &e)
  {
	  std::cerr << e;
	  exit (-1);
  }
  std::cout << "Done." << std::endl;
  tensors = reader1->GetOutput();

  }


  {
	  std::cout << "Reading: " << fileAverage << std::flush;
	TensorIOType::Pointer reader = TensorIOType::New();
    reader->SetFileName(fileAverage);
	try
	{
		reader->Read();
	}
	catch(itk::ExceptionObject &e)
	{
		std::cerr << e;
		exit (-1);
	}
std::cout << "Done." << std::endl;
    average = reader->GetOutput();
  }
   

  {
	  std::cout << "Reading: " << fileCovMat << std::flush;
	CovMatrixIOType::Pointer reader = CovMatrixIOType::New();
    reader->SetFileName(fileCovMat);
	try
	{
		reader->Read();
	}
	catch(itk::ExceptionObject &e)
	{
		std::cerr << e;
		exit (-1);
	}
std::cout << "Done." << std::endl;
    covMatrix = reader->GetOutput();
  }



  {
  
	ZScoreFilterType::Pointer myZScore = ZScoreFilterType::New();
	myZScore->SetInput( tensors );
	myZScore->SetAverageTensorImage( average );
	myZScore->SetCovarianceMatrixImage( covMatrix );
	//myZScore->SetNumberOfThreads(1);

	
	std::cout << "ZScoring..." << std::flush;
	try
	{
	  myZScore->Update();
	}
	catch(itk::ExceptionObject &e)
	{
	  std::cerr << e;
	  exit(-1);
	}
	std::cout << "Done." << std::endl;
	
	result = myZScore->GetOutput();
  }
	

  std::cout << "Writing resut..." << std::flush;

  itk::AnalyzeImageIOFactory::RegisterOneFactory();

  typedef itk::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( fileOut );
  writer->SetInput( result );

  try
  {
	writer->Update();
  }
  catch(itk::ExceptionObject &e)
	{
	  std::cerr << e;
	  exit(-1);
	}
	std::cout << "Done." << std::endl;
 
  return 0;
}
