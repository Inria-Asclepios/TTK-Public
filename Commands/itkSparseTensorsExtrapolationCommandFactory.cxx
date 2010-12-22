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
#include "itkSparseTensorsExtrapolationCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkSparseTensorsExtrapolationCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  SparseTensorsExtrapolationCommandFactory::SparseTensorsExtrapolationCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkSparseTensorsExtrapolationCommand",
			   "Extrapolate a set of tensors on a regular grid using a diffusion equation",
			   1,
			   CreateObjectFunction<SparseTensorsExtrapolationCommand>::New());
  }
  
  SparseTensorsExtrapolationCommandFactory::~SparseTensorsExtrapolationCommandFactory()
  {
  }
  
  const char* 
  SparseTensorsExtrapolationCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  SparseTensorsExtrapolationCommandFactory::GetDescription(void) const
  {
    return "Extrapolate a set of tensors on a regular grid using a diffusion equation";
  }
  
} // end namespace itk
