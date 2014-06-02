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
#include "itkHelloWorldCommand.h"

namespace itk
{

	HelloWorldCommand::HelloWorldCommand()
	{}
	
	HelloWorldCommand::~HelloWorldCommand()
	{}
	
	const char *HelloWorldCommand::GetCommandName()
	{
		return "HelloWorld";
	}
	
	const char *HelloWorldCommand::GetShortDescription() const
	{
		return "This is a short description";
	}
	
	const char *HelloWorldCommand::GetLongDescription() const
	{
		return "This is a long description";
	}
	
	int HelloWorldCommand::Execute (int nargs, const char *args[])
	{
		std::cout << "Hello World!" << std::endl;
		return EXIT_SUCCESS;
	}
	
}
