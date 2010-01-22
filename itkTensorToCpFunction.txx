#ifndef _itk_TensorToCpFunction_txx_
#define _itk_TensorToCpFunction_txx_

#include "itkTensorToCpFunction.h"

namespace itk
{


  template <class TInputPixelType, class TOutputPixelType>
  typename TensorToCpFunction<TInputPixelType,TOutputPixelType>::OutputPixelType
  TensorToCpFunction<TInputPixelType,TOutputPixelType>
  ::ComputeScalar (const InputPixelType & T) const
  {

	return T.GetCp();
    
  }
  


}// end of namespace



#endif
