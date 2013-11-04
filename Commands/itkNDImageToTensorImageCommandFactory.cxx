/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: $
  Language:  C++
  Date:      $Date: 2010-08-17 09:58:00 +0200 (mar., 17 août 2010) $
  Version:   $Revision: 97 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkNDImageToTensorImageCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkNDImageToTensorImageCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  NDImageToTensorImageCommandFactory::NDImageToTensorImageCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkNDImageToTensorImageCommand",
			   "Convert a 4D image to a ttk compatible tensor file",
			   1,
			   CreateObjectFunction<NDImageToTensorImageCommand>::New());
  }
  
  NDImageToTensorImageCommandFactory::~NDImageToTensorImageCommandFactory()
  {
  }
  
  const char* 
  NDImageToTensorImageCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  NDImageToTensorImageCommandFactory::GetDescription(void) const
  {
    return "Convert a 4D image to a ttk compatible tensor file";
  }
  
} // end namespace itk
