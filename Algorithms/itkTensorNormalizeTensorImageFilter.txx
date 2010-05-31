#ifndef _itk_TensorNormalizeTensorImageFilter_txx_
#define _itk_TensorNormalizeTensorImageFilter_txx_

#include "itkTensorNormalizeTensorImageFilter.h"

#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>


namespace itk
{

  template <class TTensorInput, class TTensorOutput>
  void
  TensorNormalizeTensorImageFilter<TTensorInput, TTensorOutput>
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
	T /= m_NormalizationFactor;
      }

      itOut.Set (T);
      ++itOut;
      ++itIn;
    }
    
  }
  
} // end of namespace


#endif
