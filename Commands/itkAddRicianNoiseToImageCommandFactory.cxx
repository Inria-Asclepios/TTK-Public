/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: https://scm.gforge.inria.fr/svn/ttk/trunk/Commands/itkAddRicianNoiseToImageCommandFactory.cxx $
  Language:  C++
  Date:      $Date: 2010-12-22 10:25:59 +0000 (Wed, 22 Dec 2010) $
  Version:   $Revision: 124 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkAddRicianNoiseToImageCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkAddRicianNoiseToImageCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  AddRicianNoiseToImageCommandFactory::AddRicianNoiseToImageCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkAddRicianNoiseToImageCommand",
			   "Add Rician noise on an input scalar image",
			   1,
			   CreateObjectFunction<AddRicianNoiseToImageCommand>::New());
  }
  
  AddRicianNoiseToImageCommandFactory::~AddRicianNoiseToImageCommandFactory()
  {
  }
  
  const char* 
  AddRicianNoiseToImageCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  AddRicianNoiseToImageCommandFactory::GetDescription(void) const
  {
    return "Add Rician noise on an input scalar image";
  }
  
} // end namespace itk
