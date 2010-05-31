#ifndef _itk_GaussianDerivativeImageFilter_txx_
#define _itk_GaussianDerivativeImageFilter_txx_

#include "itkGaussianDerivativeTensorImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"


namespace itk
{


  template<class TInputImage, class TOutputImage>
  void
  GaussianDerivativeTensorImageFilter<TInputImage,TOutputImage>
  ::PrintSelf (std::ostream& os, Indent indent) const
  {
    Superclass::PrintSelf (os,indent);
  }


  template<class TInputImage, class TOutputImage>
  GaussianDerivativeTensorImageFilter<TInputImage,TOutputImage>
  ::GaussianDerivativeTensorImageFilter()
  {
    /*
    m_GaussianFilter = GaussianFilterType::New();
    m_GaussianFilter->SetZeroOrder();
    */
    m_GaussianFilter = GaussianFilterType::New();

    m_DerivativeFilter = DerivativeFilterType::New();
    m_DerivativeFilter->SetOrder(1);
    m_DerivativeFilter->SetUseImageSpacing(1);
    m_DerivativeFilter->SetDirection (0);
  }



  template<class TInputImage, class TOutputImage>
  void
  GaussianDerivativeTensorImageFilter<TInputImage,TOutputImage>
  ::BeforeThreadedGenerateData ()
  {
    m_GaussianFilter->SetInput ( this->GetInput() );

    std::cout << "Gaussian smoothing in progress...";
    try
    {
      m_GaussianFilter->UpdateLargestPossibleRegion();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error in GaussianDerivativeImageFilter::BeforeThreadedGenerateData() when smoothing.");
    }
    std::cout << "Done." << std::endl;
    

    m_DerivativeFilter->SetInput ( m_GaussianFilter->GetOutput() );
    std::cout << "Derivation in progress...";
    try
    {
      m_DerivativeFilter->UpdateLargestPossibleRegion();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error in GaussianDerivativeImageFilter::BeforeThreadedGenerateData() when derivating.");
    }
    std::cout << "Done." << std::endl;
    
  }

  
  template<class TInputImage, class TOutputImage>
  void
  GaussianDerivativeTensorImageFilter<TInputImage,TOutputImage>
  ::ThreadedGenerateData (const OutputImageRegionType& outputRegion, int threadId)
  {

    typedef ImageRegionIterator<OutputImageType>      OutputIteratorType;
    typedef ImageRegionConstIterator<InputImageType>  InputIteratorType;
                                      
    InputIteratorType  itIn  (this->GetInput(), outputRegion);
    OutputIteratorType itOut(this->GetOutput(), outputRegion);
    OutputIteratorType itGaussian    (m_GaussianFilter->GetOutput(), outputRegion);
    OutputIteratorType itGaussianDer (m_DerivativeFilter->GetOutput(), outputRegion);

    while( !itOut.IsAtEnd() )
    {

      vnl_matrix<double> MatRes (3,3,0.0);
      OutputPixelType res (0.0);

    
      
      if( !itIn.Get().IsZero() )
      {
       
        OutputPixelType DX = itGaussianDer.Get();
        OutputPixelType X  = itGaussian.Get();


        vnl_matrix<double> MatRes (3,3,0.0);
        for(unsigned int i=0; i<=1000; i++)
        {
          double alpha = (double)i/1000.0;

          OutputPixelType tmp1 = (X*(1.0-alpha)).Exp();
          OutputPixelType tmp2 = (X*alpha).Exp();

          vnl_matrix<double> mat = (tmp1.GetVnlMatrix())*(DX.GetVnlMatrix())*(tmp2.GetVnlMatrix());
          
          MatRes += mat;
        }
        MatRes *= 0.001;

        /*
        if( threadId==0 )
        {
          std::cout << MatRes << std::endl;
          getchar();
        }
        */

        res.SetVnlMatrix (MatRes);
        
      }

      itOut.Set (res);
      
          
      ++itGaussian;
      ++itGaussianDer;
      ++itIn;
      ++itOut;
    }
    

    
  }
  
  
  
}


#endif
