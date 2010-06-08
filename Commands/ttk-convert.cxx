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
#include "itkTensorConverterCommandFactory.h"
#include "itkVTKFibersToITKGroupSpatialObjectCommandFactory.h"
#include "itkTensorsToVTKUnstructuredGridCommandFactory.h"
#include "itkImageConverterCommandFactory.h"
#include "itkConsolidateFiberBundleCommandFactory.h"

#include "itkCommandObjectFactory.h"

#include "ttkConfigure.h"

#ifdef TTK_USE_MIPS
#include "mipsInrimageImageIOFactory.h"
#endif

int main (int narg, char *args[])
{

  itk::TensorConverterCommandFactory::RegisterOneFactory();
  itk::VTKFibersToITKGroupSpatialObjectCommandFactory::RegisterOneFactory();
  itk::TensorsToVTKUnstructuredGridCommandFactory::RegisterOneFactory();
  itk::ImageConverterCommandFactory::RegisterOneFactory();
  itk::ConsolidateFiberBundleCommandFactory::RegisterOneFactory();

  if (narg<2) {
    std::cout << "Software Tensor ToolKit (c)INRIA 2010, version 1.0.0\n";
    std::cout << "\n";
    std::cout << "Author: Pierre Fillard (Pierre.Fillard@inria.fr)\n";
    std::cout << "\n";
    std::cout << "INRIA freely grants the non-exclusive right to use the Software for RESEARCH\n"
                 "PURPOSES ONLY. Every user of the Software will communicate to the Asclepios\n"
                 "team (pierre.fillard@inria.fr) their remarks as to the use of the Software.\n"
                 "Please note that the Software implements the following patent: \"Improved\n"
                 "device for processing raw images or tensor images\", Arsigny V., Pennec X.,\n"
				 "Fillard P., Ayache N., 2006.04.07, EP1866866, WO2006106242.\n\n"
                 "THUS, NO COMMERCIAL USE, EXPLOITATION NOR DISTRIBUTION IS AUTHORIZED BY THIS\n"
                 "LICENSE. IF SUCH USE IS DESIRED, PLEASE CONTACT THE TECH TRANSFER OFFICE\n"
				 "(rev-ppi@sophia.inria.fr).\n\n";
    std::cout << "Available commands:\n";
    itk::CommandObjectFactory::PrintHelp( std::cout, 0 );
    return EXIT_FAILURE;
  }


#ifdef TTK_USE_MIPS
    itk::InrimageImageIOFactory::RegisterOneFactory();
#endif
  
	
  const char *programName = args[1];
  
  
  itk::CommandObjectBase::Pointer prog = itk::CommandObjectFactory::CreateCommandObject( programName );
  
  int returnValue = EXIT_SUCCESS;
  
  if( !prog.IsNull() )
  {
    returnValue = prog->Execute(narg-1, (const char**)args+1);
  }
  else {
    std::cout << "Command not found" << std::endl;
  }
  
  return returnValue;
  
}
