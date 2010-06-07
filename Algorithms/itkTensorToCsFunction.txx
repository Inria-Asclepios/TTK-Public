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
