#ifndef _itk_TensorToRAFunction_txx_
#define _itk_TensorToRAFunction_txx_

#include "itkTensorToRAFunction.h"

namespace itk
{


  template <class TInputPixelType, class TOutputPixelType>
  typename TensorToRAFunction<TInputPixelType,TOutputPixelType>::OutputPixelType
  TensorToRAFunction<TInputPixelType,TOutputPixelType>
  ::ComputeScalar (const InputPixelType & T) const
  {  
    return T.GetRA();
  }
  


}// end of namespace



#endif
