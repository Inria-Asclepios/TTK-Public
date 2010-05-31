#include "itkNormalizeTensorsCommand.h"

#include "itkTensorImageIO.h"
#include "itkTensorToLambdaFunction.h"
#include "itkTensorToScalarTensorImageFilter.h"
#include "itkTensorNormalizeTensorImageFilter.h"
#include <itkImage.h>
#include <itkMinimumMaximumImageFilter.h>

#include <iostream>
#include "GetPot.h"


namespace itk
{

  NormalizeTensorsCommand::NormalizeTensorsCommand()
  {
    m_ShortDescription = "Divide tensors by the largest eigenvalue of the field";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i input> <-o output>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  NormalizeTensorsCommand::~NormalizeTensorsCommand()
  {}

  
  int NormalizeTensorsCommand::Execute (int narg, const char* arg[])
  {
    
    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
    
    
    const char* input   = cl.follow ("NoFile", 2, "-i", "-I");
    const char* output  = cl.follow ("output.inr.gz", 2, "-o", "-O");
    
    
    typedef double                               ScalarType;  
    typedef itk::TensorImageIO<ScalarType, 3, 3> TensorIOType;
    typedef TensorIOType::TensorImageType        TensorImageType;
    typedef itk::Image <ScalarType, 3>           ImageType;
    
    
    TensorImageType::Pointer tensors = 0;
    {
      TensorIOType::Pointer io = TensorIOType::New();
      io->SetFileName (input);
      try
      {
	io->Read();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      
      tensors = io->GetOutput();
    }
    
    
    
    double factor = 1.0;
    // compute l1
    {
      itk::TensorToLambdaFunction <TensorImageType::PixelType, ScalarType>::Pointer function = itk::TensorToLambdaFunction <TensorImageType::PixelType, ScalarType>::New();
      function->SetLambdaIndex( 2 );
      
      typedef itk::TensorToScalarTensorImageFilter<TensorImageType, ImageType>  FilterType;
      
      FilterType::Pointer myFilter = FilterType::New();
      myFilter->SetTensorToScalarFunction ( function );
      myFilter->SetInput ( tensors );
      
      itk::MinimumMaximumImageFilter<ImageType>::Pointer MinMaxFilter = itk::MinimumMaximumImageFilter<ImageType>::New();
      MinMaxFilter->SetInput ( myFilter->GetOutput() );
      
      try
      {
	MinMaxFilter->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      
      factor = MinMaxFilter->GetMaximum();
    }
    
    
    
    {
      
      itk::TensorNormalizeTensorImageFilter <TensorImageType, TensorImageType>::Pointer normalizer = 
	itk::TensorNormalizeTensorImageFilter <TensorImageType, TensorImageType>::New();
      normalizer->SetInput (tensors);
      normalizer->SetNormalizationFactor (factor);
      
      try
      {
	normalizer->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      
      tensors = normalizer->GetOutput();
    }
    
    
    
    TensorIOType::Pointer io = TensorIOType::New();
    io->SetFileName (output);
    io->SetInput ( tensors );
    try
    {
      io->Write();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    
    return 0;
  }
  
}
