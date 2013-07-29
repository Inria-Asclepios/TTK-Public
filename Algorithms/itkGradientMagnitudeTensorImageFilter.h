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
#ifndef _itk_GradientMagnitudeTensorImageFilter_h_
#define _itk_GradientMagnitudeTensorImageFilter_h_


#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkConstNeighborhoodIterator.h"


namespace itk
{
  /*! \class GradientMagnitudeTensorImageFilter
    \ingroup TensorToolKit
    Calculate the gradient magnitude of an itk tensor image with Log-Euclidean metrics.
   */

  template<class TInputImage, class TOutputImage>
    class ITK_EXPORT GradientMagnitudeTensorImageFilter :
  public ImageToImageFilter< TInputImage, TOutputImage >
  {

  public:
    /** Standard class typedefs */
    typedef GradientMagnitudeTensorImageFilter Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkNewMacro (Self);
    itkTypeMacro (GradientMagnitudeTensorImageFilter, ImageToImageFilter);

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


    virtual void GenerateInputRequestedRegion() throw (InvalidRequestedRegionError);

    void SetUseImageSpacingOn()
    { this->SetUseImageSpacing (true); }

    void SetUseImageSpacingOff()
    { this->SetUseImageSpacing (false); }

    itkSetMacro (UseImageSpacing, bool);
    itkGetMacro (UseImageSpacing, bool);       
        
    
  protected:

    GradientMagnitudeTensorImageFilter();
    virtual ~GradientMagnitudeTensorImageFilter(){}

    
    void ThreadedGenerateData (const OutputImageRegionType&, ThreadIdType);
    void PrintSelf (std::ostream&, Indent) const;
        

    OutputPixelType FiniteDifferenceCalculation (const ConstNeighborhoodIteratorType &);
   
    
    
  private:
    GradientMagnitudeTensorImageFilter (const Self&);
    void operator=(const Self&);

    bool          m_UseImageSpacing;        
    RadiusType    m_NeighborhoodRadius;
    
  };
  


} // end of namespace


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGradientMagnitudeTensorImageFilter.txx"
#endif

#endif
