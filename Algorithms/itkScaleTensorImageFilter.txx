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
#ifndef _itk_ScaleTensorImageFilter_txx_
#define _itk_ScaleTensorImageFilter_txx_

#include "itkScaleTensorImageFilter.h"

#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>


namespace itk
{

  template <class TTensorInput, class TTensorOutput>
  void
  ScaleTensorImageFilter<TTensorInput, TTensorOutput>
  ::ThreadedGenerateData (const OutputRegionType& outputRegion, int threadId)
  {
    
    typedef ImageRegionIterator<OutputImageType>       OutputIteratorType;
    typedef ImageRegionConstIterator<InputImageType>  InputIteratorType;
    
    InputIteratorType itIn(this->GetInput(), outputRegion);
    OutputIteratorType itOut(this->GetOutput(), outputRegion);
    
    while(!itOut.IsAtEnd() )
    {
      InputPixelType T = itIn.Get();	
      if( !T.IsZero() )
      {
	if (m_UseLogScale)
	  T = (T.Log() * m_ScalingFactor).Exp();
	else
	  T *= m_ScalingFactor;
      }

      itOut.Set (T);
      ++itOut;
      ++itIn;
    }
    
  }
  
} // end of namespace


#endif
