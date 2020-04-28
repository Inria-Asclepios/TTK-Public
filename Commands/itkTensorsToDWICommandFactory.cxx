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
#include "itkTensorsToDWICommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkTensorsToDWICommand.h"
#include "itkVersion.h"

namespace itk
{
  
  TensorsToDWICommandFactory::TensorsToDWICommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkTensorsToDWICommand",
			   "Create DWI from tensors using the Stejskal & Tanner diffusion equation (and a list of gradients)",
			   1,
			   CreateObjectFunction<TensorsToDWICommand>::New());
  }
  
  TensorsToDWICommandFactory::~TensorsToDWICommandFactory()
  {
  }
  
  const char* 
  TensorsToDWICommandFactory::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  TensorsToDWICommandFactory::GetDescription() const
  {
    return "Create DWI from tensors using the Stejskal & Tanner diffusion equation (and a list of gradients)";
  }
  
} // end namespace itk
