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
#ifndef __itkResampleTensorImageFilter_h
#define __itkResampleTensorImageFilter_h

#include "itkFixedArray.h"
#include "itkTensorTransform.h"
#include "itkImageFunction.h"
#include "itkImageRegionIterator.h"
#include "itkImageToImageFilter.h"
#include "itkInterpolateImageFunction.h"
#include "itkSize.h"


namespace itk
{
  
  
  template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType=double>
    class ITK_EXPORT ResampleTensorImageFilter:
    public ImageToImageFilter<TInputImage, TOutputImage>
    {

    public:

    /** Standard class typedefs. */
    typedef ResampleTensorImageFilter         Self;
    typedef ImageToImageFilter<TInputImage,TOutputImage>  Superclass;
    typedef SmartPointer<Self>        Pointer;
    typedef SmartPointer<const Self>  ConstPointer;
    typedef TInputImage  InputImageType;
    typedef TOutputImage OutputImageType;
    typedef typename InputImageType::Pointer InputImagePointer;
    typedef typename InputImageType::ConstPointer InputImageConstPointer;
    typedef typename OutputImageType::Pointer OutputImagePointer;
    typedef typename InputImageType::RegionType InputImageRegionType;
    

    /** Method for creation through the object factory. */
    itkNewMacro(Self);  
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(ResampleTensorImageFilter, ImageToImageFilter);
    
    /** Number of dimensions. */
    itkStaticConstMacro(ImageDimension, unsigned int,
                        TOutputImage::ImageDimension);
    itkStaticConstMacro(InputImageDimension, unsigned int,
                        TInputImage::ImageDimension);
    
    
    /** Transform typedef.
     *
     **/
    typedef TensorTransform<TInterpolatorPrecisionType, 
    itkGetStaticConstMacro(ImageDimension), 
    itkGetStaticConstMacro(ImageDimension)> TransformType;
    typedef typename TransformType::ConstPointer TransformPointerType;
    
    /** Interpolator typedef. */
    typedef InterpolateImageFunction<InputImageType, TInterpolatorPrecisionType> InterpolatorType;
    typedef typename InterpolatorType::Pointer  InterpolatorPointerType;
    
    /** Image size typedef. */
    typedef Size<itkGetStaticConstMacro(ImageDimension)> SizeType;
    
    /** Image index typedef. */
    typedef typename TOutputImage::IndexType IndexType;

    /** Image point typedef. */
    typedef typename InterpolatorType::PointType    PointType;
    //typedef typename TOutputImage::PointType    PointType;
    
    /** Image pixel value typedef. */
    typedef typename TOutputImage::PixelType   PixelType;
    typedef typename TInputImage::PixelType    InputPixelType;
    
    /** Typedef to describe the output image region type. */
    typedef typename TOutputImage::RegionType OutputImageRegionType;
    
    /** Image spacing,origin and direction typedef */
    typedef typename TOutputImage::SpacingType SpacingType;
    typedef typename TOutputImage::PointType   OriginPointType;
    typedef typename TOutputImage::DirectionType DirectionType;
    
    /** Set the coordinate transformation.
     * Set the coordinate transform to use for resampling.  Note that this must
     * be in physical coordinates and it is the output-to-input transform, NOT
     * the input-to-output transform that you might naively expect.  By default
     * the filter uses an Identity transform. You must provide a different
     * transform here, before attempting to run the filter, if you do not want to
     * use the default Identity transform. */
    itkSetConstObjectMacro( TensorTransform, TransformType ); 
    
    /** Get a pointer to the coordinate transform. */
    itkGetConstObjectMacro( TensorTransform, TransformType );
    
    /** Set the interpolator function.  The default is
     * itk::LinearInterpolateImageFunction<InputImageType, TInterpolatorPrecisionType>. Some
     * other options are itk::NearestNeighborInterpolateImageFunction
     * (useful for binary masks and other images with a small number of
     * possible pixel values), and itk::BSplineInterpolateImageFunction
     * (which provides a higher order of interpolation).  */
    itkSetObjectMacro( TensorInterpolator, InterpolatorType );
    
    /** Get a pointer to the interpolator function. */
    itkGetConstObjectMacro( TensorInterpolator, InterpolatorType );
    
    /** Set the size of the output image. */
    itkSetMacro( Size, SizeType );
    
    /** Get the size of the output image. */
    itkGetConstReferenceMacro( Size, SizeType );
    
    /** Set the pixel value when a transformed pixel is outside of the
     * image.  The default default pixel value is 0. */
    itkSetMacro(DefaultPixelValue,PixelType);
    
    /** Get the pixel value when a transformed pixel is outside of the image */
    itkGetMacro(DefaultPixelValue,PixelType);
    
    /** Set the output image spacing. */
    itkSetMacro(OutputSpacing, SpacingType);
    virtual void SetOutputSpacing( const double* values);
    
    /** Get the output image spacing. */
    itkGetConstReferenceMacro( OutputSpacing, SpacingType );
    
    /** Set the output image origin. */
    itkSetMacro(OutputOrigin, OriginPointType);
    virtual void SetOutputOrigin( const double* values);
    
    /** Get the output image origin. */
    itkGetConstReferenceMacro( OutputOrigin, OriginPointType );
    
    /** Set the output direciton cosine matrix. */
    itkSetMacro(OutputDirection, DirectionType);
    itkGetConstReferenceMacro(OutputDirection, DirectionType);
    
    /** Helper method to set the output parameters based on this image */
    void SetOutputParametersFromImage ( typename OutputImageType::Pointer Image )
    {
      this->SetOutputOrigin ( Image->GetOrigin() );
      this->SetOutputSpacing ( Image->GetSpacing() );
	  this->SetOutputDirection ( Image->GetDirection() );
      this->SetSize ( Image->GetLargestPossibleRegion().GetSize() );
    }
    
    /** Set the start index of the output largest possible region. 
     * The default is an index of all zeros. */
    itkSetMacro( OutputStartIndex, IndexType );
    
    /** Get the start index of the output largest possible region. */
    itkGetConstReferenceMacro( OutputStartIndex, IndexType );
    
    /** Copy the output information from another Image.  By default,
     *  the information is specified with the SetOutputSpacing, Origin,
     *  and Direction methods. UseReferenceImage must be On and a
     *  Reference image must be present to override the defaul behavior.*/
    void SetReferenceImage (TOutputImage *image)
    {
      if (image != m_ReferenceImage)
      {
        m_ReferenceImage = image;
        this->ProcessObject::SetNthInput(1, image);
        this->Modified();
      }
    }
    itkGetObjectMacro(ReferenceImage, TOutputImage);
    
    itkSetMacro(UseReferenceImage, bool);
    itkBooleanMacro(UseReferenceImage);
    itkGetMacro(UseReferenceImage, bool);
    
    /** ResampleTensorImageFilter produces an image which is a different size
     * than its input.  As such, it needs to provide an implementation
     * for GenerateOutputInformation() in order to inform the pipeline
     * execution model.  The original documentation of this method is
     * below. \sa ProcessObject::GenerateOutputInformaton() */
    virtual void GenerateOutputInformation();
    
    /** ResampleTensorImageFilter needs a different input requested region than
     * the output requested region.  As such, ResampleTensorImageFilter needs
     * to provide an implementation for GenerateInputRequestedRegion()
     * in order to inform the pipeline execution model.
     * \sa ProcessObject::GenerateInputRequestedRegion() */
    virtual void GenerateInputRequestedRegion();
    
    /** This method is used to set the state of the filter before 
     * multi-threading. */
    virtual void BeforeThreadedGenerateData();
    
    /** This method is used to set the state of the filter after 
     * multi-threading. */
    virtual void AfterThreadedGenerateData();
    
    /** Method Compute the Modified Time based on changed to the components. */
    unsigned long GetMTime( void ) const;
    
    
    protected:
    ResampleTensorImageFilter();
    ~ResampleTensorImageFilter() {};
    void PrintSelf(std::ostream& os, Indent indent) const;
    
    /** ResampleTensorImageFilter can be implemented as a multithreaded filter.  Therefore,
     * this implementation provides a ThreadedGenerateData() routine which
     * is called for each processing thread. The output image data is allocated
     * automatically by the superclass prior to calling ThreadedGenerateData().
     * ThreadedGenerateData can only write to the portion of the output image
     * specified by the parameter "outputRegionForThread"
     * \sa ImageToImageFilter::ThreadedGenerateData(),
     *     ImageToImageFilter::GenerateData() */
    void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                              int threadId );
    
    /** Default implementation for resampling that works for any
     * transformation type. */
    void NonlinearThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                       int threadId );
    
    /** Implementation for resampling that works for with linear 
     * transformation types. */
    void LinearThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                    int threadId );
    
    
    private:
    ResampleTensorImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented
    
    OutputImagePointer      m_ReferenceImage;
    
    SizeType                m_Size;       // Size of the output image
    TransformPointerType    m_TensorTransform;  // Coordinate transform to use
    InterpolatorPointerType m_TensorInterpolator; // Image function for interpolation
    PixelType               m_DefaultPixelValue; // default pixel value if the point 
    // is outside the image
    SpacingType             m_OutputSpacing; // output image spacing
    OriginPointType         m_OutputOrigin;  // output image origin
    DirectionType           m_OutputDirection; // output image direction cosines
    IndexType               m_OutputStartIndex; // output image start index
    bool m_UseReferenceImage;
    
    };
  
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkResampleTensorImageFilter.txx"
#endif

#endif
