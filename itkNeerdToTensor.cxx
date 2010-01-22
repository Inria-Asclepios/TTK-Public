#include <iostream>

#include <itkImage.h>
#include <itkTensor.h>
#include <itkTensorImageIO.h>

void PrintHelp (const char* exec)
{
  std::cout << "Usage: " << std::endl;
  std::cout << exec << " <file in> <file out>" << std::endl;
  exit (0);
}


int main (int narg, char* arg[])
{

  if( narg != 3 )
  {
    PrintHelp (arg[0]);
  }

  
  // write the output
  typedef itk::TensorImageIO<double, 3, 3> IOType;


  IOType::Pointer myReader = IOType::New();
  myReader->SetFileName(arg[1]);
  try
  {
	myReader->Read();
  }
  catch(itk::ExceptionObject &e)
  {
	std::cerr << e;
	exit(-1);
  }


  IOType::Pointer myWriter = IOType::New();
  myWriter->SetFileName(arg[2]);
  myWriter->SetInput( myReader->GetOutput() );
  
  std::cout << "Writing: " << arg[2] << std::flush;
  try
  {
    myWriter->Write();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e;
    exit(-1);
  }
  std::cout << " Done." << std::endl;

    

  return 0;
}
