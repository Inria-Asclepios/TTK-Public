#ifndef _itk_SparseTensorsDiffusionTensorImageFilter_txx_
#define _itk_SparseTensorsDiffusionTensorImageFilter_txx_


namespace itk
{
  

  template< class TInputImage, class TOutputImage >
  bool
  SparseTensorsDiffusionTensorImageFilter< TInputImage, TOutputImage >
  ::Halt()
  {
    std::cout << "########## " << "Iteration No " << this->GetElapsedIterations() << " ##########" << std::endl;    
    std::cout << "Delta Energy: " << this->GetRMSChange() << std::endl;
    std::cout << std::endl;
    
    if (this->GetNumberOfIterations() != 0)
      {
	this->UpdateProgress( static_cast<float>( this->GetElapsedIterations() ) /
			      static_cast<float>( this->GetNumberOfIterations() ) );
      }
    
    if (this->GetElapsedIterations() >= this->GetNumberOfIterations())
      {
	return true;
      }
    else if ( this->GetElapsedIterations() == 0)
      {
	return false; 
      }
    else if ( this->GetRMSChange() <= this->GetMaximumRMSError() )
      {
	return true;
      }
    else
      { 
	return false; 
      }
    
  }


  template< class TInputImage, class TOutputImage >
  void
  SparseTensorsDiffusionTensorImageFilter< TInputImage, TOutputImage >
  ::Initialize()
  {

    VectorOfTensorsType vecT = m_Function->GetTensors();
    VectorOfPointsType  vecP = m_Function->GetPoints();
    VectorOfPointsType  vecTan = m_Function->GetTangents();
    
    
    if( vecT.size() != vecP.size() )
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: the vector of tensors and points must be of same length.");

    if( vecTan.size() != 0  &&  vecTan.size() != vecT.size() )
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: the vector of tensors and tangents must be of same length.");
    
  }

} // end of namespace

#endif
