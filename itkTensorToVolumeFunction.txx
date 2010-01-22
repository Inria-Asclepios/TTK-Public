#ifndef _itk_TensorToVolumeFunction_txx_
#define _itk_TensorToVolumeFunction_txx_

#include "itkTensorToVolumeFunction.h"

namespace itk
{


  template <class TInputPixelType, class TOutputPixelType>
  typename TensorToVolumeFunction<TInputPixelType,TOutputPixelType>::OutputPixelType
  TensorToVolumeFunction<TInputPixelType,TOutputPixelType>
  ::ComputeScalar (const InputPixelType & T) const
  {

    OutputPixelType vol = 1.0;
    for( unsigned int i=0; i< InputPixelType::Dimension; i++)
      vol *= T.GetEigenvalue (i);

    return vol;
  }
  


}// end of namespace



#endif
