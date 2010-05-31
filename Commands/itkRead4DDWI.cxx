#include <iostream>

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkNiftiImageIO.h>
#include <itkNrrdImageIO.h>
#include <itkVectorImage.h>

void PrintHelp (const char* exec)
{
  std::cout << "Usage: " << std::endl;
  std::cout << exec << " <file in>" << std::endl;
  exit (0);
}


int main (int narg, char* arg[])
{

  if( narg != 2 )
  {
    PrintHelp (arg[0]);
  }

  
  // write the output
  typedef itk::VectorImage<float, 3>            ImageType;
  typedef itk::ImageFileReader<ImageType> IOType;


  IOType::Pointer myReader = IOType::New();
  
  itk::NrrdImageIO::Pointer io = itk::NrrdImageIO::New();
  io->SetFileName(arg[1]);
  io->ReadImageInformation();
  
  std::cout << "NUmber of dimensions: " << io->GetNumberOfDimensions() << std::endl;
  std::cout << io << std::endl;
  
  myReader->SetFileName(arg[1]);
  myReader->SetImageIO(io);
  try
  {
    myReader->Update();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e;
    exit(-1);
  }

  ImageType::Pointer myImage = myReader->GetOutput();

  std::cout << myImage << std::endl;
    

  return 0;
}
