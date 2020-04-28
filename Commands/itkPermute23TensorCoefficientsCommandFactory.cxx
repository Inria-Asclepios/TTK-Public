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
#include "itkPermute23TensorCoefficientsCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkPermute23TensorCoefficientsCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  Permute23TensorCoefficientsCommandFactory::Permute23TensorCoefficientsCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkPermute23TensorCoefficientsCommand",
			   "Permute 2nd and 3rd coefficients of all tensors",
			   1,
			   CreateObjectFunction<Permute23TensorCoefficientsCommand>::New());
  }
  
  Permute23TensorCoefficientsCommandFactory::~Permute23TensorCoefficientsCommandFactory()
  {
  }
  
  const char* 
  Permute23TensorCoefficientsCommandFactory::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  Permute23TensorCoefficientsCommandFactory::GetDescription() const
  {
    return "Divide tensors by the largest eigenvalue of the field";
  }
  
} // end namespace itk
