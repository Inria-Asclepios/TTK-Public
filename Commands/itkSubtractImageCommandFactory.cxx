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
#include "itkSubtractImageCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkSubtractImageCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  SubtractImageCommandFactory::SubtractImageCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkSubtractImageCommand",
			   "Subtract two images",
			   1,
			   CreateObjectFunction<SubtractImageCommand>::New());
  }
  
  SubtractImageCommandFactory::~SubtractImageCommandFactory()
  {
  }
  
  const char* 
  SubtractImageCommandFactory::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  SubtractImageCommandFactory::GetDescription() const
  {
    return "Subtract two images";
  }
  
} // end namespace itk
