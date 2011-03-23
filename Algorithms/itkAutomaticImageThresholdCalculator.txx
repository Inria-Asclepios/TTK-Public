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
#ifndef _itk_AutomaticImageThresholdCalculator_txx_
#define _itk_AutomaticImageThresholdCalculator_txx_

#include "itkAutomaticImageThresholdCalculator.h"

namespace itk
{

  template <class TInputImage>
  AutomaticImageThresholdCalculator<TInputImage>
  ::AutomaticImageThresholdCalculator()
  {
    m_Threshold = static_cast<PixelType>(0.0);
    m_Image = 0;
  }


  template <class TInputImage>
  AutomaticImageThresholdCalculator<TInputImage>
  ::~AutomaticImageThresholdCalculator()
  {
  }

  template <class TInputImage>
  void
  AutomaticImageThresholdCalculator<TInputImage>
  ::Compute()
  {
    if (m_Image.IsNull())
    {
      itkExceptionMacro (<<"Image is not set");
    }
    
    typename MinMaxCalculatorType::Pointer minMaxCalculator = MinMaxCalculatorType::New();
    minMaxCalculator->SetImage ( m_Image );
    minMaxCalculator->Compute();

    PixelType minimum = minMaxCalculator->GetMinimum();
    PixelType maximum = minMaxCalculator->GetMaximum();

    typename HistogramGeneratorType::Pointer histogramGenerator = HistogramGeneratorType::New();
    histogramGenerator->SetInput        ( m_Image );
    histogramGenerator->SetNumberOfBins ( maximum - minimum + 1 );
    histogramGenerator->SetMarginalScale( 1.0 );
    histogramGenerator->SetHistogramMin ( maximum );
    histogramGenerator->SetHistogramMax ( minimum );
    histogramGenerator->Compute();

#ifdef ITK_USE_REVIEW_STATISTICS
    typedef typename HistogramType::AbsoluteFrequencyType FrequencyType;
#else
	typedef typename HistogramType::FrequencyType FrequencyType;
#endif

    typename HistogramType::ConstPointer histogram = histogramGenerator->GetOutput();
    typename HistogramType::ConstIterator it = histogram->Begin();

    double mean = 0.0;
    while (it!=histogram->End())
    {
      mean += static_cast<double>( it.GetFrequency() ) * static_cast<double>( it.GetMeasurementVector()[0] );
      ++it;
    }

    double voxelCount = static_cast<double>( m_Image->GetLargestPossibleRegion().GetNumberOfPixels() );

    if (voxelCount==0.0)
    {
      itkExceptionMacro ( << "Region is empty");
    }
      
    mean /= voxelCount;

    double variance  = 0.0;
    PixelType threshold = static_cast<PixelType>(0.0);

    it = histogram->Begin();
    while (it!=histogram->End())
    {
      double mean0 = 0.0;
      double mean1 = 0.0;

      double voxelCount0 = 0.0;
      double voxelCount1 = 0.0;

      typename HistogramType::ConstIterator it2 = histogram->Begin();
      while (it2!=it)
      {
	voxelCount0 += static_cast<double>( it2.GetFrequency() );
	mean0 += static_cast<double>( it2.GetFrequency() ) * static_cast<double>( it2.GetMeasurementVector()[0] );
	++it2;
      }
      it2 = it;
      while (it2!=histogram->End())
      {
	voxelCount1 += static_cast<double>( it2.GetFrequency() );
	mean1 += static_cast<double>( it2.GetFrequency() ) * static_cast<double>( it2.GetMeasurementVector()[0] );
	++it2;
      }

      if (voxelCount0)
      {
	mean0 /= static_cast<double>(voxelCount0);
      }

      if (voxelCount1)
      {
	mean1 /= static_cast<double>(voxelCount1);
      }

      double diff0 = mean0 - mean;
      double diff1 = mean1 - mean;
      
      double testVariance =  voxelCount0 * diff0 * diff0
	                   + voxelCount1 * diff1 * diff1;

      if (testVariance > variance)
      {
	variance = testVariance;
	threshold = it.GetMeasurementVector()[0];
      }
      
      ++it;
    }

    m_Threshold = threshold;
  }

} // end of namespace

#endif
