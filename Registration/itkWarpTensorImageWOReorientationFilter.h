#ifndef __itkWarpTensorImageWOReorientationFilter_h
#define __itkWarpTensorImageWOReorientationFilter_h

#include "itkImageToImageFilter.h"
#include "itkInterpolateImageFunction.h"
#include "itkTensorLinearInterpolateImageFunction.h"
#include "itkWarpJacobianFilter.h"
#include "itkPoint.h"
#include "itkFixedArray.h"

namespace itk
{

template <
  class TInputImage,
  class TOutputImage,
  class TDisplacementField
  >
class ITK_EXPORT WarpTensorImageWOReorientationFilter :
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef WarpTensorImageWOReorientationFilter      Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage> Superclass;
  typedef SmartPointer<Self>         Pointer;
  typedef SmartPointer<const Self>   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro( WarpTensorImageWOReorientationFilter, ImageToImageFilter );

  /** Typedef to describe the output image region type. */
  typedef typename TOutputImage::RegionType OutputImageRegionType;

  /** Inherit some types from the superclass. */
  typedef typename Superclass::InputImageType        InputImageType;
  typedef typename Superclass::InputImagePointer     InputImagePointer;
  typedef typename Superclass::OutputImageType       OutputImageType;
  typedef typename Superclass::OutputImagePointer    OutputImagePointer;
  typedef typename Superclass::InputImageConstPointer InputImageConstPointer;
  typedef typename OutputImageType::IndexType        IndexType;
  typedef typename OutputImageType::SizeType         SizeType;
  typedef typename OutputImageType::PixelType        PixelType;
  typedef typename OutputImageType::SpacingType      SpacingType;
  typedef typename OutputImageType::DirectionType    DirectionType;

  /** Determine the image dimension. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension );
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension );
  itkStaticConstMacro(DisplacementFieldDimension, unsigned int,
                      TDisplacementField::ImageDimension );

  itkStaticConstMacro(VectorDimension, unsigned int,
	  TDisplacementField::PixelType::Dimension);

  typedef Matrix<double, ImageDimension, VectorDimension> JacobianType;
  typedef Image<JacobianType, ImageDimension>             JacobianImageType;
  typedef typename JacobianImageType::Pointer             JacobianPointer;
  typedef WarpJacobianFilter<TDisplacementField, JacobianImageType>  JacobianFilterType;

  itkGetObjectMacro(Jacobian, JacobianImageType);

  /** Displacement field typedef support. */
  typedef TDisplacementField    DisplacementFieldType;
  typedef typename DisplacementFieldType::Pointer  DisplacementFieldPointer;
  typedef typename DisplacementFieldType::PixelType DisplacementType;

  /** Interpolator typedef support. */
  typedef double CoordRepType;
  typedef InterpolateImageFunction<InputImageType,CoordRepType>   InterpolatorType;
  typedef typename InterpolatorType::Pointer   InterpolatorPointer;
  typedef TensorLinearInterpolateImageFunction<InputImageType,CoordRepType>
  DefaultInterpolatorType;

  /** Point type */
  typedef Point<CoordRepType,itkGetStaticConstMacro(ImageDimension)> PointType;

  /** Set the deformation field. */
  void SetDisplacementField( const DisplacementFieldType * field );

  /** Get a pointer the deformation field. */
  DisplacementFieldType * GetDisplacementField(void);

  /** Set the interpolator function. */
  itkSetObjectMacro( Interpolator, InterpolatorType );

  /** Get a pointer to the interpolator function. */
  itkGetObjectMacro( Interpolator, InterpolatorType );

  /** Set the output image spacing. */
  itkSetMacro(OutputSpacing, SpacingType);
  virtual void SetOutputSpacing( const double* values);

  /** Get the output image spacing. */
  itkGetConstReferenceMacro(OutputSpacing, SpacingType);

  /** Set the output image origin. */
  itkSetMacro(OutputOrigin, PointType);
  virtual void SetOutputOrigin( const double* values);

  /** Set the output image direction. */
  itkSetMacro(OutputDirection, DirectionType);

  /** Get the output image direction. */
  itkGetConstReferenceMacro(OutputDirection, DirectionType);

  /** Get the output image origin. */
  itkGetConstReferenceMacro(OutputOrigin, PointType);

  /** Set the edge padding value */
  itkSetMacro( EdgePaddingValue, PixelType );

  /** Get the edge padding value */
  itkGetMacro( EdgePaddingValue, PixelType );

  /** WarpTensorImageWOReorientationFilter produces an image which is a different
   * size than its input image. As such, it needs to provide an
   * implemenation for GenerateOutputInformation() which set
   * the output information according the OutputSpacing, OutputOrigin
   * and the deformation field's LargestPossibleRegion. */
  virtual void GenerateOutputInformation();

  /** It is difficult to compute in advance the input image region
   * required to compute the requested output region. Thus the safest
   * thing to do is to request for the whole input image.
   *
   * For the deformation field, the input requested region
   * set to be the same as that of the output requested region. */
  virtual void GenerateInputRequestedRegion();

  /** This method is used to set the state of the filter before 
   * multi-threading. */
  virtual void BeforeThreadedGenerateData();

  /** This method is used to set the state of the filter after 
   * multi-threading. */
  virtual void AfterThreadedGenerateData();

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(SameDimensionCheck1,
    (Concept::SameDimension<ImageDimension, InputImageDimension>));
  itkConceptMacro(SameDimensionCheck2,
    (Concept::SameDimension<ImageDimension, DisplacementFieldDimension>));
  /** itkConceptMacro(InputHasNumericTraitsCheck,
      (Concept::HasNumericTraits<typename TInputImage::PixelType>)); **/ 
  itkConceptMacro(DisplacementFieldHasNumericTraitsCheck,
    (Concept::HasNumericTraits<typename TDisplacementField::PixelType::ValueType>));
  /** End concept checking */
#endif

protected:
  WarpTensorImageWOReorientationFilter();
  ~WarpTensorImageWOReorientationFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** WarpTensorImageWOReorientationFilter is implemented as a multi-threaded filter.
   * As such, it needs to provide and implementation for 
   * ThreadedGenerateData(). */
  void DynamicThreadedGenerateData(const OutputImageRegionType& outputRegionForThread);

private:
  WarpTensorImageWOReorientationFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  PixelType                  m_EdgePaddingValue;
  SpacingType                m_OutputSpacing;
  PointType                  m_OutputOrigin;
  DirectionType              m_OutputDirection;

  InterpolatorPointer        m_Interpolator;

  typename JacobianImageType::Pointer m_Jacobian;
  
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkWarpTensorImageWOReorientationFilter.txx"
#endif

#endif
