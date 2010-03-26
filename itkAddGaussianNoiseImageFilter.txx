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
  ::BeforeThreadedGenerateData()
  {
    m_NormalGenerator->Initialize( (int) clock() );
  }
  

  template <class TInputImage, class TOutputImage>
  void
  AddGaussianNoiseImageFilter<TInputImage, TOutputImage>
  ::ThreadedGenerateData(const OutputRegionType &outputRegion, int threadId)
  {

    typedef ImageRegionIterator<OutputImageType>      OutputIteratorType;
    typedef ImageRegionConstIterator<InputImageType>  InputIteratorType;
                                      
    InputIteratorType itIn(this->GetInput(), outputRegion);
    OutputIteratorType itOut(this->GetOutput(), outputRegion);

    while( !itOut.IsAtEnd() )
    {

      InputPixelType T = itIn.Get();
      if (T != 0)
      {
	
	double random_number = m_NormalGenerator->GetVariate() * sqrt ( m_Variance );
	
	T+=random_number;
      }
      
      itOut.Set (T);
      
      ++itOut;
      ++itIn;
    }
    
  }
  

  
} // end of namespace


#endif
