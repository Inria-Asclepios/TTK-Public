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
#ifndef _itk_AddGaussianNoiseImageFilter_txx_
#define _itk_AddGaussianNoiseImageFilter_txx_

#include "itkAddGaussianNoiseImageFilter.h"

#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>

#include <time.h>

namespace itk
{

  template <class TInputImage, class TOutputImage>
  void
  AddGaussianNoiseImageFilter<TInputImage, TOutputImage>
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
	double random_number = m_NormalGenerator->GetVariate() * std::sqrt ( m_Variance );
	T+=random_number;
      }
      
      itOut.Set (T);
      
      ++itOut;
      ++itIn;
    }

    
  }


  

  template <class TInputImage, class TOutputImage>
  void
  AddGaussianNoiseImageFilter<TInputImage, TOutputImage>
  ::GenerateInputRequestedRegion() throw (InvalidRequestedRegionError)
  {
    Superclass::GenerateInputRequestedRegion();
    if ( this->GetInput() )
    {
      InputImagePointer image = const_cast< InputImageType * >( this->GetInput() );
      image->SetRequestedRegionToLargestPossibleRegion();
    }
  }

  

  template <class TInputImage, class TOutputImage>
  void
  AddGaussianNoiseImageFilter<TInputImage, TOutputImage>
  ::GenerateOutputInformation(void)
  {

    // call the superclass' implementation of this method
    Superclass::GenerateOutputInformation();
    if (!this->GetInput())
      return;

    std::cout<<"generating output information"<<std::endl;
    
    // we need to compute the output spacing, the output origin, the
    // output image size, and the output image start index
    this->GetOutput()->SetRegions( this->GetInput()->GetLargestPossibleRegion() );
    this->GetOutput()->SetDirection (this->GetInput()->GetDirection());
    this->GetOutput()->SetOrigin(this->GetInput()->GetOrigin());
    this->GetOutput()->SetSpacing(this->GetInput()->GetSpacing());    
    this->GetOutput()->Allocate();
  }
  

  
} // end of namespace


#endif
