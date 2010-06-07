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
#include "itkWarpTensorImageCommandFactory.h"
#include "itkNPTRemoverCommandFactory.h"
#include "itkTensorFlipCommandFactory.h"
#include "itkAddGaussianNoiseToTensorsCommandFactory.h"
#include "itkTensorAnisotropicFilteringCommandFactory.h"
#include "itkNormalizeTensorsCommandFactory.h"
#include "itkTensorGaussianFilteringCommandFactory.h"
#include "itkSparseTensorsExtrapolationCommandFactory.h"
#include "itkRBFTensorExtrapolationCommandFactory.h"
#include "itkTensorNormalizedGaussianInterpolationCommandFactory.h"
#include "itkWarpFibersCommandFactory.h"
#include "itkTensorGradientMagnitudeCalculatorCommandFactory.h"
#include "itkHessianTensorImageCalculatorCommandFactory.h"
#include "itkResampleTensorImageCommandFactory.h"
#include "itkResampleTensorImage2CommandFactory.h"
#include "itkTensorToScalarCommandFactory.h"
#include "itkTensorsToDWICommandFactory.h"
#include "itkSyntheticTensorFieldGeneratorCommandFactory.h"
#include "itkBoostTensorAnisotropyCommandFactory.h"
#include "itkAverageStdTensorCalculatorCommandFactory.h"
#include "itkNormalize2TensorsCommandFactory.h"

#include "itkCommandObjectFactory.h"

#include "ttkConfigure.h"

#ifdef TTK_USE_MIPS
#include "mipsInrimageImageIOFactory.h"
#endif

int main (int narg, char *args[])
{

  itk::WarpTensorImageCommandFactory::RegisterOneFactory();
  itk::NPTRemoverCommandFactory::RegisterOneFactory();
  itk::TensorFlipCommandFactory::RegisterOneFactory();
  itk::AddGaussianNoiseToTensorsCommandFactory::RegisterOneFactory();
  itk::TensorAnisotropicFilteringCommandFactory::RegisterOneFactory();
  itk::NormalizeTensorsCommandFactory::RegisterOneFactory();
  itk::Normalize2TensorsCommandFactory::RegisterOneFactory();
  itk::TensorGaussianFilteringCommandFactory::RegisterOneFactory();
  itk::SparseTensorsExtrapolationCommandFactory::RegisterOneFactory();
  itk::RBFTensorExtrapolationCommandFactory::RegisterOneFactory();
  itk::TensorNormalizedGaussianInterpolationCommandFactory::RegisterOneFactory();
  itk::WarpFibersCommandFactory::RegisterOneFactory();
  itk::TensorGradientMagnitudeCalculatorCommandFactory::RegisterOneFactory();
  itk::HessianTensorImageCalculatorCommandFactory::RegisterOneFactory();
  itk::ResampleTensorImageCommandFactory::RegisterOneFactory();
  itk::ResampleTensorImage2CommandFactory::RegisterOneFactory();
  itk::TensorToScalarCommandFactory::RegisterOneFactory();
  itk::TensorsToDWICommandFactory::RegisterOneFactory();
  itk::SyntheticTensorFieldGeneratorCommandFactory::RegisterOneFactory();
  itk::BoostTensorAnisotropyCommandFactory::RegisterOneFactory();
  itk::AverageStdTensorCalculatorCommandFactory::RegisterOneFactory();
  
  
  if (narg<2) {
    std::cout << "\n";
    std::cout << "TTK - The Tensor ToolKit - Copyright 2010 - INRIA, France\n";
    std::cout << "\n";
    std::cout << "Author: Pierre Fillard (Pierre.Fillard@inria.fr)\n";
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "Available commands:\n";
    std::cout << "\n";
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
