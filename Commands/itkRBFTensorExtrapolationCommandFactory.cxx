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
#include "itkRBFTensorExtrapolationCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkRBFTensorExtrapolationCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  RBFTensorExtrapolationCommandFactory::RBFTensorExtrapolationCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkRBFTensorExtrapolationCommand",
			   "Extrapolate a set of tensors using radial basis functions (RBF)",
			   1,
			   CreateObjectFunction<RBFTensorExtrapolationCommand>::New());
  }
  
  RBFTensorExtrapolationCommandFactory::~RBFTensorExtrapolationCommandFactory()
  {
  }
  
  const char* 
  RBFTensorExtrapolationCommandFactory::GetITKSourceVersion() const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  RBFTensorExtrapolationCommandFactory::GetDescription() const
  {
    return "Extrapolate a set of tensors using radial basis functions (RBF)";
  }
  
} // end namespace itk
