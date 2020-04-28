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
#include "itkTensorGradientMagnitudeCalculatorCommand.h"

#include "itkTensorImageIO.h"
#include <itkImage.h>
#include <itkImageFileWriter.h>
#include <itkVector.h>
#include "itkLogTensorImageFilter.h"
#include "itkTensorImageToVectorImage.h"
#include "itkVectorGradientMagnitudeImageFilter.h"
#include <itkDiscreteGaussianImageFilter.h>
#include <itkImageRegionIteratorWithIndex.h>

#include "GetPot.h"


namespace itk
{

  TensorGradientMagnitudeCalculatorCommand::TensorGradientMagnitudeCalculatorCommand()
  {
    m_ShortDescription = "Calculate the gradient magnitude of a tensor image";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "-i [fileIn]\n";
    m_LongDescription += "-s [sigma]\n";
    m_LongDescription += "-euc\n";
    m_LongDescription += "-o [fileOut]\n\n";
    m_LongDescription += m_ShortDescription;
  }

  TensorGradientMagnitudeCalculatorCommand::~TensorGradientMagnitudeCalculatorCommand()
  {}

  int TensorGradientMagnitudeCalculatorCommand::Execute (int narg, const char* arg[])
  {
      return 0;
  }

}
