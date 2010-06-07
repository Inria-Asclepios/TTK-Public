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
#ifndef _itk_TensorToLogFAFunction_txx_
#define _itk_TensorToLogFAFunction_txx_

#include "itkTensorToLogFAFunction.h"
#include "vnl/vnl_trace.h"

namespace itk
{


  template <class TInputPixelType, class TOutputPixelType>
  typename TensorToLogFAFunction<TInputPixelType,TOutputPixelType>::OutputPixelType
  TensorToLogFAFunction<TInputPixelType,TOutputPixelType>
  ::ComputeScalar (const InputPixelType & T) const
  {

    typedef OutputPixelType ValueType;
    typedef typename TInputPixelType::ComponentType InputScalarType;
    
    //vnl_matrix<ValueType> M = T.Log().GetVnlMatrix();
    vnl_matrix<InputScalarType> L = (T.Log()).GetVnlMatrix();
    InputScalarType lt = static_cast<InputScalarType>( vnl_trace(L) / static_cast<InputScalarType>(InputPixelType::Dimension) );

    InputPixelType eye;
    vnl_matrix<InputScalarType> EYE  = eye.GetVnlMatrix();    
    EYE.set_identity();
        
    vnl_matrix<InputScalarType> TOT = lt*lt*EYE + L*L -lt*static_cast<InputScalarType>(2.0)*L;

    OutputPixelType fa = static_cast<OutputPixelType>
      ( tanh (sqrt ( vnl_trace (TOT) )) );

    return fa;
  }
  


}// end of namespace



#endif
