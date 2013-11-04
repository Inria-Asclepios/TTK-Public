#include <iostream>
#include "GetPot.h"

#include <itkImageSeriesReader.h>
#include <itkImage.h>
#include <itkImageFileWriter.h>

#include <itksys/SystemTools.hxx>
#include <itksys/Directory.hxx>

void PrintHelp (const char* exec)
{
  std::cout << "Usage:\n";
  std::cout << exec << " <-sx x spacing (default: image)> <-sy y spacing (default: image)> <-sz z spacing (default: image)> <-st t spacing (default: 1.0)> <-o output (default: output.nii.gz)> <-i input1 input2 ...> <-d directory>\n";
}


int main (int argc, char* argv[])
{

  GetPot cl (argc, argv);
  if( cl.size()==1 || cl.search (2,"-h","--help") )
  {
    PrintHelp (cl[0]);
    return -1;
  }

  const char* output = cl.follow ("output.nii.gz", 2, "-o", "-O");

  std::vector<std::string> filenames;

  std::string input = cl.follow ("", 2, "-i", "-I");
  while (input!="")
  {
    filenames.push_back (input);
    input = cl.next("");
  }

  std::string s_directory = cl.follow ("directory", 2, "-d", "-D");
  if ( itksys::SystemTools::FileIsDirectory ( s_directory.c_str() ) )
  {
    itksys::Directory directory;
    directory.Load( s_directory.c_str() );
        
    for( unsigned long i=0; i<directory.GetNumberOfFiles(); i++ )
    {
      std::string name = directory.GetPath();
      std::string file = directory.GetFile (i);
      if( file!="." && file!=".." )
      {
#ifdef WIN32
	name += "\\" + file;
#else
	name += "/" + file;
#endif
	filenames.push_back (name);
      }
    }
  }


  if (!filenames.size())
  {
    std::cerr << "Error: no input specified" << std::endl;
    return -1;
  }



  itk::ImageIOBase::Pointer io = itk::ImageIOFactory::CreateImageIO (filenames[0].c_str(), itk::ImageIOFactory::ReadMode);
  if (io.IsNull())
    return -1;

  io->SetFileName (filenames[0].c_str());
  io->ReadImageInformation();

  if (io->GetNumberOfDimensions()==2) {
    
    typedef itk::Image<unsigned char, 3>      ImageType;
    typedef itk::ImageSeriesReader<ImageType> SeriesReaderType;
    
    
    SeriesReaderType::Pointer reader = SeriesReaderType::New();
    std::cout << "Adding:\n";
    for (unsigned int i=0; i<filenames.size(); i++)
    {
      std::cout << filenames[i] << std::endl;
      reader->AddFileName ( filenames[i].c_str() );
    }
    
    try
    {
      reader->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }


    double spacingX = cl.follow (io->GetSpacing (0), 2, "-sx", "-SX");
    double spacingY = cl.follow (io->GetSpacing (1), 2, "-sy", "-SY");
    double spacingZ = cl.follow (1.0, 2, "-sz", "-SZ");
    
    
    ImageType::Pointer image = reader->GetOutput();
    ImageType::SpacingType spacing = image->GetSpacing();
    spacing[0] = spacingX;
    spacing[1] = spacingY;
    spacing[2] = spacingZ;
    image->SetSpacing (spacing);
    
    typedef itk::ImageFileWriter<ImageType> OutputWriterType;
    {
      OutputWriterType::Pointer writer = OutputWriterType::New();
      writer->SetFileName ( output );
      writer->SetInput ( image );
      
      std::cout << "Writing: " << output << std::flush;
      try
      {
      writer->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << " Done." << std::endl;
    }
  }
  else if (io->GetNumberOfDimensions()==3)
  {
    
    typedef itk::Image<short, 4>      ImageType;
    typedef itk::ImageSeriesReader<ImageType> SeriesReaderType;
    
    SeriesReaderType::Pointer reader = SeriesReaderType::New();
    std::cout << "Adding:\n";
    for (unsigned int i=0; i<filenames.size(); i++)
    {
      std::cout << filenames[i] << std::endl;
      reader->AddFileName ( filenames[i].c_str() );
    }
    
    try
    {
      reader->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }

    double spacingX = cl.follow (io->GetSpacing (0), 2, "-sx", "-SX");
    double spacingY = cl.follow (io->GetSpacing (1), 2, "-sy", "-SY");
    double spacingZ = cl.follow (io->GetSpacing (2), 2, "-sz", "-SZ");
    double spacingT = cl.follow (1.0, 2, "-st", "-ST");
    
    ImageType::Pointer image = reader->GetOutput();
    ImageType::SpacingType spacing = image->GetSpacing();
    spacing[0] = spacingX;
    spacing[1] = spacingY;
    spacing[2] = spacingZ;
    spacing[3] = spacingT;
    image->SetSpacing (spacing);
    
    typedef itk::ImageFileWriter<ImageType> OutputWriterType;
    {
      OutputWriterType::Pointer writer = OutputWriterType::New();
      writer->SetFileName ( output );
      writer->SetInput ( image );
      
      std::cout << "Writing: " << output << std::flush;
      try
      {
	writer->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << " Done." << std::endl;
    }
  }
  
  
  return 0;  
  
}
