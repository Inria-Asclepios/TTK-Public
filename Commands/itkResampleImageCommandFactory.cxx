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
#include "itkResampleImageCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkResampleImageCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  ResampleImageCommandFactory::ResampleImageCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkResampleImageCommand",
			   "Resample an image given a linear transform and a reference image",
			   1,
			   CreateObjectFunction<ResampleImageCommand>::New());
  }
  
  ResampleImageCommandFactory::~ResampleImageCommandFactory()
  {
  }
  
  const char* 
  ResampleImageCommandFactory::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  ResampleImageCommandFactory::GetDescription() const
  {
    return "Resample an image given a linear transform and a reference image";
  }
  
} // end namespace itk
