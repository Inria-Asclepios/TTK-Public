/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: https://scm.gforge.inria.fr/svn/ttk/trunk/Algorithms/itkAddRicianNoiseImageFilter.txx $
  Language:  C++
  Date:      $Date: 2011-01-10 17:45:44 +0000 (Mon, 10 Jan 2011) $
  Version:   $Revision: 130 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_AddRicianNoiseImageFilter_txx_
#define _itk_AddRicianNoiseImageFilter_txx_

#include "itkAddRicianNoiseImageFilter.h"

#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>

#include <time.h>

namespace itk
{

  template <class TInputImage, class TOutputImage>
  void
  AddRicianNoiseImageFilter<TInputImage, TOutputImage>
  ::GenerateData(void)
  {
    typedef ImageRegionIterator<OutputImageType>      OutputIteratorType;
    typedef ImageRegionConstIterator<InputImageType>  InputIteratorType;

    InputIteratorType itIn  (this->GetInput(), this->GetInput()->GetLargestPossibleRegion());
    OutputIteratorType itOut(this->GetOutput(), this->GetOutput()->GetLargestPossibleRegion());
    
    while( !itOut.IsAtEnd() )
    {
      InputPixelType T = itIn.Get();
      if (T != 0)
      {
	double real = this->GetNormalGenerator()->GetVariate() * std::sqrt ( this->GetVariance() );
	double imag = this->GetNormalGenerator()->GetVariate() * std::sqrt ( this->GetVariance() );
	double realplusT = static_cast<double>(T) + real;
	T = static_cast<InputPixelType>( std::sqrt (realplusT*realplusT + imag*imag) );
      }
      
      itOut.Set (T);
      
      ++itOut;
      ++itIn;
    }

    
  }

  
} // end of namespace


#endif
