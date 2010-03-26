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
#include "itkCommandObjectFactory.h"


int main (int narg, char *args[])
{

	itk::HelloWorldCommandFactory::RegisterOneFactory();

	
	if (narg<2) {
		itk::CommandObjectFactory::PrintHelp( std::cout, 0 );
		return EXIT_FAILURE;
	}
	
	
	const char *programName = args[1];
	
	
	itk::CommandObjectBase::Pointer prog = itk::CommandObjectFactory::CreateCommandObject( programName );
	
	int returnValue = EXIT_SUCCESS;
	
	if( !prog.IsNull() )
	{
		std::cout << prog->GetCommandName() << std::endl;
		std::cout << prog->GetShortDescription() << std::endl;
		std::cout << prog->GetLongDescription() << std::endl;
		
		std::cout << "Executing...\n";
		returnValue = prog->Execute(narg, (const char**)args);
		std::cout << "Done." << std::endl;
	}
	else {
		std::cout << "Prog is null" << std::endl;
	}

	return returnValue;

}
