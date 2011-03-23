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
#ifndef _itk_BrainExtractionImageFilter_txx_
#define _itk_BrainExtractionImageFilter_txx_

#include "itkBrainExtractionImageFilter.h"

#include <itkImageRegionConstIterator.h>

namespace itk
{

  template <class TInputImage, class TOutputImage>
  BrainExtractionImageFilter<TInputImage, TOutputImage>
  ::BrainExtractionImageFilter()
  {
    m_MaximumNumberOfIterations = 500;
  }

  template <class TInputImage, class TOutputImage>
  BrainExtractionImageFilter<TInputImage, TOutputImage>
  ::~BrainExtractionImageFilter()
  {
  }


  template <class TInputImage, class TOutputImage>
  void
  BrainExtractionImageFilter<TInputImage, TOutputImage>
  ::GenerateData()
  {
    typename GaussianFilterType::Pointer smoother =
      GaussianFilterType::New();
    smoother->SetInput ( this->GetInput() );

    typename InputImageType::Pointer auxImage = const_cast<InputImageType*>(this->GetInput());
    
    for (unsigned int i=0; i<InputImageType::ImageDimension; i++)
    {
      smoother->SetInput ( auxImage );
      smoother->SetDirection (i);
      smoother->SetSigma (1.0);

      smoother->Update();

      auxImage = smoother->GetOutput();
      auxImage->DisconnectPipeline();
    }
    

    InputPixelType threshold = static_cast<InputPixelType>(0.0);

    {
      typename AutomaticImageThresholdCalculatorType::Pointer
	calculator = AutomaticImageThresholdCalculatorType::New();
      calculator->SetImage ( auxImage );
      calculator->Compute();
      
      threshold = calculator->GetThreshold();
    }

    typename OutputImageType::Pointer outputImage    = 0;
    typename OutputImageType::Pointer afterThreshold = 0;
    {
      typename BinaryThresholdFilterType::Pointer thresholder =
	BinaryThresholdFilterType::New();
      thresholder->SetInput (auxImage);
      thresholder->SetLowerThreshold (threshold);
      
      thresholder->Update();
      
      outputImage    = thresholder->GetOutput();
      afterThreshold = thresholder->GetOutput();
      outputImage   ->DisconnectPipeline();
      afterThreshold->DisconnectPipeline();
    }
    
    {
      StructuralElementType ball;
      ball.SetRadius( 3 );

      typename BinaryErodeFilterType::Pointer eroder =
	BinaryErodeFilterType::New();
      eroder->SetInput  ( outputImage );
      eroder->SetKernel ( ball );

      eroder->Update();
      
      outputImage = eroder->GetOutput();
      outputImage->DisconnectPipeline();
    }

    CrossType cross;
    cross.SetRadius( 1 );

    typename OutputImageType::Pointer outputAuxImage = outputImage;
    int iterCount = 0;
    do
    {
      outputImage = outputAuxImage;
      
      typename DilateFilterType::Pointer dilater = DilateFilterType::New();
      dilater->SetInput ( outputImage );
      dilater->SetKernel( cross );
      
      dilater->Update();
      
      typename AndFilterType::Pointer adder = AndFilterType::New();
      adder->SetInput(0, afterThreshold);
      adder->SetInput(1, dilater->GetOutput() );
      
      adder->Update();
      
      outputAuxImage = adder->GetOutput();
      outputAuxImage->DisconnectPipeline();

      ++iterCount;
    }
    while( !CompareImages( outputImage, outputAuxImage) && iterCount<m_MaximumNumberOfIterations);

    outputImage = outputAuxImage;
    
    {
      typename HoleFillingFilterType::Pointer filler = HoleFillingFilterType::New();
      filler->SetInput (outputImage);
      filler->SetMaximumNumberOfIterations (1000);

      filler->Update();

      outputImage = filler->GetOutput();
      outputImage->DisconnectPipeline();
    }

	{
	  cross.SetRadius( 2 );

	  typename DilateFilterType::Pointer dilater = DilateFilterType::New();
	  dilater->SetInput ( outputImage );
	  dilater->SetKernel( cross );
      
      dilater->Update();

	  outputImage = dilater->GetOutput();
      outputImage->DisconnectPipeline();
	}
    
    
    this->GraftOutput ( outputImage );
  }


  template <class TInputImage, class TOutputImage>
  bool
  BrainExtractionImageFilter<TInputImage, TOutputImage>
  ::CompareImages (OutputImageType *image1, OutputImageType *image2)
  {
    ImageRegionConstIterator<OutputImageType> it1 (image1, image1->GetLargestPossibleRegion());
    ImageRegionConstIterator<OutputImageType> it2 (image2, image2->GetLargestPossibleRegion());
    
    while( !it1.IsAtEnd() )
    {
      if( it1.Value() != it2.Value() )
      {
	return false;
      }
      ++it1;
      ++it2;
    }
    
    return true;
  }
  
  
} // end of namespace

#endif
