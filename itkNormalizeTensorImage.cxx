
#include "itkNormalizeTensorImageFilter.h"
#include "itkTensorImageIO.h"

#include "GetPot.h"

#include <itkImageFileReader.h>
#include <itkImage.h>


void PrintHelp(const char* exec)
{

  std::cout << std::endl;
  std::cout << exec << ": " << std::endl;
  std::cout << "-i <input>" << std::endl;
  std::cout << "-o <output>" << std::endl;
  exit(0);

}

int main(int narg, char* arg[])
{

  GetPot   cl(narg, arg); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    PrintHelp(cl[0]);

  const char* fileIn   = cl.follow("",2,"-i","-I");
  const char* fileOut  = cl.follow("",2,"-o","-O");


  typedef double ScalarType;  
  typedef itk::Image<ScalarType, 3>                      ImageType;
  typedef itk::TensorImageIO<ScalarType, 3, 3>           IOType;
  typedef IOType::TensorImageType                        TensorImageType;    
  typedef itk::NormalizeTensorImageFilter<TensorImageType,TensorImageType>
    FilterType;

    IOType::Pointer myIO = IOType::New();
  myIO->SetFileName (fileIn);
  try
  {
    myIO->Read();
  } catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }
  
  
  FilterType::Pointer myFilter = FilterType::New();
  myFilter->SetInput ( myIO->GetOutput() );

  // now: filter
  try
  {
    myFilter->Update();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }


  
  // write the output
  myIO->SetFileName(fileOut);
  myIO->SetInput ( myFilter->GetOutput() );
  
  std::cout << "Writing..." << std::flush;
  try
  {
    myIO->Write();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }
  std::cout << "Done." << std::endl;

      
  return 0;
}
