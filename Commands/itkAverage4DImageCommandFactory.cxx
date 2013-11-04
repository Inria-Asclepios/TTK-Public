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
#include "itkAverage4DImageCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkAverage4DImageCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  Average4DImageCommandFactory::Average4DImageCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkAverage4DImageCommand",
			   "Average a 4D image",
			   1,
			   CreateObjectFunction<Average4DImageCommand>::New());
  }
  
  Average4DImageCommandFactory::~Average4DImageCommandFactory()
  {
  }
  
  const char* 
  Average4DImageCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  Average4DImageCommandFactory::GetDescription(void) const
  {
    return "Subtract two images";
  }
  
} // end namespace itk
