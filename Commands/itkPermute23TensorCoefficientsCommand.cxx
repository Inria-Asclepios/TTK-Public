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
#include "itkPermute23TensorCoefficientsCommand.h"

#include "itkTensorImageIO.h"
#include "itkPermute23CoefficientsTensorImageFilter.h"

#include <iostream>
#include "GetPot.h"


namespace itk
{

  Permute23TensorCoefficientsCommand::Permute23TensorCoefficientsCommand()
  {
    m_ShortDescription = "Permute 2nd and 3rd coefficients of all tensors";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i input> <-o output>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  Permute23TensorCoefficientsCommand::~Permute23TensorCoefficientsCommand()
  {}

  
  int Permute23TensorCoefficientsCommand::Execute (int narg, const char* arg[])
  {
    
    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
    
    
    const char* input   = cl.follow ("NoFile", 2, "-i", "-I");
    const char* output  = cl.follow ("output.nii.gz", 2, "-o", "-O");
    
    
    typedef double                               ScalarType;  
    typedef itk::TensorImageIO<ScalarType, 3, 3> TensorIOType;
    typedef TensorIOType::TensorImageType        TensorImageType;
    
    TensorImageType::Pointer tensors = 0;
    {
      TensorIOType::Pointer io = TensorIOType::New();
      io->SetFileName (input);
      try
      {
	io->Read();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      
      tensors = io->GetOutput();
    }
    
    
    typedef itk::Permute23CoefficientsTensorImageFilter<TensorImageType, TensorImageType> PermuteType;
    {
      PermuteType::Pointer permute = PermuteType::New();
      permute->SetInput (tensors);
      try
      {
	permute->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }

      tensors = permute->GetOutput();
    }
    
    
    TensorIOType::Pointer io = TensorIOType::New();
    io->SetFileName (output);
    io->SetInput ( tensors );
    try
    {
      io->Write();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    
    return 0;
  }
  
}
