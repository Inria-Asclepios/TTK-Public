/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL:$
  Language:  C++
  Date:      $Date:$
  Version:   $Revision:$

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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
