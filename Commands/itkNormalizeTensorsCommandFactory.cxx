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
#include "itkNormalizeTensorsCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkNormalizeTensorsCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  NormalizeTensorsCommandFactory::NormalizeTensorsCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkNormalizeTensorsCommand",
			   "Divide tensors by the largest eigenvalue of the field",
			   1,
			   CreateObjectFunction<NormalizeTensorsCommand>::New());
  }
  
  NormalizeTensorsCommandFactory::~NormalizeTensorsCommandFactory() = default;
  
  const char* 
  NormalizeTensorsCommandFactory::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  NormalizeTensorsCommandFactory::GetDescription() const
  {
    return "Divide tensors by the largest eigenvalue of the field";
  }
  
} // end namespace itk
