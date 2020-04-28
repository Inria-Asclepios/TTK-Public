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
#ifndef _itk_RemoveNonPositiveTensorsTensorImageFilter_txx_
#define _itk_RemoveNonPositiveTensorsTensorImageFilter_txx_
#include "itkRemoveNonPositiveTensorsTensorImageFilter.h"

#include <itkNeighborhoodAlgorithm.h>

namespace itk
{

  template <class TInputImage, class TOutputImage>
  RemoveNonPositiveTensorsTensorImageFilter<TInputImage, TOutputImage>
  ::RemoveNonPositiveTensorsTensorImageFilter()
  {
    m_UseImageSpacing = true;
    for( unsigned int i=0; i<ImageDimension; i++ )
      m_NeighborhoodRadius[i] = 1;

    m_RemovingType = 0;

    this->InPlaceOff();
  }


  template<class TInputImage, class TOutputImage>
  void
  RemoveNonPositiveTensorsTensorImageFilter<TInputImage,TOutputImage>
  ::GenerateInputRequestedRegion() noexcept(false)
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
  RemoveNonPositiveTensorsTensorImageFilter<TInputImage,TOutputImage>
  ::DynamicThreadedGenerateData(const OutputImageRegionType& outputRegionForThread)
  {
    
    ZeroFluxNeumannBoundaryCondition<TInputImage> nbc;
    ConstNeighborhoodIterator<TInputImage> bit;
    ImageRegionIterator<TOutputImage> it;
    
    
    typename OutputImageType::Pointer       output = this->GetOutput();
    typename InputImageType::ConstPointer   input  = this->GetInput();

    unsigned long numPixels = outputRegionForThread.GetNumberOfPixels();
    unsigned long step = numPixels/100;
    unsigned long progress = 0;

    // Find the data-set boundary "faces"
    typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::
      FaceListType faceList;
    typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage> bC;
    faceList = bC(input, outputRegionForThread, m_NeighborhoodRadius);
  
    typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::FaceListType::iterator fit;
    fit = faceList.begin();
    
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

        if( this->GetAbortGenerateData() )
          throw itk::ProcessAborted(__FILE__,__LINE__);

        InputPixelType T = bit.GetCenterPixel();
        InputPixelType out = T;
        
        if ( !T.IsZero() && (!T.IsFinite() || !T.IsPositive()) )
        {

          InputPixelType Mean (static_cast<ScalarType>( 0.0 ));
          ScalarType sum = static_cast<ScalarType>( 0.0 );

          switch ( m_RemovingType )
          {

              case 0:

                out = Mean;
                
                break;


              case 1:
          
                for( unsigned int i=0; i<ImageDimension; i++)
                {
                  
                  InputPixelType Tn = bit.GetNext (i);
                  InputPixelType Tmn = bit.GetPrevious (i);
                  
                  if( !Tn.IsZero() && Tn.IsPositive() && Tn.IsFinite() )
                  {
                    Mean += Tn.Log() * input->GetSpacing()[i];
                    sum += input->GetSpacing()[i];
                  }
                  
                  if( !Tmn.IsZero() &&  Tmn.IsPositive() && Tmn.IsFinite() )
                  {
                    Mean += Tmn.Log() * input->GetSpacing()[i];
                    sum += input->GetSpacing()[i];
                  }            
                  
                }
                
                if (sum <= static_cast<ScalarType>( 0.0 ) )
                  out = static_cast<ScalarType>( 0.0 );
                else
                {
                  Mean /= sum;
                  if ( !Mean.IsZero() && Mean.IsPositive() && Mean.IsFinite() )
                    out = Mean.Exp();
                  else
                    out = static_cast<ScalarType>( 0.0 );
                }
                break;


              default:
                throw itk::ExceptionObject (__FILE__,__LINE__,"Error: Removing type is unknown.");
                
          }
          
        }

        it.Set ( out );
        ++bit;
        ++it;
        ++progress;
      }
      
    }
    
  }
  
  
  

} // end of namespace itk


#endif
