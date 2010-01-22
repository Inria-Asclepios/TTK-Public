#include "itkRBFInterpolationTensorImageFilter.h"
#include "itkExpTensorImageFilter.h"
#include "itkTensorImageIO.h"

#include <itkImageFileReader.h>
#include <itkPoint.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>

#include "GetPot.h"

void PrintHelp(const char* exec)
{

  std::cout << exec << ": " << std::endl;
  std::cout << "-i    [Input Scalar Image = domain of diffusion]" << std::endl;
  std::cout << "-tens [vtkUnstructuredGrid File]" << std::endl;
  std::cout << "-s    [Sigma]" << std::endl;
  std::cout << "-g    [Gamma]" << std::endl;
  std::cout << "-euc"          << std::endl;
  //std::cout << "-or [ox oy oz]" << std::endl;
  std::cout << "-tan  [Tangent File]" << std::endl;
  std::cout << "-t    [number of threads]" << std::endl;
  std::cout << "-o    [Output Tensor Image]" << std::endl;
  exit(0);
}


int main(int narg, char* arg[])
{
  
  GetPot   cl(narg, arg); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") ) 
    PrintHelp(cl[0]);
  

  const bool IsInputPresent    = cl.search(2,"-i","-I");
  const bool AreSourcesPresent = cl.search(1,"-tens");  
  const bool IsOutputPresent   = cl.search(2,"-o","-O");
  const bool IsTangentPresent  = cl.search(1,"-tan");
  const bool isEucSet          = cl.search(1,"-euc");
  
  if(!IsInputPresent || !AreSourcesPresent || !IsOutputPresent)
    {
      std::cerr << "Input and (or) mask and (or) output not set." << std::endl;
      exit (-1);
    }
  
  const char* filemask = cl.follow("NoFile", 2, "-i","-I");
  const char* output   = cl.follow("NoFile", 2, "-o","-O");
  const char* tensorsFile = cl.follow("NoFile",1,"-tens");
  const char* tangentFile = cl.follow("NoFile",1,"-tan");
  
  if( strcmp(tensorsFile,"NoFile")==0 || strcmp(output,"NoFile")==0
      || strcmp(filemask,"NoFile")==0)
  {
    std::cerr << "Input and (or) mask and (or) output not set." << std::endl;
    exit (-1);
  }

  /*const double ox = cl.follow(0.0,1,"-or");
    const double oy = cl.next(0.0);
    const double oz = cl.next(0.0);
  */
  const double sigma = cl.follow(1.0, 2, "-s","-S");
  const double gamma = cl.follow(1.0, 2, "-g","-G");
  const int t        = cl.follow (1, 2, "-t", "-T");
    
  std::cout << "Processing RBF interpolation of file: " << tensorsFile << ", with: " << std::endl;
  std::cout << "Sigma: " << sigma << std::endl;
  std::cout << "Gamma: " << gamma << std::endl;
  //std::cout << "Origin is: " << ox << " " << oy << " " << oz << std::endl;
  if( IsTangentPresent )
    std::cout << "Tangent: " << tangentFile << std::endl;
  std::cout << "Mask: " << filemask << std::endl;
  std::cout << "Output: " << output << std::endl;
  std::cout << std::flush;

  // typedefs
  typedef double ScalarType;
  typedef itk::TensorImageIO<ScalarType, 3, 3> IOType;
  typedef IOType::TensorImageType              TensorImageType;
  typedef itk::Image<ScalarType, 3>            ImageType;
  typedef TensorImageType::PixelType           TensorType;
  typedef itk::ExpTensorImageFilter<TensorImageType, TensorImageType> ExpFilterType;
  typedef itk::RBFInterpolationTensorImageFilter<ImageType, TensorImageType>
    FilterType;
  typedef FilterType::VectorOfPixelType        VectorOfTensorsType;
  typedef FilterType::PointType                PointType;
  typedef FilterType::VectorOfPointsType       VectorOfPointsType;
  typedef itk::ImageFileReader<ImageType>      ImageFileReaderType;






  // read the input image
  ImageFileReaderType::Pointer imReader = ImageFileReaderType::New();
  imReader->SetFileName(filemask);
  std::cout << "Reading: " << filemask << std::flush;
  try
  {
    imReader->Update();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e << std::endl;
    exit(-1);
  }
  std::cout << " Done." << std::endl;
  
  ImageType::Pointer input = imReader->GetOutput();

  
  // read the model
  std::cout << "Reading: " << tensorsFile;
  vtkUnstructuredGridReader* tensReader = vtkUnstructuredGridReader::New();
  tensReader->SetFileName(tensorsFile);
  vtkUnstructuredGrid* tensors = tensReader->GetOutput();
  tensors->Update();
  std::cout << " Done." << std::endl;
  
  std::cout << "Converting...";
  // convert the model to a vector of tensors + points
  VectorOfTensorsType vecT;
  VectorOfPointsType vecP;
  int numPoints = tensors->GetNumberOfPoints();
  
  for(int i=0;i<numPoints;i++)
    {
      double pt[3];
      tensors->GetPoint(i,pt);
      PointType p;
      for(int m=0;m<3;m++)
	p[m]=pt[m];
      
      double tensorCoefs[9];
      tensors->GetPointData()->GetTensors()->GetTuple(i,tensorCoefs);
      
      TensorType T;
      T.SetNthComponent ( 0, tensorCoefs[0] );
      T.SetNthComponent ( 1, tensorCoefs[3] );
      T.SetNthComponent ( 2, tensorCoefs[4] );
      T.SetNthComponent ( 3, tensorCoefs[6] );
      T.SetNthComponent ( 4, tensorCoefs[7] );
      T.SetNthComponent ( 5, tensorCoefs[8] );
      
      if( isEucSet )
      {
        vecT.push_back(T);
      }
      else
      {
        vecT.push_back(T.Log());
      }
      
      
      vecP.push_back(p);
    }
  tensReader->Delete();
  std::cout << "Done." << std::endl;
    
  VectorOfPointsType vecTangent;
  if(IsTangentPresent)
  {
    std::cout << "Reading tangent file...";
    vtkUnstructuredGridReader* tanReader = vtkUnstructuredGridReader::New();
    tanReader->SetFileName(tangentFile);
    vtkUnstructuredGrid* tangent = tanReader->GetOutput();
    tangent->Update();
    
    int nTan = tangent->GetNumberOfPoints();

    for(int i=0;i<nTan;i++)
    {
      double pt[3];
      tangent->GetPoint(i,pt);
      PointType p;
      for(int m=0;m<3;m++)
        p[m]=pt[m];
      vecTangent.push_back(p);
    }

    tangent->Delete();
    std::cout << "Done." << std::endl;
  }  
   
  FilterType::Pointer myFilter = FilterType::New();
  myFilter->SetInput(input);
  myFilter->SetTensors(vecT);
  myFilter->SetPoints(vecP);
  myFilter->SetTangents(vecTangent);
  myFilter->SetSigma(sigma);
  myFilter->SetGamma(gamma);
  myFilter->SetNumberOfThreads (t);

  try
  {
    myFilter->Update();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e << std::endl;
    exit(-1);
  }


  ExpFilterType::Pointer myExpFilter = 0;
  if( !isEucSet )
  {
    myExpFilter = ExpFilterType::New();
    myExpFilter->SetInput ( myFilter->GetOutput() );
    
    std::cout << "Filtering..." << std::flush;
    try
    {
      myExpFilter->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e << std::endl;
      exit(-1);
    }
    std::cout << "Done." << std::endl;
  }
  

  
  IOType::Pointer writer = IOType::New();
  writer->SetFileName(output);
  if( isEucSet )
  {
    writer->SetInput( myFilter->GetOutput() );
  }
  else
  {
    writer->SetInput( myExpFilter->GetOutput() );
  }
  
  std::cout << "Writing..." << std::flush;
  try
  {
    writer->Write();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e;
    exit(-1);
  }
  std::cout << "Done." << std::endl;
  
  return 0;
}
