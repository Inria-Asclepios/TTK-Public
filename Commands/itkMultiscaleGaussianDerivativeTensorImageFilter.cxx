#include <iostream>

#include "GetPot.h"

#include "itkTensorImageIO.h"
#include "itkLogTensorImageFilter.h"
#include "itkExpTensorImageFilter.h"
#include <itkImage.h>
#include <itkImageFileWriter.h>
#include "itkGaussianDerivativeTensorImageFilter.h"
#include <itkImageRegionIteratorWithIndex.h>

void PrintHelp (const char* exec)
{
  std::cout << "Usage: " << std::endl;
  std::cout << exec << " <-i input> <-s sigma> <-d direction> <-o output>" << std::endl;
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
  const int direction = cl.follow (0,2,"-d","-D");

  typedef double                               ScalarType;
  typedef itk::TensorImageIO<ScalarType, 3, 3> TensorIOType;
  typedef TensorIOType::TensorType             TensorType;
  typedef TensorIOType::TensorImageType        TensorImageType;


  
  std::cout << "Reading: " << file_in;
  std::cout << std::flush;

  TensorImageType::Pointer myTensorImage = 0;
    
  // read in the tensor field
  {
    
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
    
    myTensorImage = myIO->GetOutput();
  }


  std::cout << "Loging..." << std::flush;
  
  // log the tensor field
  typedef itk::LogTensorImageFilter<TensorImageType, TensorImageType> LogFilterType;
  
  {
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
  


  TensorImageType::Pointer myTensorImageX = 0;
  TensorImageType::Pointer myTensorImageY = 0;
  TensorImageType::Pointer myTensorImageZ = 0;

  typedef itk::GaussianDerivativeTensorImageFilter<TensorImageType, TensorImageType>
    GaussianFilterType;
  
  {
    std::cout << "Smoothing X..." << std::flush;
    GaussianFilterType::Pointer gaussianFilterX = GaussianFilterType::New();
    gaussianFilterX->SetInput ( myTensorImage );
    gaussianFilterX->SetSigma (sigma);
    gaussianFilterX->SetDirection (0);
    
    try
    {
      gaussianFilterX->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;

    myTensorImageX = gaussianFilterX->GetOutput();
    
  }


  {
    std::cout << "Smoothing Y..." << std::flush;
    GaussianFilterType::Pointer gaussianFilterY = GaussianFilterType::New();
    gaussianFilterY->SetInput ( myTensorImage );
    gaussianFilterY->SetSigma (sigma);
    gaussianFilterY->SetDirection (1);
    
    try
    {
      gaussianFilterY->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;

    myTensorImageY = gaussianFilterY->GetOutput();
  }
  

  {
    std::cout << "Smoothing Z..." << std::flush;
    GaussianFilterType::Pointer gaussianFilterZ = GaussianFilterType::New();
    gaussianFilterZ->SetInput ( myTensorImage );
    gaussianFilterZ->SetSigma (sigma);
    gaussianFilterZ->SetDirection (2);
    
    try
    {
      gaussianFilterZ->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;
    
    myTensorImageZ = gaussianFilterZ->GetOutput();
  }
  



  // compute the gradient norm
  
  typedef itk::ImageRegionIteratorWithIndex<TensorImageType> TensorImageIteratorType;
  TensorImageIteratorType itX (myTensorImageX, myTensorImageX->GetLargestPossibleRegion());
  TensorImageIteratorType itY (myTensorImageY, myTensorImageY->GetLargestPossibleRegion());
  TensorImageIteratorType itZ (myTensorImageZ, myTensorImageZ->GetLargestPossibleRegion());

  typedef itk::Image<double, 3> ImageType;

  ImageType::Pointer myGradientImage = ImageType::New();
  myGradientImage->SetRegions (myTensorImageX->GetLargestPossibleRegion());
  myGradientImage->SetOrigin  (myTensorImageX->GetOrigin());
  myGradientImage->SetSpacing (myTensorImageX->GetSpacing());
  myGradientImage->Allocate();

  typedef itk::ImageRegionIteratorWithIndex<ImageType> ImageIteratorType;
  ImageIteratorType itGrad(myGradientImage, myGradientImage->GetLargestPossibleRegion());

  while( !itGrad.IsAtEnd() )
  {
    double val = sqrt ( itX.Get().GetSquaredNorm() + itY.Get().GetSquaredNorm() + itZ.Get().GetSquaredNorm() );

    itGrad.Set (val);
    
    ++itGrad;
    ++itX;
    ++itY;
    ++itZ;
  }



  itk::ImageFileWriter<ImageType>::Pointer writer = itk::ImageFileWriter<ImageType>::New();
  writer->SetFileName (file_out);
  writer->SetInput (myGradientImage);
  writer->Update();
  

  /*
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
  */
  

  return 0;

}
