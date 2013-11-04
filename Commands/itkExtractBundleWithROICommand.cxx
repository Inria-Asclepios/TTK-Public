#include "itkExtractBundleWithROICommand.h"

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageToVTKImageFilter.h>

#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkMatrix4x4.h>

#include <vtkLimitFibersToROI.h>

#include <errno.h>
#include <iostream>
#include "GetPot.h"

int atoi_check( const char * str )
{
  char *endptr;
  long val= strtol(str, &endptr, 0);
  
  /* Check for various possible errors */
  if ( (errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
       || val>=INT_MAX || val<=INT_MIN )
    {
    std::cout<<std::endl;
    std::cout<<"Cannot parse integer. Out of bound."<<std::endl;
    exit( EXIT_FAILURE );
    }
  
  if (endptr == str || *endptr!='\0')
    {
    std::cout<<std::endl;
    std::cout<<"Cannot parse integer. Contains non-digits or is empty."<<std::endl;
    exit( EXIT_FAILURE );
    }

  return val;
}

  std::vector<unsigned int> parseUIntVector( const std::string & str)
{
  std::vector<unsigned int> vect;
  
  std::string::size_type crosspos = str.find('x',0);
  
  if (crosspos == std::string::npos)
    {
    // only one uint
    vect.push_back( static_cast<unsigned int>( atoi_check(str.c_str()) ));
    return vect;
    }

  // first uint
  vect.push_back( static_cast<unsigned int>(
                     atoi_check( (str.substr(0,crosspos)).c_str()  ) ));

  while(true)
    {
    std::string::size_type crossposfrom = crosspos;
    crosspos =  str.find('x',crossposfrom+1);
    
    if (crosspos == std::string::npos)
      {
      vect.push_back( static_cast<unsigned int>(
                         atoi_check( (str.substr(crossposfrom+1,str.length()-crossposfrom-1)).c_str()  ) ));
      return vect;
      }
    
    vect.push_back( static_cast<unsigned int>(
                       atoi_check( (str.substr(crossposfrom+1,crosspos-crossposfrom-1)).c_str()  ) ));
    }
}


namespace itk
{

  ExtractBundleWithROICommand::ExtractBundleWithROICommand()
  {
    m_ShortDescription = "Extract a bundle given a set of fibers and a ROI";
    m_LongDescription  = "Usage:\n";
    m_LongDescription += "-i <input fibers> -m <ROI> -b <boolean operations: roi0xroi1xroi2 where roii = 0 (NULL), 1 (NOT), 2 (AND)> -o <output bundle>\n\n";
    m_LongDescription += m_ShortDescription;
  }
  
  ExtractBundleWithROICommand::~ExtractBundleWithROICommand()
  {}
  
  int ExtractBundleWithROICommand::Execute (int narg, const char* arg[])
  {    
    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
	
    
    const char* fileIn = cl.follow("",2,"-i","-I");
    const char *roiName = cl.follow("", "-m");
    const char* fileOut = cl.follow("",2,"-o","-O");

    std::vector<unsigned int> booleans;
    if (cl.search("-b"))
    {
       booleans = parseUIntVector(cl.follow("", "-b"));
    }

    std::cout << "Reading " << fileIn << "..." << std::flush;
    vtkPolyDataReader *reader = vtkPolyDataReader::New();
    reader->SetFileName( fileIn );
    reader->Update();
    std::cout << "Done\n";


    typedef itk::Image<unsigned char, 3> MaskImageType;
    MaskImageType::Pointer mask = 0;
    {
        std::cout << "Reading " << roiName << "..." << std::flush;
        typedef itk::ImageFileReader<MaskImageType> MaskReaderType;
        MaskReaderType::Pointer maskReader = MaskReaderType::New();
        maskReader->SetFileName(roiName);
        try
        {
            maskReader->Update();
        }
        catch(itk::ExceptionObject &e)
        {
            std::cerr << e;
            return EXIT_FAILURE;
        }

        std::cout << "Done\n";

        mask = maskReader->GetOutput();
        mask->DisconnectPipeline();
    }

    typedef itk::ImageToVTKImageFilter<MaskImageType> ConverterType;
    ConverterType::Pointer converter = ConverterType::New();
    converter->SetInput (mask);
    converter->Update();

    MaskImageType::DirectionType directions = mask->GetDirection();
    vtkMatrix4x4 *matrix = vtkMatrix4x4::New();
    matrix->Identity();
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            matrix->SetElement (i, j, directions (i,j));


    vtkLimitFibersToROI* limiter = vtkLimitFibersToROI::New();
    limiter->SetInput (reader->GetOutput());
    limiter->SetMaskImage (converter->GetOutput());
    limiter->SetDirectionMatrix (matrix);

    for (unsigned int i=0; i<booleans.size(); i++)
    {
        std::cout << "Label: " << i << " is set to: ";
        switch(booleans[i])
        {
          case 2:
              std::cout << "AND\n";
            break;

          case 1:
              std::cout << "NOT\n";
            break;

          case 0:
              std::cout << "NULL\n";
          default:
            break;
        }
        // ROI 0 is not a ROI
        limiter->SetBooleanOperation (i+1, booleans[i]);
    }

    std::cout << "Limiting..." << std::flush;
    limiter->Update();
    std::cout << "Done\n";

    std::cout << "Cleaning..." << std::flush;
    vtkCleanPolyData *cleaner = vtkCleanPolyData::New();
    cleaner->SetInputConnection (limiter->GetOutputPort());
    cleaner->SetAbsoluteTolerance (0.0);
    cleaner->ConvertLinesToPointsOff ();
    cleaner->Update();
    std::cout << "Done\n";

    std::cout << "Writing " << fileOut << "..." << std::flush;
    vtkPolyDataWriter *writer = vtkPolyDataWriter::New();
    writer->SetInputConnection (cleaner->GetOutputPort());
    writer->SetFileName(fileOut);
    writer->Update();
    std::cout << "Done\n";

    reader->Delete();
    matrix->Delete();
    limiter->Delete();
    writer->Delete();
    
    return 0;
  }
  
}
