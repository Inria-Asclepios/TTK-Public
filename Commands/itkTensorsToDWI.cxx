#include <iostream>

#include "GetPot.h"
#include "itkTensorImageIO.h"
#include "itkTensorToScalarTensorImageFilter.h"
#include "itkTensorToL2NormFunction.h"
#include "itkTensorsToDWITensorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkAnalyzeImageIO.h"

#include <itkImage.h>
#include <itkImageFileWriter.h>



void PrintHelp (const char* exec)
{
  std::cout << "Usage: " << std::endl;
  std::cout << exec << " <-i input> <-b bvalue> <-g gradients> <-o output> <-e extension>" << std::endl;
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


  const char* file_in = cl.follow ("NoFile",2,"-i","-I");
  const double bvalue = cl.follow (1.0, 2, "-b", "-B");
  const char* gradients_file = cl.follow ("NoFile",2,"-g","-G");
  const char* extension= cl.follow ("*.nrrd", 2, "-e", "-E");
  const char* file_out= cl.follow ("NoFile", 2, "-o", "-O");

  typedef double                               ScalarType;
  typedef itk::TensorImageIO<ScalarType, 3, 3> TensorIOType;
  typedef TensorIOType::TensorType             TensorType;
  typedef TensorIOType::TensorImageType        TensorImageType;


  typedef itk::Image<double, 3>                ImageType;
  typedef itk::Image<unsigned short, 3>        LightImageType;



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


  // create a B0 image by taking the norm of the tensor field * scale:
  typedef itk::TensorToScalarTensorImageFilter<TensorImageType, ImageType>
    TensorToScalarFilterType;

  typedef itk::TensorToL2NormFunction<TensorType, ScalarType>
    TensorFunctionType;
  

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

  ImageType::Pointer myB0 = myFilter1->GetOutput();

  
  itk::RescaleIntensityImageFilter<ImageType, LightImageType>::Pointer rescaler=
    itk::RescaleIntensityImageFilter<ImageType, LightImageType>::New();
  
  rescaler->SetOutputMinimum ( 0 );
  rescaler->SetOutputMaximum ( 32767 );
  rescaler->SetInput ( myB0 );
  try
  {
    rescaler->Update();
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }
  


  typedef itk::TensorsToDWITensorImageFilter<TensorImageType, LightImageType>
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
    if (g[0]!=0.0 && g[1]!=0.0 && g[2]!=0.0)
    {
      g.Normalize();
    }
    myGradients.push_back ( g );
  }


  TensorsToDWIFilter::Pointer myFilter2 = TensorsToDWIFilter::New();
  myFilter2->SetBaselineImage (rescaler->GetOutput());
  myFilter2->SetBValue (bvalue);
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
  


  // save the results
  char filename[512];
  sprintf (filename, "%s_%.2d.%s", file_out, 0, extension);
  std::cout << "Saving: " << filename;
  itk::ImageFileWriter<LightImageType>::Pointer writer0 = itk::ImageFileWriter<LightImageType>::New();
  writer0->SetFileName (filename);
  writer0->SetInput (rescaler->GetOutput());

  writer0->SetImageIO( itk::AnalyzeImageIO::New() );
  
  try
  {
    writer0->Write();
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }
  std::cout << " Done." << std::endl;
  
  unsigned int numOutputs = myFilter2->GetNumberOfOutputs();
  for( unsigned int i=0; i<numOutputs; i++)
  {
    sprintf (filename, "%s_%.2d.%s", file_out, i+1, extension);    
    std::cout << "Saving: " << filename;
    
    itk::ImageFileWriter<LightImageType>::Pointer writer = itk::ImageFileWriter<LightImageType>::New();
    writer->SetFileName (filename);
    writer->SetImageIO( itk::AnalyzeImageIO::New() );
    writer->SetInput (myFilter2->GetOutput (i));
    
    try
    {
      writer->Write();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
  }
  
  
  
}
