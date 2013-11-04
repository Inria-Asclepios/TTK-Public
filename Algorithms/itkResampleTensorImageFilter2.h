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
#ifndef _itk_ResampleTensorImageFilter_h_
#define _itk_ResampleTensorImageFilter_h_

#include "itkImageToImageFilter.h"

namespace itk
{
  
  template <class TInputImage, class TOutputImage>
    class ITK_EXPORT ResampleTensorImageFilter :
  public ImageToImageFilter<TInputImage,TOutputImage>
  {
    
  public:
    
    typedef ResampleTensorImageFilter                    Self;
    typedef ImageToImageFilter<TInputImage,TOutputImage> Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkNewMacro(Self);
    itkTypeMacro(ResampleTensorImageFilter, ImageToImageFilter);
    itkStaticConstMacro(ImageDimension, unsigned int,
			TOutputImage::ImageDimension);
    
    /** Standard image typedef */
    typedef TInputImage                           InputImageType;
    typedef typename InputImageType::PixelType    InputPixelType;
    typedef TOutputImage                          OutputImageType;
    typedef typename OutputImageType::PixelType   OutputPixelType;
    typedef typename OutputPixelType::ValueType   ScalarType;
    typedef typename OutputImageType::IndexType   IndexType;
    typedef typename OutputImageType::SizeType    SizeType;
    typedef typename OutputImageType::SpacingType SpacingType;
    typedef typename OutputImageType::PointType   PointType;
    typedef typename OutputImageType::RegionType  OutputImageRegionType;
    typedef itk::ContinuousIndex<ScalarType,ImageDimension>    ContinuousIndexType;

   
    
    virtual void GenerateInputRequestedRegion() throw(InvalidRequestedRegionError);
    virtual void GenerateOutputInformation();
    
    /** Set the desired output resolution*/
    itkSetMacro (OutputSize, SizeType);
    itkGetMacro (OutputSize, SizeType);

    /** Set the output spacing. Must be set AFTER calling the SetInput() method.*/
    itkSetMacro(OutputSpacing, SpacingType);
    itkGetMacro(OutputSpacing, SpacingType);

    /** Set the output origin.*/
    itkSetMacro (OutputOrigin, PointType);
    itkGetMacro (OutputOrigin, PointType);
    
    
    
    /*
    void SetOutputSize(const SizeType& size)      
    {
    for(unsigned int i=0; i< ImageDimension; i++)
    m_Size[i] = size[i];
    }    
    void SetOutputSpacing(const SpacingType& spacing)
    {
    for(unsigned int i=0; i<ImageDimension; i++)
    m_Spacing[i] = spacing[i];
    }    
    void SetOutputOrigin(double ox, double oy, double oz)
    {
    m_Origin[0] = ox;
    m_Origin[1] = oy;
    m_Origin[2] = oz;
    }
    */
      
  protected:
    
    ResampleTensorImageFilter()
    {
      m_OutputSize.Fill (0);
      m_OutputSpacing.Fill (1.0);
      m_OutputOrigin.Fill (0.0);
      
    };

    ~ResampleTensorImageFilter(){};
    
    /** Threaded implementation */
    void ThreadedGenerateData(const OutputImageRegionType &,ThreadIdType);
    void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf(os, indent);      
    }
    
    
  private:

    ResampleTensorImageFilter(const Self&);
    void operator=(const Self&);
  
    SizeType    m_OutputSize;
    SpacingType m_OutputSpacing;
    PointType   m_OutputOrigin;
    
  };

} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkResampleTensorImageFilter.txx"
#endif

#endif
