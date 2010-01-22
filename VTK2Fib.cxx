#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <vector>
#include <list>
#include <string>

#include "GetPot.h"

#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"

#include <itkGroupSpatialObject.h>
#include <itkSpatialObjectWriter.h>

#include <itkImage.h>
#include "itkTensor.h"
#include "itkTensorImageIO.h"
#include "itkLogTensorImageFilter.h"
#include "itkVTKFibersToITKFibers.h"


void PrintHelp (const char* exec)
{
  std::cout << exec << std::endl;
  std::cout << "-i [vtk fiber file]" << std::endl;
  std::cout << "-t [tensor file]" << std::endl;
  std::cout << "-b [b-value]" << std::endl;
  std::cout << "-o [itk fiber file - output]" << std::endl;
  exit (0);
}



int main(int narg,char* arg[])
{

  GetPot cl (narg, arg);
  if( cl.size() == 1  || cl.search (2,"--help","-h") )
    PrintHelp (arg[0]);

  const char* vtkFiberFile  = cl.follow ("NoFile",2,"-i","-I");
  const char* inrTensorFile = cl.follow ("NoFile",2,"-t","-T");
  const double bvalue       = cl.follow (1.0, 2, "-b","-B");
  const char* itkFiberFile  = cl.follow ("NoFile", 2, "-o","-O");
  

  // Read the input:
  typedef double  ScalarType;
  const unsigned int ImageDimension  = 3;
  const unsigned int TensorDimension = 3;
  
  typedef itk::TensorImageIO<ScalarType, TensorDimension, ImageDimension> IOType;
  typedef IOType::TensorImageType     TensorImageType;
  typedef TensorImageType::PointType  PointType;
  typedef TensorImageType::IndexType  IndexType;
  typedef TensorImageType::PixelType  TensorType;
  typedef TensorType::VectorType      VectorType;
  typedef TensorImageType::SizeType   SizeType;

  
  // read the vtk fiber file
  std::cout << "Reading: " << vtkFiberFile << std::endl;
  vtkPolyDataReader* reader = vtkPolyDataReader::New();
  reader->SetFileName( vtkFiberFile );
  reader->Update();

  vtkPolyData* vtkFibers = reader->GetOutput();
  vtkFibers->Update();


  // read the inrimage tensor file
  IOType::Pointer myReader = IOType::New();
  myReader->SetFileName (inrTensorFile);
  
  std::cout << "Reading: " << inrTensorFile;
  std::cout << std::flush;
  try
  {
    myReader->Read();
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    exit (-1);
  }
  std::cout << " Done." << std::endl;
  

  SizeType dims = myReader->GetOutput()->GetLargestPossibleRegion().GetSize();


  // log the input
  typedef itk::LogTensorImageFilter<TensorImageType,TensorImageType>
    LogFilterType;
  LogFilterType::Pointer myLog = LogFilterType::New();
  myLog->SetInput (myReader->GetOutput() );
  std::cout << "Logarithming... " << std::flush;
  try
  {
    myLog->Update();
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    exit (-1);
  }
  std::cout << " Done." << std::endl;
  TensorImageType::Pointer myTensors = myLog->GetOutput();


  // convert
  typedef itk::VTKFibersToITKFibers<TensorImageType> ConverterType;
  typedef ConverterType::GroupSpatialObjectType      GroupSpatialObjectType;
  ConverterType::Pointer myConverter = ConverterType::New();
  myConverter->SetInput (vtkFibers);
  myConverter->SetTensorImage (myTensors);
  myConverter->SetBVal (bvalue);

  std::cout << "Converting...";
  std::cout << std::flush;
  try
  {
    myConverter->Update();
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }
  std::cout << "Done." << std::endl;
  
  GroupSpatialObjectType::Pointer output = myConverter->GetOutput();


  std::cout << "Wrting...";
  itk::SpatialObjectWriter<3>::Pointer fibWriter  = itk::SpatialObjectWriter<3>::New();
  fibWriter->SetInput(output);
  fibWriter->SetFileName(itkFiberFile);
  fibWriter->Update();
  std::cout << "Done." << std::endl;
  
  reader->Delete();
  
  return 0;
}

