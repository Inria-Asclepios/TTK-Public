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
#ifndef _itk_GradientTensorImageFilter_h_
#define _itk_GradientTensorImageFilter_h_


#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkCovariantVector.h"
#include "itkConstNeighborhoodIterator.h"


namespace itk
{
  /*! \class GradientTensorImageFilter
    \ingroup TensorToolKit
    Calculate the gradient of an itk tensor image.
   */


  template < class TInputImage, class TOutputValue=double >
    class ITK_EXPORT GradientTensorImageFilter :
  public ImageToImageFilter(TInputImage,
                            Image<
                            CovariantVector<TOutputValueType, ::itk::GetImageDimension<TInputImage>::ImageDimension>,
                            ::itk::GetImageDimension<TInputImage>::ImageDimension
                            >)
  {
    public:
    /** Extract dimension from input image. */
    itkStaticConstMacro(InputImageDimension, unsigned int,
                        TInputImage::ImageDimension);
    itkStaticConstMacro(OutputImageDimension, unsigned int,
                        TInputImage::ImageDimension);
    
    /** Standard class typedefs. */
    typedef GradientTensorImageFilter Self;
    
    /** Convenient typedefs for simplifying declarations. */
    typedef TInputImage                      InputImageType;
    typedef Image<CovariantVector<TOutputValueType, itkGetStaticConstMacro(OutputImageDimension)>,  itkGetStaticConstMacro(OutputImageDimension)> OutputImageType;
    
    /** Standard class typedefs. */
    typedef ImageToImageFilter< InputImageType, OutputImageType> Superclass;
    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self>  ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GradientImageFilter, ImageToImageFilter);
    

    /** Image typedef support. */
    typedef typename InputImageType::PixelType    InputPixelType;
    typedef TOutputValueType                      OutputValueType;
    typedef CovariantVector<OutputValueType,
    itkGetStaticConstMacro(OutputImageDimension)> OutputPixelType;
    typedef typename OutputImageType::RegionType  OutputImageRegionType;


    typedef ConstNeighborhoodIterator<TInputImage> ConstNeighborhoodIteratorType;
    typedef typename ConstNeighborhoodIteratorType::RadiusType RadiusType;
    
    virtual void GenerateInputRequestedRegion() throw(InvalidRequestedRegionError);


    /** Use the image spacing information in calculations. Use this option if you
     *  want derivatives in physical space. Default is UseImageSpacingOn. */
    void SetUseImageSpacingOn()
    { this->SetUseImageSpacing(true); }
    
    /** Ignore the image spacing. Use this option if you want derivatives in
        isotropic pixel space.  Default is UseImageSpacingOn. */
    void SetUseImageSpacingOff()
    { this->SetUseImageSpacing(false); }
    
    /** Set/Get whether or not the filter will use the spacing of the input
        image in its calculations */
    itkSetMacro(UseImageSpacing, bool);
    itkGetMacro(UseImageSpacing, bool);

    
    protected:

    GradientTensorImageFilter()
    {
      m_UseImageSpacing = true;
      for(unsigned int i=0;i<ImageDimension;i++)
        m_NeighborhoodRadius[i]=1;      
    }
    
    virtual ~GradientTensorImageFilter(){};

    void PrintSelf(std::ostream& os, Indent indent) const;
    
    void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

    void FiniteDifferenceCalculation (const ConstNeighborhoodIteratorType &it);
    

    private:

    GradientTensorImageFilter (const Self&);
    void operator=(const Self&);

    bool m_UseImageSpacing;
    RadiusType m_NeighborhoodRadius;
    
  };
  


} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGradientTensorImageFilter.txx"
#endif

#endif
