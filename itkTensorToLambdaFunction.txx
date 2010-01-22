#ifndef _itk_TensorToLambdaFunction_txx_
#define _itk_TensorToLambdaFunction_txx_

#include "itkTensorToLambdaFunction.h"
#include "vnl/vnl_trace.h"

namespace itk
{


  template <class TInputPixelType, class TOutputPixelType>
  typename TensorToLambdaFunction<TInputPixelType,TOutputPixelType>::OutputPixelType
  TensorToLambdaFunction<TInputPixelType,TOutputPixelType>
  ::ComputeScalar (const InputPixelType & T) const
  {
  
	if( this->GetLambdaIndex()>=InputPixelType::Dimension )
	{
		throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Lambda index is too high.");
	}

	return T.GetEigenvalue( this->GetLambdaIndex() );
    
  }
  


}// end of namespace



#endif
