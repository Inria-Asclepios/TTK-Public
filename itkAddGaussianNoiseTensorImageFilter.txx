#ifndef _itk_AddGaussianNoiseTensorImageFilter_txx_
#define _itk_AddGaussianNoiseTensorImageFilter_txx_

#include "itkAddGaussianNoiseTensorImageFilter.h"

#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>

#include <time.h>

namespace itk
{

  template <class TInputImage, class TOutputImage>
  void
  AddGaussianNoiseTensorImageFilter<TInputImage, TOutputImage>
  ::BeforeThreadedGenerateData()
  {
    m_NormalGenerator->Initialize( (int) clock() );
  }
  

  template <class TInputImage, class TOutputImage>
  void
  AddGaussianNoiseTensorImageFilter<TInputImage, TOutputImage>
  ::ThreadedGenerateData(const OutputRegionType &outputRegion, int threadId)
  {

    typedef ImageRegionIterator<OutputImageType>      OutputIteratorType;
    typedef ImageRegionConstIterator<InputImageType>  InputIteratorType;
                                      
    InputIteratorType itIn(this->GetInput(), outputRegion);
    OutputIteratorType itOut(this->GetOutput(), outputRegion);

    while( !itOut.IsAtEnd() )
    {

      InputPixelType T = itIn.Get();

      if( !T.IsZero() && T.IsFinite())
      {
	T = T.Log();

	double *random_numbers = new double[InputPixelType::NDegreesOfFreedom];
	for( unsigned int i=0; i<InputPixelType::NDegreesOfFreedom; i++)
	{
	  random_numbers[i] = m_NormalGenerator->GetVariate() * sqrt ( m_Variance );
	}


	for( unsigned int i=0; i<InputPixelType::NDegreesOfFreedom; i++)
	{
	  double v = T[i];
	  T.SetNthComponentAsVector ( i, random_numbers[i] );
	  T[i]+=v;
	}
	if (T.IsFinite())
	  T = T.Exp();
	else
	{
	  
	  InputPixelType nullt(0.0);
	  T = nullt;
	}
      }


      itOut.Set (T);

      ++itOut;
      ++itIn;
    }
    
  }
  

  
} // end of namespace


#endif
