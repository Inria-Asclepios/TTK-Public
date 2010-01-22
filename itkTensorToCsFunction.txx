#ifndef _itk_TensorToCsFunction_txx_
#define _itk_TensorToCsFunction_txx_

#include "itkTensorToCsFunction.h"

namespace itk
{


  template <class TInputPixelType, class TOutputPixelType>
  typename TensorToCsFunction<TInputPixelType,TOutputPixelType>::OutputPixelType
  TensorToCsFunction<TInputPixelType,TOutputPixelType>
  ::ComputeScalar (const InputPixelType & T) const
  {

	return T.GetCs();
    
  }
  


}// end of namespace



#endif
