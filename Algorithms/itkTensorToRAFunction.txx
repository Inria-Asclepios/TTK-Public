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
