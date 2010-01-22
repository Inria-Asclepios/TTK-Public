#ifndef _itk_TensorToADCFunction_txx_
#define _itk_TensorToADCFunction_txx_

#include "itkTensorToADCFunction.h"

namespace itk
{


  template <class TInputPixelType, class TOutputPixelType>
  typename TensorToADCFunction<TInputPixelType,TOutputPixelType>::OutputPixelType
  TensorToADCFunction<TInputPixelType,TOutputPixelType>
  ::ComputeScalar (const InputPixelType & T) const
  {    
    return static_cast<OutputPixelType>( T.GetTrace() );
  }
  


}// end of namespace



#endif
