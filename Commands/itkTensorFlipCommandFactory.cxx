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
#include "itkTensorFlipCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkTensorFlipCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  TensorFlipCommandFactory::TensorFlipCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkTensorFlipCommand",
			   "flip tensors w.r.t. a given axis",
			   1,
			   CreateObjectFunction<TensorFlipCommand>::New());
  }
  
  TensorFlipCommandFactory::~TensorFlipCommandFactory()
  {
  }
  
  const char* 
  TensorFlipCommandFactory::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  TensorFlipCommandFactory::GetDescription() const
  {
    return "Flip tensors w.r.t. a given axis";
  }
  
} // end namespace itk
