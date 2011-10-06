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
#ifndef _itk_GradientTensorImageFilter_txx_
#define _itk_GradientTensorImageFilter_txx_
#include "itkGradientTensorImageFilter.h"



namespace itk
{

  template< class TInputImage, class TOutputValue >
  void
  GradientTensorImageFilter< TInputImage, TOutputValue >
  ::GenerateInputRequestedRegion() throw(InvalidRequestedRegionError)
  {
    
    // call the superclass' implementation of this method
    Superclass::GenerateInputRequestedRegion();
    
    // get pointers to the input and output
    typename InputImageType::Pointer  inputPtr = const_cast<InputImageType*>(this->GetInput());
    typename OutputImageType::Pointer outputPtr = this->GetOutput();

    typename InputImageType::SpacingType spacing = this->GetInput()->GetSpacing();
    std::cout<<"gradient computation with sp : "<<spacing<<std::endl;
    

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
      OStringStream msg;
      msg << static_cast<const char *>(this->GetNameOfClass())
          << "::GenerateInputRequestedRegion()";
      e.SetLocation(msg.str().c_str());
      e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
      e.SetDataObject(inputPtr);
      throw e;
    }
  }



  template<class TInputImage, class TOutputValue>
  void
  GradientTensorImageFilter<TInputImage,TOutputValue>
  ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId)
  {
    
    ZeroFluxNeumannBoundaryCondition<TInputImage> nbc;
    ConstNeighborhoodIterator<TInputImage> bit;
    ImageRegionIterator<OutputImageType> it;

    
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
  


  // template<class TInputImage, class TOutputValue>
  // void
  // GradientTensorImageFilter<TInputImage,TOutputValue>
  // ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId)
  // {

  //   typename OutputImageType::SizeType radius;
  //   radius[0] = radius[1] = radius[2] = 1;
    
  //   ConstNeighborhoodIterator<InputImageType>  itIn (radius, this->GetInput(),  outputRegionForThread);
  //   ImageRegionIterator<OutputImageType>      itOut (this->GetOutput(), outputRegionForThread);
  //   typename OutputImageType::SpacingType spacing = this->GetInput()->GetSpacing();
    
  //   while (!itIn.IsAtEnd())
  //   {
  //     OutputPixelType G (0.0), gradient (0.0);
  //     InputPixelType L = itIn.GetCenterPixel();
      
  //     if (L.GetTrace() > 0.0)
  //     {
  // 	G = L.GetEigenvector(2);
	
  //     	// InputPixelType gradient (0.0);
  //     	// for(unsigned int i=0; i< InputImageType::ImageDimension; i++)
  //     	// {
  //     	//   InputPixelType Ln = itIn.GetNext (i);
  //     	//   InputPixelType Lmn = itIn.GetPrevious (i);
	  
  //     	//   bool isNzero = Ln.IsZero();
  //     	//   bool isMNzero = Lmn.IsZero();
	  
  //     	//   if( !isNzero || !isMNzero )
  //     	//   {
  //     	//     // neuman conditions
  //     	//     if(isNzero && !isMNzero)
  //     	//       Ln = Lmn;
  //     	//     if(isMNzero && !isNzero) 
  //     	//       Lmn = Ln;
  //     	//   }
	  
  //     	//   // gradient += ( Ln - Lmn ) / static_cast<double>( 2 * spacing[i]*spacing[i] );
  //     	//   OutputPixelType V1 = Ln.GetEigenvector (2);
  //     	//   OutputPixelType V2 = Lmn.GetEigenvector (2);
  //     	//   G += (V1-V2);
  // 	  // G /= static_cast<OutputValueType>( 2.0 * spacing[i]*spacing[i] );
  //     	// }
  //     }

  //     itOut.SetIndex (itIn.GetIndex());
  //     // itOut.Set (gradient);
  //     itOut.Set (G);
  //     ++itIn;
  //   }
    
  // }
  
  
  template<class TInputImage, class TOutputValue>
  typename GradientTensorImageFilter<TInputImage,TOutputValue>::OutputPixelType
  GradientTensorImageFilter<TInputImage,TOutputValue>
  ::FiniteDifferenceCalculation(const ConstNeighborhoodIteratorType &it)
  {

    typename InputImageType::SpacingType spacing = this->GetInput()->GetSpacing();
    
    InputPixelType L = it.GetCenterPixel();
    OutputPixelType G (0.0), gradient (0.0);
    if (!L.IsZero())
    {
      
      for (unsigned int i = 0; i <InputImageType::ImageDimension; ++i)
      {
      
        InputPixelType Ln = it.GetNext (i);
        InputPixelType Lmn = it.GetPrevious (i);
      
        bool isNzero = Ln.IsZero();
        bool isMNzero = Lmn.IsZero();
      
        if( !isNzero || !isMNzero )
        {
	  // neuman conditions
	  if(isNzero && !isMNzero)
	    Ln = Lmn;
	  if(isMNzero && !isNzero) 
	    Lmn = Ln;
        }

	InputPixelType T = ( Ln - Lmn ) / static_cast<OutputValueType>( 2.0 * spacing[i]);

	if (Ln.GetNorm() > Lmn.GetNorm())
	  gradient[i] = T.GetNorm();
	else
	  gradient[i] = -T.GetNorm();
      }
      
    }
    
    return gradient;
    
  }
  
} // end of namespace itk


#endif
