#ifndef _itkTensorImageFilter_txx
#define _itkTensorImageFilter_txx

#include "itkExpTensorImageFilter.h"

#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>


namespace itk
{

  template <class TInputImage, class TOutputImage>
  void
  ExpTensorImageFilter<TInputImage, TOutputImage>
  ::ThreadedGenerateData(const OutputRegionType& outputRegion, int threadId)
  {

    typedef ImageRegionIterator<OutputImageType>       OutputIteratorType;
    typedef ImageRegionConstIterator<InputImageType>   InputIteratorType;

    InputIteratorType itIn(this->GetInput(), outputRegion);
    OutputIteratorType itOut(this->GetOutput(), outputRegion);

    while( !itOut.IsAtEnd() )
    {
	
	InputPixelType T = itIn.Get();	
	if( !T.IsZero() )
	  itOut.Set (T.Exp());
        else
          itOut.Set(T);
        
	++itOut;
	++itIn;
    }
    
    
  }
  

} // end of namespace itk



#endif
