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
#include "itkBoostTensorAnisotropyCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkBoostTensorAnisotropyCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  BoostTensorAnisotropyCommandFactory::BoostTensorAnisotropyCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkBoostTensorAnisotropyCommand",
			   "Boost the tensor anisotropy (FA)",
			   1,
			   CreateObjectFunction<BoostTensorAnisotropyCommand>::New());
  }
  
  BoostTensorAnisotropyCommandFactory::~BoostTensorAnisotropyCommandFactory()
  {
  }
  
  const char* 
  BoostTensorAnisotropyCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  BoostTensorAnisotropyCommandFactory::GetDescription(void) const
  {
    return "Boost the tensor anisotropy (FA)";
  }
  
} // end namespace itk
