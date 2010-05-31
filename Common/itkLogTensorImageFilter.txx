#ifndef _itkLogTensorImageFilter_txx
#define _itkLogTensorImageFilter_txx

#include "itkLogTensorImageFilter.h"

#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>


namespace itk
{

  template <class TInputImage, class TOutputImage>
  void
  LogTensorImageFilter<TInputImage, TOutputImage>
  ::ThreadedGenerateData(const OutputRegionType& outputRegion, int threadId)
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
	try
	{
	  T = T.Log();
	}
	catch( itk::ExceptionObject &)
	{
	  throw itk::ExceptionObject(__FILE__,__LINE__,"Error in LogTensorImageFilter::ThreadedGenerateData()");
	}
      }      
      itOut.Set (T);
      
      ++itOut;
      ++itIn;
    }
    
  }


} // end of namespace itk

#endif
