/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_TensorToFAFunction_txx_
#define _itk_TensorToFAFunction_txx_

#include "itkTensorToFAFunction.h"
#include "vnl/vnl_trace.h"

namespace itk
{


  template <class TInputPixelType, class TOutputPixelType>
  typename TensorToFAFunction<TInputPixelType,TOutputPixelType>::OutputPixelType
  TensorToFAFunction<TInputPixelType,TOutputPixelType>
  ::ComputeScalar (const InputPixelType & T) const
  {

    typedef typename InputPixelType::ValueType ValueType;
    vnl_matrix<ValueType> M = T.GetVnlMatrix();

    ValueType t = vnl_trace(M);
    ValueType t2 = vnl_trace(M*M);
    
    ValueType tmp = (t*t)/(3.0*t2);

    if(tmp>1.0)
      tmp = 1.0;
    
    OutputPixelType fa = static_cast<OutputPixelType>
      ( sqrt ( 1.5* ( 1.0  - tmp  )) );

    
    return fa;
  }
  


}// end of namespace



#endif
