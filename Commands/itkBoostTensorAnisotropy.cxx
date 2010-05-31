#include "itkLogTensorImageFilter.h"
#include "itkExpTensorImageFilter.h"
#include "itkResampleTensorImageFilter.h"
// Software Guide : BeginCodeSnippet
#include "itkCenteredRigid2DTransform.h"
#include "itkAffineTensorTransform.h"
#include "itkTransformFileWriter.h"
#include "itkTransformFileReader.h"
#include "itkTransformFactory.h"
// Software Guide : EndCodeSnippet
#include "itkTensorLinearInterpolateImageFunction.h"
#include "itkTensorImageIO.h"
#include <itkFixedCenterOfRotationAffineTransform.h>
#include "GetPot.h"


void PrintHelp(const char* exec)
{
  std::cout << "Usage: " << std::endl;
  std::cout << exec << " <-i input> <-r ratio (1.0 -- 3.0)> <-o output>" << std::endl;
  exit(0);
}


int main(int argc, char* argv[])
{
  
  GetPot cl(argc, argv); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") )
  {
    PrintHelp(cl[0]);
  }
  
  const bool IsInputPresent = cl.search(2,"-I","-i");
  const bool IsOutputPresent = cl.search(2,"-O","-o");

  if( !IsInputPresent || !IsOutputPresent )
  {
    std::cerr << "Error: Input and (or) output not set." << std::endl;
    exit (-1);
  }
  
  const char* tensorFile = cl.follow("NoFile",2,"-I","-i");
  double  ratio = cl.follow(2.0,2,"-r","-R");
  const char* outFile = cl.follow("NoFile",2,"-O","-o");
    
  typedef double                                ScalarType;  
  typedef itk::Image<ScalarType,3>              ImageType;
  typedef itk::TensorImageIO<ScalarType, 3, 3>  IOType;
  typedef IOType::TensorImageType               TensorImageType;
  typedef TensorImageType::SizeType    SizeType;
  typedef TensorImageType::SpacingType SpacingType;
  typedef TensorImageType::PointType   PointType;

  
  std::cout<<"reading list : "<<tensorFile<<std::endl;
  IOType::Pointer reader = IOType::New();
  reader->SetFileName(tensorFile);
  
  try
  {
    reader->Read();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e << std::endl;
    exit(-1);
  }

  TensorImageType::Pointer tensors = reader->GetOutput();
  std::cout << "done."<<std::endl;
  
  
  TensorImageType::Pointer output = TensorImageType::New();

  output->SetRegions (tensors->GetLargestPossibleRegion());
  output->SetOrigin(tensors->GetOrigin());
  output->SetSpacing(tensors->GetSpacing());  
  output->SetDirection(tensors->GetDirection());
  output->Allocate();
  
  itk::ImageRegionIterator<TensorImageType> itIn(tensors, tensors->GetLargestPossibleRegion());
  itk::ImageRegionIterator<TensorImageType> itOut(output, output->GetLargestPossibleRegion());
  itk::ContinuousIndex<double, 3> index;

  TensorImageType::PointType x;
  TensorImageType::PixelType pix;

  itOut.GoToBegin();
  itIn.GoToBegin();

  TensorImageType::PixelType::MatrixType U;
  TensorImageType::PixelType::MatrixType D;

  std::cout<<"ratio : "<<ratio<<std::endl;
  
  while( !itOut.IsAtEnd() )
  {
    pix = itIn.Get();

    D.Fill (0.0);
    D[0][0] = pix.GetEigenvalue (0);
    D[1][1] = pix.GetEigenvalue (1);
    D[2][2] = pix.GetEigenvalue (2) * (ratio);

    for (unsigned int i=0; i<3; i++)
      for (unsigned int j=0; j<3; j++)
	U[i][j] = pix.GetEigenvector (j)[i];
    
    pix.SetVnlMatrix (U * D * U.GetTranspose());
    
    itOut.Set ( pix );
    ++itIn;
    ++itOut;
  }


  IOType::Pointer writer = IOType::New();
  
  writer->SetFileName(outFile);
  writer->SetInput( output );
  
  std::cout << "Writing: " << outFile << std::flush;
  try
  {
    writer->Write();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e << std::endl;
    exit(-1);
  }
  std::cout << " Done." << std::endl;
  
  return 0;

  
}
