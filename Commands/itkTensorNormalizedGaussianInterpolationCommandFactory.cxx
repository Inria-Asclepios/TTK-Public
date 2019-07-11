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
#include "itkTensorNormalizedGaussianInterpolationCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkTensorNormalizedGaussianInterpolationCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  TensorNormalizedGaussianInterpolationCommandFactory::TensorNormalizedGaussianInterpolationCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkTensorNormalizedGaussianInterpolationCommand",
			   "Extrapolate sparse tensors on a regular grid using a normalized Gaussian",
			   1,
			   CreateObjectFunction<TensorNormalizedGaussianInterpolationCommand>::New());
  }
  
  TensorNormalizedGaussianInterpolationCommandFactory::~TensorNormalizedGaussianInterpolationCommandFactory()
  {
  }
  
  const char* 
  TensorNormalizedGaussianInterpolationCommandFactory::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  TensorNormalizedGaussianInterpolationCommandFactory::GetDescription() const
  {
    return "Extrapolate sparse tensors on a regular grid using a normalized Gaussian";
  }
  
} // end namespace itk
