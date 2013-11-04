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
#ifndef _itk_GradientMagnitudeTensorImageFilter_txx_
#define _itk_GradientMagnitudeTensorImageFilter_txx_
#include "itkGradientMagnitudeTensorImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkProgressReporter.h"

namespace itk
{


  template<class TInputImage, class TOutputImage>
  void
  GradientMagnitudeTensorImageFilter<TInputImage,TOutputImage>
  ::PrintSelf (std::ostream& os, Indent indent) const
  {
    Superclass::PrintSelf (os,indent);
    os << indent << "UseImageSpacing = " << m_UseImageSpacing << std::endl;
  }



  template<class TInputImage, class TOutputImage>
  GradientMagnitudeTensorImageFilter<TInputImage,TOutputImage>
  ::GradientMagnitudeTensorImageFilter()
  {
    m_UseImageSpacing = true;
    for (unsigned int i = 0; i < ImageDimension; i++)
      m_NeighborhoodRadius[i] = 1; // radius of neighborhood we will use
  }
  
  
  
  template<class TInputImage, class TOutputImage>
  void
  GradientMagnitudeTensorImageFilter<TInputImage,TOutputImage>
  ::GenerateInputRequestedRegion() throw(InvalidRequestedRegionError)
  {
    
    // call the superclass' implementation of this method
    Superclass::GenerateInputRequestedRegion();
  
    // get pointers to the input and output
    typename InputImageType::Pointer  inputPtr = const_cast<InputImageType*>(this->GetInput());
    typename OutputImageType::Pointer outputPtr = this->GetOutput();
    
    if ( !inputPtr || !outputPtr )
      return;

    // get a copy of the input requested region (should equal the output
    // requested region)
    typename InputImageType::RegionType inputRequestedRegion
      = inputPtr->GetRequestedRegion();

    // pad the input requested region by the operator radius
    inputRequestedRegion.PadByRadius(m_NeighborhoodRadius);

    // crop the input requested region at the input's largest possible region
    if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
      inputPtr->SetRequestedRegion( inputRequestedRegion );
      return;
    }
    else
    {
      // Couldn't crop the region (requested region is outside the largest
      // possible region).  Throw an exception.
      
      // store what we tried to request (prior to trying to crop)
      inputPtr->SetRequestedRegion( inputRequestedRegion );
      
      // build an exception
      InvalidRequestedRegionError e(__FILE__, __LINE__);
      std::ostringstream msg;
      msg << static_cast<const char *>(this->GetNameOfClass())
          << "::GenerateInputRequestedRegion()";
      e.SetLocation(msg.str().c_str());
      e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
      e.SetDataObject(inputPtr);
      throw e;
    }
    
  }


  
  template<class TInputImage, class TOutputImage>
  void
  GradientMagnitudeTensorImageFilter<TInputImage,TOutputImage>
  ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, ThreadIdType threadId)
  {
    
    ZeroFluxNeumannBoundaryCondition<TInputImage> nbc;
    ConstNeighborhoodIterator<TInputImage> bit;
    ImageRegionIterator<TOutputImage> it;

    
    typename OutputImageType::Pointer       output = this->GetOutput();
    typename  InputImageType::ConstPointer  input  = this->GetInput();


    // Find the data-set boundary "faces"
    typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::
      FaceListType faceList;
    typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage> bC;
    faceList = bC(input, outputRegionForThread, m_NeighborhoodRadius);
  
    typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::
      FaceListType::iterator fit;
    fit = faceList.begin();
  
    // support progress methods/callbacks
    ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
  

    // Process each of the boundary faces.  These are N-d regions which border
    // the edge of the buffer.
    for (fit=faceList.begin(); fit != faceList.end(); ++fit)
    { 
      bit = ConstNeighborhoodIterator<InputImageType>(m_NeighborhoodRadius,
                                                      input, *fit);
      it = ImageRegionIterator<OutputImageType>(output, *fit);
      bit.OverrideBoundaryCondition(&nbc);
      bit.GoToBegin();

      while ( ! bit.IsAtEnd() )
      {
        it.Set (this->FiniteDifferenceCalculation (bit));
        ++bit;
        ++it;
        progress.CompletedPixel();
      }
    }    
  }


  template<class TInputImage, class TOutputImage>
  typename GradientMagnitudeTensorImageFilter<TInputImage,TOutputImage>::OutputPixelType
  GradientMagnitudeTensorImageFilter<TInputImage,TOutputImage>
  ::FiniteDifferenceCalculation(const ConstNeighborhoodIteratorType &it)
  {

    OutputPixelType sum = NumericTraits<OutputPixelType>::Zero;
        
    for (unsigned int i = 0; i < ImageDimension; ++i)
      {

        if( !it.GetNext (i).IsZero() && !it.GetPrevious (i).IsZero() )
        {
          InputPixelType dx = ( it.GetNext (i) - it.GetPrevious (i) )*0.5;
          if(m_UseImageSpacing)
          {
            dx /= static_cast<RealType>(this->GetInput()->GetSpacing()[i]);
          }
          
          sum += dx.GetSquaredNorm();
        }
        
      }

    return sqrt (sum);
    
  }


} // end of namespace



#endif
