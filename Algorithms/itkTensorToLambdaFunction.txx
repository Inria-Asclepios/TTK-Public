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
#ifndef _itk_TensorToLambdaFunction_txx_
#define _itk_TensorToLambdaFunction_txx_

#include "itkTensorToLambdaFunction.h"
#include "vnl/vnl_trace.h"

namespace itk
{


  template <class TInputPixelType, class TOutputPixelType>
  typename TensorToLambdaFunction<TInputPixelType,TOutputPixelType>::OutputPixelType
  TensorToLambdaFunction<TInputPixelType,TOutputPixelType>
  ::ComputeScalar (const InputPixelType & T) const
  {
  
	if( this->GetLambdaIndex()>=InputPixelType::Dimension )
	{
		throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Lambda index is too high.");
	}

	return T.GetEigenvalue( this->GetLambdaIndex() );
    
  }
  


}// end of namespace



#endif
