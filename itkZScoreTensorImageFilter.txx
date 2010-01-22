#ifndef _itk_ZScoreTensorImageFilter_txx_
#define _itk_ZScoreTensorImageFilter_txx_

#include "itkZScoreTensorImageFilter.h"
#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIterator.h>

#define INF 10.0;

namespace itk
{

template <class TInputImage, class TOutputImage>
void
ZScoreTensorImageFilter<TInputImage, TOutputImage>::
BeforeThreadedGenerateData()
{
	if ( m_CovarianceMatrixImage.IsNull() || m_AverageTensorImage.IsNull() )
	{
		throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Covariance matrix image is not set");
	}
}


template<class TInputImage, class TOutputImage>
void
ZScoreTensorImageFilter<TInputImage,TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, int threadId)
{

	typedef ImageRegionIterator<OutputImageType>      IteratorOutputType;
	typedef ImageRegionConstIterator<CovMatrixImageType>   IteratorCovMatrixType;
    typedef ImageRegionConstIterator<InputImageType>  IteratorInputType;
    
    IteratorOutputType    itOut(this->GetOutput(), outputRegionForThread);
	IteratorCovMatrixType itCovMatrix(this->GetCovarianceMatrixImage(), outputRegionForThread);
	IteratorInputType      itIn(this->GetInput(), outputRegionForThread);
	IteratorInputType      itAverage(this->GetAverageTensorImage(), outputRegionForThread);
	
	if( threadId==0 )
      this->UpdateProgress (0.0);
    
    while(!itOut.IsAtEnd())
    {

      if( this->GetAbortGenerateData() )
	  {
        throw itk::ProcessAborted(__FILE__,__LINE__);
	  }
	  
	  
	  InputPixelType tensor = itIn.Get();
	  InputPixelType average = itAverage.Get();
	  CovMatrixType  covMat = itCovMatrix.Get();
	  
	  OutputPixelType output = static_cast<OutputPixelType>(0.0);
	  
	  if( tensor.IsPositive() && average.IsPositive() && covMat.IsPositive() )
	  {
		
		try
		{
			CovMatrixType invCovMat = covMat.Inv();
			
			VectorType vecAverage = this->Tensor2Vec( average.Log() );
			VectorType vecTensor  = this->Tensor2Vec( tensor.Log() );
			
			VectorType diff = vecTensor - vecAverage;
			
			output = static_cast<OutputPixelType>( sqrt( invCovMat.ScalarProductWith( diff ) ) );
			if (output>1000.0 )
			{
				output = 10.0;
			}
			//std::cout << output << std::endl;			
			
		}
		catch(itk::ExceptionObject &e)
		{
			std::cerr << e;
			throw itk::ExceptionObject(__FILE__,__LINE__,"Error in ZScoreTensorImageFilter::ThreadedGenerateData()");
		}
		
	  }
	  
	  
	  itOut.Set( output );
	  
	  ++itOut;
	  ++itIn;
	  ++itAverage;
	  ++itCovMatrix;
	  
	}
}


template<class TInputImage, class TOutputImage>
  typename ZScoreTensorImageFilter<TInputImage,TOutputImage>::VectorType
  ZScoreTensorImageFilter<TInputImage,TOutputImage>
  ::Tensor2Vec(const InputPixelType &tensor) const
  {
    VectorType vec;
	
	for( unsigned int i=0; i<Self::TensorDegreesOfFreedom; i++)
	{
		vec[i] = tensor.GetNthComponentAsVector(i);
	}
    return vec;
  }
  
 } // end of namespace

#endif
