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
#include "itkTensorGaussianFilteringCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkTensorGaussianFilteringCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  TensorGaussianFilteringCommandFactory::TensorGaussianFilteringCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkTensorGaussianFilteringCommand",
			   "Log-Eculidean Gaussian filtering of tensors",
			   1,
			   CreateObjectFunction<TensorGaussianFilteringCommand>::New());
  }
  
  TensorGaussianFilteringCommandFactory::~TensorGaussianFilteringCommandFactory()
  {
  }
  
  const char* 
  TensorGaussianFilteringCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  TensorGaussianFilteringCommandFactory::GetDescription(void) const
  {
    return "Log-Eculidean Gaussian filtering of tensors";
  }
  
} // end namespace itk
