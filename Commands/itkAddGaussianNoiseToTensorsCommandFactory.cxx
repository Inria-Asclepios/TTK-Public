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
#include "itkAddGaussianNoiseToTensorsCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkAddGaussianNoiseToTensorsCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  AddGaussianNoiseToTensorsCommandFactory::AddGaussianNoiseToTensorsCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkAddGaussianNoiseToTensorsCommand",
			   "Add Gaussian noise on the tensor logarithm and exponentiate the result",
			   1,
			   CreateObjectFunction<AddGaussianNoiseToTensorsCommand>::New());
  }
  
  AddGaussianNoiseToTensorsCommandFactory::~AddGaussianNoiseToTensorsCommandFactory()
  {
  }
  
  const char* 
  AddGaussianNoiseToTensorsCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  AddGaussianNoiseToTensorsCommandFactory::GetDescription(void) const
  {
    return "Add Gaussian noise on the tensor logarithm and exponentiate the result";
  }
  
} // end namespace itk
