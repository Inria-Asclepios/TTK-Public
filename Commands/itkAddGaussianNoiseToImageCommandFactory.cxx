/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: https://scm.gforge.inria.fr/svn/ttk/trunk/Commands/itkAddGaussianNoiseToImageCommandFactory.cxx $
  Language:  C++
  Date:      $Date: 2010-12-22 10:25:59 +0000 (Wed, 22 Dec 2010) $
  Version:   $Revision: 124 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkAddGaussianNoiseToImageCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkAddGaussianNoiseToImageCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  AddGaussianNoiseToImageCommandFactory::AddGaussianNoiseToImageCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkAddGaussianNoiseToImageCommand",
			   "Add Gaussian noise on an input scalar image",
			   1,
			   CreateObjectFunction<AddGaussianNoiseToImageCommand>::New());
  }
  
  AddGaussianNoiseToImageCommandFactory::~AddGaussianNoiseToImageCommandFactory()
  {
  }
  
  const char* 
  AddGaussianNoiseToImageCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  AddGaussianNoiseToImageCommandFactory::GetDescription(void) const
  {
    return "Add Gaussian noise on an input scalar image";
  }
  
} // end namespace itk
