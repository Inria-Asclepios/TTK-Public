#ifndef _itk_LogEuclideanDTIEstimatorTensorImageFilter_txx_
#define _itk_LogEuclideanDTIEstimatorTensorImageFilter_txx_
#include "itkLogEuclideanDTIEstimatorTensorImageFilter.h"

namespace itk
{

  template< class TInputImage, class TOutputImage >
  bool
  LogEuclideanDTIEstimatorTensorImageFilter< TInputImage, TOutputImage >
  ::Halt()
  {
    std::cout << "Delta Energy: " << this->GetRMSChange() << std::endl;
    std::cout << std::endl;
    
    if (this->GetNumberOfIterations() != 0)
      this->UpdateProgress( static_cast<float>( this->GetElapsedIterations() ) /
                            static_cast<float>( this->GetNumberOfIterations() ) );
    
    if (this->GetElapsedIterations() >= this->GetNumberOfIterations())
      return true;
    else
      if ( this->GetElapsedIterations() == 0)
	return false; 
      else
        if ( this->GetRMSChange() <= this->GetMaximumRMSError() )
          return true;
        else
          return false; 
  }
  
    
  template< class TInputImage, class TOutputImage >
  void
  LogEuclideanDTIEstimatorTensorImageFilter<TInputImage,TOutputImage>
  ::InitializeIteration()
  {
    std::cout << "######### Iteration No " << this->GetElapsedIterations() << " #########" << std::endl;
    
    if( this->GetElapsedIterations() ==0 )
      m_GradientFilter->SetInput(this->GetInput());
    else
    {
      this->GetOutput()->Modified();
      m_GradientFilter->SetInput(this->GetOutput());
    }
    
    std::cout << "Calculating the gradient..." << std::flush;
    try
    {
      m_GradientFilter->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject(__FILE__,
                                 __LINE__,
                                 "Error in LogEuclideanDTIEstimatorTensorImageFilter::InitializeIteration()");
    }
    std::cout << "Done." << std::endl;
    std::cout << std::flush;
    
   
    m_Function->SetGradient( m_GradientFilter->GetOutput() );
    m_Function->InitializeIteration();
    
  }


  
  template< class TInputImage, class TOutputImage >
  void
  LogEuclideanDTIEstimatorTensorImageFilter< TInputImage, TOutputImage >
  ::BeforeThreadedGenerateData()
  {
    
    if( m_Function->GetGradientList().size()+1 != m_Function->GetDTI().size() )
      throw itk::ExceptionObject(__FILE__,__LINE__,"Gradient and DTI do not match");

    if( ( m_Function->GetEstimationType() == 2 ) && ( m_Function->GetDTI().size() != m_Function->GetVariances().size() ) )
      throw itk::ExceptionObject(__FILE__,__LINE__,"Variances and DTI do not match");
  }
  
  

} // end of namespace

#endif
