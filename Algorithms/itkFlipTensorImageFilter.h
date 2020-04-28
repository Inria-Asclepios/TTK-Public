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
#ifndef _itk_FlipTensorImageFilter_h_
#define _itk_FlipTensorImageFilter_h_

#include "itkImageToImageFilter.h"
#include "itkInPlaceImageFilter.h"
#include "itkConstNeighborhoodIterator.h"

namespace itk
{
  /*! \class FlipTensorImageFilter
    \ingroup TensorToolKit
    Flip a tensor among the x/y/z axis.
   */

  template <class TInputImage, class TOutputImage>
    class ITK_EXPORT FlipTensorImageFilter :
  //public ImageToImageFilter<TInputImage, TOutputImage>
  public InPlaceImageFilter<TInputImage, TOutputImage>
  {

  public:

    typedef FlipTensorImageFilter     Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer<Self>        Pointer;
    typedef SmartPointer< const Self> ConstPointer;

    itkNewMacro (Self);
    itkTypeMacro (FlipTensorImageFilter,
                  ImageToImageFilter);
    
    itkStaticConstMacro(ImageDimension, unsigned int,
			TOutputImage::ImageDimension);



    /** Image typedefs */
    typedef typename TOutputImage::PixelType        OutputPixelType;
    typedef typename TInputImage::PixelType         InputPixelType;
    typedef typename InputPixelType::ValueType      ScalarType;

    /** Image typedefs support */
    typedef TInputImage  InputImageType;
    typedef TOutputImage OutputImageType;    

    typedef ConstNeighborhoodIterator<TInputImage> ConstNeighborhoodIteratorType;
    typedef typename ConstNeighborhoodIteratorType::RadiusType RadiusType;
   
    /** Superclass typedefs */
    typedef typename Superclass::OutputImageRegionType OutputImageRegionType;

    /** Set (to 1) here the axes to be flipped 
        Note that axes array size must be ImageDimension */
    void SetFlipAxes (const bool* axes)
    { for (unsigned int i=0;i<ImageDimension;i++)
        m_FlipAxes[i] = axes[i];
        this->Modified();
    }
    /** Set here weither an axis has
        to be flipped or not */
    void SetFlipAxis (int id, bool val)
    {
      if (id<0 || id>=(int)ImageDimension)
        return;
      m_FlipAxes[id] = val;
      this->Modified();
    }
    /** Get the axis flip states */
    bool* GetFlipAxes (void) const
    { return m_FlipAxes; }
    /** Get an axis flip state */
    bool GetFlipAxis (int id)
    {
      if (id<0 || id>=(int)ImageDimension)
        return false;
      return m_FlipAxes[id];
    }
    
  protected:

    FlipTensorImageFilter();
    ~FlipTensorImageFilter();

    void DynamicThreadedGenerateData (const OutputImageRegionType&);
    void PrintSelf (std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf (os, indent);      
    }
    
    

  private:

    FlipTensorImageFilter (const Self&);
    void          operator=(const Self&);

    /// The axes to be flipped
    /// 0: Axis not flipped
    /// 1: Axis flipped
    bool*      m_FlipAxes;
    
    
    RadiusType m_NeighborhoodRadius;
    
    
  };
  
  
      

} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFlipTensorImageFilter.txx"
#endif

#endif
