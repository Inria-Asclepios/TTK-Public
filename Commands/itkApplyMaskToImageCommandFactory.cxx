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
#include "itkApplyMaskToImageCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkApplyMaskToImageCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  ApplyMaskToImageCommandFactory::ApplyMaskToImageCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkApplyMaskToImageCommand",
			   "Apply a mask to a (tensor) image",
			   1,
			   CreateObjectFunction<ApplyMaskToImageCommand>::New());
  }
  
  ApplyMaskToImageCommandFactory::~ApplyMaskToImageCommandFactory()
  {
  }
  
  const char* 
  ApplyMaskToImageCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  ApplyMaskToImageCommandFactory::GetDescription(void) const
  {
    return "Apply a mask to a (tensor) image";
  }
  
} // end namespace itk
