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
#include "itkScaleTensorCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkScaleTensorCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  ScaleTensorCommandFactory::ScaleTensorCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkScaleTensorCommand",
			   "Scale tensors by a given factor",
			   1,
			   CreateObjectFunction<ScaleTensorCommand>::New());
  }
  
  ScaleTensorCommandFactory::~ScaleTensorCommandFactory()
  {
  }
  
  const char* 
  ScaleTensorCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  ScaleTensorCommandFactory::GetDescription(void) const
  {
    return "Scale tensors by a given factor";
  }
  
} // end namespace itk
