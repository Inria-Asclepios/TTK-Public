#ifndef _itk_TensorToVRFunction_txx_
#define _itk_TensorToVRFunction_txx_

#include "itkTensorToVRFunction.h"

namespace itk
{


  template <class TInputPixelType, class TOutputPixelType>
  typename TensorToVRFunction<TInputPixelType,TOutputPixelType>::OutputPixelType
  TensorToVRFunction<TInputPixelType,TOutputPixelType>
  ::ComputeScalar (const InputPixelType & T) const
  {  
    return T.GetVR();
  }
  


}// end of namespace



#endif
