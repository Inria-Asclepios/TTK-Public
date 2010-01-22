#include <iostream>

#include "GetPot.h"

#include "itkTensorImageIO.h"
#include "itkLogTensorImageFilter.h"
#include "itkExpTensorImageFilter.h"
#include <itkImage.h>
#include <itkImageFileWriter.h>
#include <itkDiscreteGaussianImageFilter.h>
#include <itkImageRegionIteratorWithIndex.h>


void PrintHelp (const char* exec)
{
  std::cout << "Usage: " << std::endl;
  std::cout << exec << " <-i input> <-s sigma> <-euc> <-o output>" << std::endl;
  exit (0);
}


int main (int narg, char* arg[])
{

  // parse arguments
  GetPot cl (narg, arg);
  if( cl.size() == 1 || cl.search (2,"--help","-h") )
  {
    PrintHelp (cl[0]);
  }

  const char* file_in = cl.follow("NoFile", 2, "-i","-I");
  const char* file_out = cl.follow("NoFile", 2, "-o","-O");
  const double sigma = cl.follow (1.0, 2, "-s", "-S");
  const bool euclidean = cl.search (1,"-euc");

  typedef double                               ScalarType;
  typedef itk::TensorImageIO<ScalarType, 3, 3> TensorIOType;
  typedef TensorIOType::TensorType             TensorType;
  typedef TensorIOType::TensorImageType        TensorImageType;


  
  std::cout << "Reading: " << file_in;
  std::cout << std::flush;
  
  // read in the tensor field
  TensorIOType::Pointer myIO = TensorIOType::New();
  myIO->SetFileName (file_in);
  try
  {
    myIO->Read();
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }

  std::cout << " Done." << std::endl;

  TensorImageType::Pointer myTensorImage = myIO->GetOutput();


  if( !euclidean )
  {
    
    std::cout << "Loging..." << std::flush;
    
    // log the tensor field
    typedef itk::LogTensorImageFilter<TensorImageType, TensorImageType> LogFilterType;
    LogFilterType::Pointer myLoger = LogFilterType::New();
    myLoger->SetInput (myTensorImage);
    try
    {
      myLoger->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    std::cout << "Done." << std::endl;
  
    myTensorImage = myLoger->GetOutput();
  }



  std::cout << "Smoothing..." << std::flush;
  typedef itk::DiscreteGaussianImageFilter<TensorImageType, TensorImageType>
    GaussianFilterType;
  GaussianFilterType::Pointer gaussianFilter = GaussianFilterType::New();
  gaussianFilter->SetInput ( myTensorImage );
  double variance[3];
  for( int i=0; i<3; i++)
  {
    variance[i] = sigma;
  }
  gaussianFilter->SetVariance ( sigma );
  gaussianFilter->SetUseImageSpacingOn ();

  std::cout << gaussianFilter << std::endl;
  
  try
  {
    gaussianFilter->Update();
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }
  std::cout << "Done." << std::endl;

  myTensorImage = gaussianFilter->GetOutput();

  /*
  typedef itk::ImageRegionIteratorWithIndex<TensorImageType> IteratorType;
  IteratorType it (myTensorImage, myTensorImage->GetLargestPossibleRegion());
  while( !it.IsAtEnd() )
  {

    if( !it.Get().IsZero() )
    {
      std::cout << it.Get() << std::endl;
    }
    
    
    ++it;
  }
  */
  

  if( !euclidean )
  {

    std::cout << "Exping..." << std::flush;
    
    // log the tensor field
    typedef itk::ExpTensorImageFilter<TensorImageType, TensorImageType> ExpFilterType;
    ExpFilterType::Pointer myExper = ExpFilterType::New();
    myExper->SetInput ( myTensorImage );
    try
    {
      myExper->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    std::cout << "Done." << std::endl;
  
    myTensorImage = myExper->GetOutput();
    
  }




  // write the tensor field
  TensorIOType::Pointer myWriter = TensorIOType::New();
  myWriter->SetFileName (file_out);
  myWriter->SetInput ( myTensorImage );
  try
  {
    myWriter->Write();
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }

  std::cout << " Done." << std::endl;
  
  

  return 0;

}
