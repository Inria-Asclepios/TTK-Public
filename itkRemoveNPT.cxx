#include "itkRemoveNonPositiveTensorsTensorImageFilter.h"

#include "itkTensorImageIO.h"


#include "GetPot.h"

#include <itkImageFileReader.h>
#include <itkImage.h>

void PrintHelp(const char* exec)
{

  std::cout << exec << ": " << std::endl;
  std::cout << "-i [tensor field]" << std::endl;
  std::cout << "-r [NPT removing type: 0: null tensor / 1: mean of neighbors]" << std::endl;
  std::cout << "-o [Output file]" << std::endl;
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


  const char* fileIn = cl.follow("NoFile",2,"-i","-I");
  const char* fileOut = cl.follow("NoFile",2,"-o","-O");
  const int RemovingType = cl.follow (0,2,"-r","-R");

  typedef double ScalarType;  
  typedef itk::Image<ScalarType, 3>                      ImageType;
  typedef itk::TensorImageIO<ScalarType, 3, 3>           IOType;
  typedef IOType::TensorImageType                        TensorImageType;    
  typedef itk::RemoveNonPositiveTensorsTensorImageFilter<TensorImageType,TensorImageType>
    RemoveNPTFilterType;
  

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
  
  
  
  RemoveNPTFilterType::Pointer myRemover = RemoveNPTFilterType::New();
  myRemover->SetInput ( myIO->GetOutput() );
  myRemover->SetRemovingType (RemovingType);
  
  // now: filter
  std::cout << "Removing NPT..." << std::flush;
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
  myIO->SetFileName(fileOut);
  myIO->SetInput (myRemover->GetOutput());
  
  std::cout << "Writing..." << std::flush;
  try
    {
      myIO->Write();
    }
  catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      exit(-1);
    }
  std::cout << "Done." << std::endl;
  
  return 0;

}
