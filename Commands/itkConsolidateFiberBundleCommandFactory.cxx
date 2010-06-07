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
#include "itkConsolidateFiberBundleCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkConsolidateFiberBundleCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  ConsolidateFiberBundleCommandFactory::ConsolidateFiberBundleCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkConsolidateFiberBundleCommand",
			   "Consolidate a fiber bundle by retrieving point data from all fibers",
			   1,
			   CreateObjectFunction<ConsolidateFiberBundleCommand>::New());
  }
  
  ConsolidateFiberBundleCommandFactory::~ConsolidateFiberBundleCommandFactory()
  {
  }
  
  const char* 
  ConsolidateFiberBundleCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  ConsolidateFiberBundleCommandFactory::GetDescription(void) const
  {
    return "Consolidate a fiber bundle by retrieving point data from all fibers";
  }
  
} // end namespace itk
