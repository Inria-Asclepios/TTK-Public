#ifndef _itk_TensorToClFunction_txx_
#define _itk_TensorToClFunction_txx_

#include "itkTensorToClFunction.h"

namespace itk
{


  template <class TInputPixelType, class TOutputPixelType>
  typename TensorToClFunction<TInputPixelType,TOutputPixelType>::OutputPixelType
  TensorToClFunction<TInputPixelType,TOutputPixelType>
  ::ComputeScalar (const InputPixelType & T) const
  {

	return T.GetCl();
    
  }
  


}// end of namespace



#endif
