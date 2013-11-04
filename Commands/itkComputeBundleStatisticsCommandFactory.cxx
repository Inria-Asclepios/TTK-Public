/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: https://scm.gforge.inria.fr/svn/ttk/trunk/Commands/itkComputeBundleStatisticsCommandFactory.cxx $
  Language:  C++
  Date:      $Date: 2010-12-22 11:25:59 +0100 (mer., 22 déc. 2010) $
  Version:   $Revision: 124 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkComputeBundleStatisticsCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkComputeBundleStatisticsCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  ComputeBundleStatisticsCommandFactory::ComputeBundleStatisticsCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkComputeBundleStatisticsCommand",
			   "Compute mean FA/ADC of a fiber bundle",
			   1,
			   CreateObjectFunction<ComputeBundleStatisticsCommand>::New());
  }
  
  ComputeBundleStatisticsCommandFactory::~ComputeBundleStatisticsCommandFactory()
  {
  }
  
  const char* 
  ComputeBundleStatisticsCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  ComputeBundleStatisticsCommandFactory::GetDescription(void) const
  {
    return "Compute mean FA/ADC of a fiber bundle";
  }
  
} // end namespace itk
