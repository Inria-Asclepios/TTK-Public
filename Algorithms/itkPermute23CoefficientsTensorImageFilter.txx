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
#ifndef _itkPermute23CoefficientsTensorImageFilter_txx
#define _itkPermute23CoefficientsTensorImageFilter_txx

#include "itkPermute23CoefficientsTensorImageFilter.h"

#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>


namespace itk
{

  template <class TInputImage, class TOutputImage>
  void
  Permute23CoefficientsTensorImageFilter<TInputImage, TOutputImage>
  ::DynamicThreadedGenerateData(const OutputRegionType& outputRegion)
  {

    typedef ImageRegionIterator<OutputImageType>       OutputIteratorType;
    typedef ImageRegionConstIterator<InputImageType>  InputIteratorType;
                                      
    InputIteratorType itIn(this->GetInput(), outputRegion);
    OutputIteratorType itOut(this->GetOutput(), outputRegion);

    while(!itOut.IsAtEnd() )
    {
      
      InputPixelType T = itIn.Get();
      typename InputPixelType::ValueType v = T[2];
      T[2] = T[3];
      T[3] = v;
      itOut.Set (T);
      
      ++itOut;
      ++itIn;
    }
    
  }


} // end of namespace itk

#endif
