#include "itkTensorZScoreCalculatorCommand.h"

#include "itkTensorImageIO.h"
#include "itkZScoreTensorImageFilter.h"
#include <itkImageFileWriter.h>

#include <iostream>
#include "GetPot.h"

namespace itk
{

  TensorZScoreCalculatorCommand::TensorZScoreCalculatorCommand()
  {
    m_ShortDescription = "Calculate a tensor z-score given an average tensor field and a covariance matrix field";
    m_LongDescription  = "Usage:\n";
    m_LongDescription += "-i <input tensor field> -a <average tensor field> -c <field of covariance matrices> -o <zscore map>\n\n";
    m_LongDescription += m_ShortDescription;
  }
  
  TensorZScoreCalculatorCommand::~TensorZScoreCalculatorCommand()
  {}

  
  int TensorZScoreCalculatorCommand::Execute (int narg, const char* arg[])
  {
    
    itk::Object::GlobalWarningDisplayOff();
    
    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
	
    
    const char* fileIn = cl.follow("NoFile",2,"-i","-I");
    const char* fileAverage = cl.follow("NoFile",2,"-a","-A");
    const char* fileCovMat = cl.follow("NoFile",2,"-c","-C");
    const char* fileOut = cl.follow("NoFile",2,"-o","-O");
  
  
    typedef double ScalarType;
    
    typedef itk::Image<ScalarType, 3> ImageType;
    typedef itk::TensorImageIO<ScalarType, 3, 3> TensorIOType;
    typedef TensorIOType::TensorImageType        TensorImageType;
    typedef itk::TensorImageIO<ScalarType, 6, 3> CovMatrixIOType;
    typedef CovMatrixIOType::TensorImageType     CovMatrixImageType;
    
    typedef itk::ZScoreTensorImageFilter<TensorImageType, ImageType> ZScoreFilterType;
    

    TensorImageType::Pointer tensors = nullptr;
    TensorImageType::Pointer average = nullptr;
    CovMatrixImageType::Pointer covMatrix = nullptr;
    ImageType::Pointer result = nullptr;
    
    {
      
      std::cout << "Reading: " << fileIn << std::flush;
      TensorIOType::Pointer reader1 = TensorIOType::New();
      reader1->SetFileName(fileIn);
      try
      {
	reader1->Read();
      }
      catch(itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << "Done." << std::endl;
      tensors = reader1->GetOutput();
      
    }


    {
      std::cout << "Reading: " << fileAverage << std::flush;
      TensorIOType::Pointer reader = TensorIOType::New();
      reader->SetFileName(fileAverage);
      try
      {
	reader->Read();
      }
      catch(itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << "Done." << std::endl;
      average = reader->GetOutput();
    }
    
    
    {
      std::cout << "Reading: " << fileCovMat << std::flush;
      CovMatrixIOType::Pointer reader = CovMatrixIOType::New();
      reader->SetFileName(fileCovMat);
      try
      {
	reader->Read();
      }
      catch(itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << "Done." << std::endl;
      covMatrix = reader->GetOutput();
    }
    


    {
      ZScoreFilterType::Pointer myZScore = ZScoreFilterType::New();
      myZScore->SetInput( tensors );
      myZScore->SetAverageTensorImage( average );
      myZScore->SetCovarianceMatrixImage( covMatrix );
      //myZScore->SetNumberOfThreads(1);
      
      std::cout << "ZScoring..." << std::flush;
      try
      {
	myZScore->Update();
      }
      catch(itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << "Done." << std::endl;
      
      result = myZScore->GetOutput();
    }
    
    
    std::cout << "Writing resut..." << std::flush;
    
    typedef itk::ImageFileWriter<ImageType> WriterType;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName( fileOut );
    writer->SetInput( result );
    
    try
    {
      writer->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;
    
    return 0;
  }
  
}
