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
#include "itkTensorAnisotropicFilteringCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkTensorAnisotropicFilteringCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  TensorAnisotropicFilteringCommandFactory::TensorAnisotropicFilteringCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkTensorAnisotropicFilteringCommand",
			   "Perform tensor Perona & Malik anistropic filtering using Log-Euclidean metric",
			   1,
			   CreateObjectFunction<TensorAnisotropicFilteringCommand>::New());
  }
  
  TensorAnisotropicFilteringCommandFactory::~TensorAnisotropicFilteringCommandFactory()
  {
  }
  
  const char* 
  TensorAnisotropicFilteringCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  TensorAnisotropicFilteringCommandFactory::GetDescription(void) const
  {
    return "Perform tensor Perona & Malik anistropic filtering using Log-Euclidean metric";
  }
  
} // end namespace itk
