#ifndef _itk_TensorToL2NormFunction_txx_
#define _itk_TensorToL2NormFunction_txx_

#include "itkTensorToL2NormFunction.h"

namespace itk
{


  template <class TInputPixelType, class TOutputPixelType>
  typename TensorToL2NormFunction<TInputPixelType,TOutputPixelType>::OutputPixelType
  TensorToL2NormFunction<TInputPixelType,TOutputPixelType>
  ::ComputeScalar (const InputPixelType & T) const
  {
    return static_cast<OutputPixelType>( T.GetNorm() );    
  }
  


}// end of namespace



#endif
