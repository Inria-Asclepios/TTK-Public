#include "itkAddGaussianNoiseImageFilter.h"

#include "itkTensorImageIO.h"

#include "GetPot.h"
#include "itkTensorToScalarTensorImageFilter.h"
#include "itkTensorToL2NormFunction.h"
#include "itkTensorsToDWITensorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkAnalyzeImageIO.h"
#include "itkDTIEstimatorTensorImageFilter.h"
#include "itkRemoveNonPositiveTensorsTensorImageFilter.h"

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImage.h>


void PrintHelp(const char* exec)
{

  std::cout << exec << ": " << std::endl;
  std::cout << "-i [tensor field]" << std::endl;
  std::cout << "-v [variance]" << std::endl;
  std::cout << "-o [Output file]" << std::endl;
  std::cout << "-g [gradients]" << std::endl;
  
  exit(0);

}


int main(int narg, char *arg[])
{

  GetPot   cl(narg, arg); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    PrintHelp(cl[0]);
  
  const bool IsInputPresent    = cl.search(2,"-i","-I");
  const bool IsOutputPresent   = cl.search(2,"-o","-O");
  
  if(!IsInputPresent || !IsOutputPresent )
    {
      std::cerr << "Error: Input and (or) output not set." << std::endl;
      exit (-1);
    }


  const char* fileIn   = cl.follow("NoFile",2,"-i","-I");
  const char* fileOut  = cl.follow("NoFile",2,"-o","-O");
  const char* gradients_file = cl.follow ("NoFile",2,"-g","-G");
  const double variance = cl.follow(1.0, 2,"-v","-V");
  
  typedef double ScalarType;  
  typedef itk::Image<ScalarType, 3>                      ImageType;
  typedef itk::TensorImageIO<ScalarType, 3, 3>           IOType;
  typedef IOType::TensorImageType                        TensorImageType;
  typedef TensorImageType::PixelType                     TensorType;
  
  typedef itk::Image<double, 3>                ImageType;
//   typedef itk::Image<unsigned short, 3>        ImageType;

  typedef itk::AddGaussianNoiseImageFilter<ImageType,ImageType>
    FilterType;
  typedef itk::TensorToScalarTensorImageFilter<TensorImageType, ImageType>
    TensorToScalarFilterType;
  typedef itk::TensorToL2NormFunction<TensorType, ScalarType>
    TensorFunctionType;
  typedef itk::DTIEstimatorTensorImageFilter<ImageType, TensorImageType>
    EstimatorType;  
  typedef EstimatorType::GradientType GradientType;
  typedef itk::RemoveNonPositiveTensorsTensorImageFilter<TensorImageType,TensorImageType>
    RemoveNPTFilterType;
  typedef itk::ImageFileWriter<ImageType> ImageWriterType;
 

  IOType::Pointer myReader = IOType::New();
  myReader->SetFileName (fileIn);
  try
  {
    myReader->Read();
  } catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }

  
  TensorImageType::Pointer myTensorImage = myReader->GetOutput();

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
    return -1;
  }

//   ImageType::Pointer myB0tp = myFilter1->GetOutput();
  
//   itk::RescaleIntensityImageFilter<ImageType, ImageType>::Pointer rescaler=
//     itk::RescaleIntensityImageFilter<ImageType, ImageType>::New();
  
//   rescaler->SetOutputMinimum ( 0 );
//   rescaler->SetOutputMaximum ( 32767 );
//   rescaler->SetInput ( myB0tp );
//   try
//   {
//     rescaler->Update();
//   }
//   catch (itk::ExceptionObject &e)
//   {
//     std::cerr << e;
//     return -1;
//   }

  ImageType::Pointer myB0 = myFilter1->GetOutput();  

  typedef itk::TensorsToDWITensorImageFilter<TensorImageType, ImageType>
    TensorsToDWIFilter;
  typedef TensorsToDWIFilter::GradientType      GradientType;
  typedef TensorsToDWIFilter::GradientListType  GradientListType;

  GradientListType myGradients;
  
  // read the fileGrad
  std::ifstream fileg (gradients_file, std::ifstream::in);
  if( fileg.fail() )
  {
    std::cerr << "Error: Cannot open " << gradients_file << " for reading." << std::endl;
    return -1;
  }
  
  // first: must be the number of gradients
  int numOfGrad = 0;
  fileg >> numOfGrad;
  
  for(int i=0; i<numOfGrad; i++)
  {
    GradientType g;
    fileg >> g[0];
    fileg >> g[1];
    fileg >> g[2];
    std::cout << g << std::endl;
    if (g[0]!=0.0 || g[1]!=0.0 || g[2]!=0.0)
    {
      g.Normalize();
    }
    myGradients.push_back ( g );
  }
  
  GradientListType myGradients2 = myGradients;

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
    return -1;
  }


  std::vector<ImageType::Pointer> dwis;
  dwis.push_back (myB0);
  
  // now: filter
  std::cout << "Adding noise to dwis ..." << std::flush << std::endl;
  
  for (unsigned int i=0; i<myFilter2->GetNumberOfOutputs(); i++)
  {
    FilterType::Pointer myFilter = FilterType::New();
    myFilter->SetInput ( myFilter2->GetOutput (i) );
    
    myFilter->SetVariance (variance);
    myFilter->Update();
    dwis.push_back (myFilter->GetOutput());
  }
  std::cout << "Done." << std::endl;

  // now: filter
  std::cout << "re-estimate tensors ..." << std::flush << std::endl;

  
  
  // Create the filter
  EstimatorType::Pointer myEstimator = EstimatorType::New();
  myEstimator->SetBST(0);
  

  for (unsigned int i=0; i<dwis.size(); i++)
  {
    myEstimator->SetInput(i,dwis[i]);
  }

  std::cout<<"numOfGrad "<<numOfGrad<<std::endl;
  
  
  for(int i=0; i<numOfGrad; i++)
  {
    myEstimator->AddGradient(myGradients[i]);
  }

  
  RemoveNPTFilterType::Pointer myRemover = RemoveNPTFilterType::New();
  myRemover->SetInput (myEstimator->GetOutput());
  myRemover->SetRemovingType (0);
  
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
