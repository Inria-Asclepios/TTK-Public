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
#include "itkTensorGaussianFilteringCommand.h"

#include "itkTensorImageIO.h"
#include "itkLogTensorImageFilter.h"
#include "itkExpTensorImageFilter.h"
#include <itkImage.h>
#include <itkImageFileWriter.h>
#include <itkDiscreteGaussianImageFilter.h>
#include <itkImageRegionIteratorWithIndex.h>

#include <iostream>
#include "GetPot.h"


namespace itk
{


  TensorGaussianFilteringCommand::TensorGaussianFilteringCommand()
  {
    m_ShortDescription = "Log-Eculidean Gaussian filtering of tensors";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i input> <-s sigma> <-euc> <-o output>\n\n";
    m_LongDescription += m_ShortDescription;
  }


  TensorGaussianFilteringCommand::~TensorGaussianFilteringCommand()
  {}


  int TensorGaussianFilteringCommand::Execute (int narg, const char* arg[])
  {
    return 0;
  }

}
