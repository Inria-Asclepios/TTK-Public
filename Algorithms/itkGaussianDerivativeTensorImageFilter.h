/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL:$
  Language:  C++
  Date:      $Date:$
  Version:   $Revision:$

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_GaussianDerivativeTensorImageFilter_h_
#define _itk_GaussianDerivativeTensorImageFilter_h_


#include <itkImageToImageFilter.h>
#include <itkImage.h>
#include <itkConstNeighborhoodIterator.h>
#include <itkDiscreteGaussianImageFilter.h>
#include <itkDerivativeImageFilter.h>

namespace itk
{


  template <class TInputImage, class TOutputImage>
    class ITK_EXPORT GaussianDerivativeTensorImageFilter :
  public ImageToImageFilter< TInputImage, TOutputImage >
  {


  public:
    typedef GaussianDerivativeTensorImageFilter          Self;
    typedef ImageToImageFilter<TInputImage,TOutputImage> Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkNewMacro  (Self);
    itkTypeMacro (GaussianDerivativeTensorImageFilter, ImageToImageFilter);


    /** Image typedefs */
    typedef typename TOutputImage::PixelType        OutputPixelType;
    typedef typename TInputImage::PixelType         InputPixelType;
    typedef typename InputPixelType::RealValueType  RealType;


    itkStaticConstMacro (ImageDimension, unsigned int,
                         TOutputImage::ImageDimension);

    typedef ConstNeighborhoodIterator<TInputImage> ConstNeighborhoodIteratorType;
    typedef typename ConstNeighborhoodIteratorType::RadiusType RadiusType;
    
    /** Image typedefs support */
    typedef TInputImage InputImageType;
    typedef TOutputImage OutputImageType;

    /** Superclass typedefs */
    typedef typename Superclass::OutputImageRegionType OutputImageRegionType;


    //typedef RecursiveGaussianImageFilter<TInputImage, TOutputImage> GaussianFilterType;
    typedef DerivativeImageFilter<TInputImage, TOutputImage>       DerivativeFilterType;
    typedef DiscreteGaussianImageFilter<TInputImage, TOutputImage> GaussianFilterType;

    void SetSigma (const double sigma)
    {
      m_GaussianFilter->SetVariance (sigma);
    }

    double GetSigma (void) const
    {
      return m_GaussianFilter->GetVariance();
    }
    
    void SetDirection (unsigned int dir)
    {
      m_DerivativeFilter->SetDirection (dir);
    }

    unsigned int GetDirection (void) const
    { return m_DerivativeFilter->GetDirection(); }
    
    
    
  protected:
    GaussianDerivativeTensorImageFilter();
    ~GaussianDerivativeTensorImageFilter(){}

    void BeforeThreadedGenerateData (void);
    void ThreadedGenerateData (const OutputImageRegionType&, int);
    void PrintSelf (std::ostream&, Indent) const;



  private:
    GaussianDerivativeTensorImageFilter (const Self&);
    void operator=(const Self&);

    //typename GaussianFilterType::Pointer m_GaussianFilter;
    typename GaussianFilterType::Pointer   m_GaussianFilter;
    typename DerivativeFilterType::Pointer m_DerivativeFilter;
    
  };
  
}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGaussianDerivativeTensorImageFilter.txx"
#endif

#endif
