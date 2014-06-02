/*=========================================================================

  Program:   ITK Program Factory
  Module:    $RCSfile: $
  Language:  C++
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) INRIA Saclay Île-de-France, Parietal Research Team. All rights reserved.
  See CodeCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkHelloWorldCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkHelloWorldCommand.h"
#include "itkVersion.h"

namespace itk
{
	
	HelloWorldCommandFactory::HelloWorldCommandFactory()
	{
		this->RegisterOverride("itkCommandObjectBase",
							   "itkHelloWorldCommand",
							   "Hello World Command",
							   1,
							   CreateObjectFunction<HelloWorldCommand>::New());
	}
	
	HelloWorldCommandFactory::~HelloWorldCommandFactory()
	{
	}
	
	const char* 
	HelloWorldCommandFactory::GetITKSourceVersion(void) const
	{
		return ITK_SOURCE_VERSION;
	}
	
	const char* 
	HelloWorldCommandFactory::GetDescription(void) const
	{
		return "Pouet pouet pouet";
	}
	
} // end namespace itk
