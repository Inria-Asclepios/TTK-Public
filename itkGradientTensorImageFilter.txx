#ifndef _itk_GradientTensorImageFilter_txx_
#define _itk_GradientTensorImageFilter_txx_
#include "itkGradientTensorImageFilter.h"



namespace itk
{


  template< class TInputImage, class TOutputImage >
  void
  GradientTensorImageFilter< TInputImage, TOutputImage >
  ::PrintSelf (const ostream& os, Indent indent) const
  {
    Superclass::PrintSelf();
    os << indent << "UseImageSpacing: " << m_UseImageSpacing << std::endl;
    
  }


  template< class TInputImage, class TOutputImage >
  void
  GradientTensorImageFilter< TInputImage, TOutputImage >
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
      OStringStream msg;
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
  GradientTensorImageFilter<TInputImage,TOutputImage>
  ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId)
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

    OutputPixelType G;    
        
    for (unsigned int i = 0; i < ImageDimension; ++i)
      {

        G[i] = ( it.GetNext (i) - it.GetPrevious (i) )*0.5;
        if(m_UseImageSpacing)
          G[i] /= static_cast<RealType>(this->GetInput()->GetSpacing()[i]);
        
      }

    return G;
    
  }
  
} // end of namespace itk


#endif
