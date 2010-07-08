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
#include "itkLogDomainDemonsTensorRegistrationCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkLogDomainDemonsTensorRegistrationCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  LogDomainDemonsTensorRegistrationCommandFactory::LogDomainDemonsTensorRegistrationCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkLogDomainDemonsTensorRegistrationCommand",
			   "non-linear, log-domain and symmetric diffeomorphic tensor registration using the demons",
			   1,
			   CreateObjectFunction<LogDomainDemonsTensorRegistrationCommand>::New());
  }
  
  LogDomainDemonsTensorRegistrationCommandFactory::~LogDomainDemonsTensorRegistrationCommandFactory()
  {
  }
  
  const char* 
  LogDomainDemonsTensorRegistrationCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  LogDomainDemonsTensorRegistrationCommandFactory::GetDescription(void) const
  {
    return "Non-linear, log-domain and symmetric diffeomorphic tensor registration using the demons";
  }
  
} // end namespace itk
