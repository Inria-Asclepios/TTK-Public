#pragma once

#include <itkImageToImageFilter.h>
#include <itkContinuousIndex.h>
#include <itkAffineTransform.h>
#include <itkAffineTensorTransform.h>
#include "itkTensorLinearInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkLogTensorImageFilter.h"
#include "itkCrossHelper.h"

namespace itk
{

  /*! \class SheetTrackingImageFilter
    \ingroup TensorToolKit
    
    Description: Track sheetss in a tensor image based
    on the tensorline algorithm proposed by Weinstein.
    This filter is multithreaded.
           
  */

  template <class TInputImage, class TOutputImage>
    class ITK_EXPORT SheetTrackingImageFilter:
  public ImageToImageFilter <TInputImage, TOutputImage>
  {

  public:

    typedef SheetTrackingImageFilter                      Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    itkNewMacro (Self);
    itkTypeMacro (SheetTrackingImageFilter,
                  ImageToImageFilter);
    itkStaticConstMacro (ImageDimension, unsigned int,
                         TOutputImage::ImageDimension);

    //static const int ImageDimension = TInputImage::ImageDimension;
    typedef TInputImage                          InputImageType;
    typedef TOutputImage                         OutputImageType;
    typedef typename InputImageType::PixelType   InputPixelType;
    typedef typename InputPixelType::ValueType   InputScalarType;
    typedef InputPixelType                       TensorType;
    typedef typename OutputImageType::PixelType  OutputPixelType;
    typedef typename OutputPixelType::ScalarType ScalarType;
    typedef typename OutputImageType::RegionType OutputImageRegionType;
    typedef typename OutputPixelType::PointType  PointType;
    typedef typename OutputPixelType::SheetPoint SheetPointType;
    typedef typename OutputImageType::IndexType  IndexType;
    typedef ContinuousIndex<ScalarType, ImageDimension>                      ContinuousIndexType;
    //typedef ContinuousIndex< InputScalarType, TInputImage::ImageDimension > ContinuousIndexType;
    typedef typename PointType::VectorType VectorType;
    
    typedef AffineTransform<ScalarType, TInputImage::ImageDimension>   AffineTransformType;
    typedef AffineTensorTransform<double, TInputImage::ImageDimension> AffineTensorTransformType;
    
    typedef Image<unsigned int, ImageDimension> ImageType;
    /// typedefs definitions
    typedef itk::CrossHelper<VectorType> CrossHelperType;
  

    // for tensor interpolation
    typedef InterpolateImageFunction<TInputImage, ScalarType>                InterpolatorType;
    typedef TensorLinearInterpolateImageFunction<TInputImage, ScalarType>    LinearInterpolatorType;
    typedef NearestNeighborInterpolateImageFunction<TInputImage, ScalarType> NNInterpolatorType;

    itkSetMacro (Smoothness, double);
    itkGetMacro (Smoothness, double);
    itkSetMacro (MinSurface, double);
    itkGetMacro (MinSurface, double);
    itkSetMacro (MaxSurface, double);
    itkGetMacro (MaxSurface, double);
    itkSetMacro (MaxDepth,   unsigned int);
    itkGetMacro (MaxDepth,   unsigned int);
    itkSetMacro (FAThreshold, double);
    itkGetMacro (FAThreshold, double);
    itkSetMacro (FAThreshold2, double);
    itkGetMacro (FAThreshold2, double);
    itkSetMacro (TimeStep, double);
    itkGetMacro (TimeStep, double);
    itkSetMacro (OutputSheetSampling, double);
    itkGetMacro (OutputSheetSampling, double);
    itkSetMacro (UseTriLinearInterpolation, bool);
    itkGetMacro (UseTriLinearInterpolation, bool);
    itkSetMacro (IntegrationMethod, int);
    itkGetMacro (IntegrationMethod, int);
    itkSetMacro (Sampling, int);
    itkGetMacro (Sampling, int);
    itkSetObjectMacro (AffineTransform, AffineTransformType);
    itkGetObjectMacro (AffineTransform, AffineTransformType);
    itkSetObjectMacro(SeedImage, ImageType);
    itkGetObjectMacro(SeedImage, ImageType);
    itkGetObjectMacro(SheetsSeededImage, ImageType);

    void SetLogTensorImage(const InputImageType *logImage);

    /**
       The tensors need to be re-oriented according to
       image directions.
       For correct behaviour please call TransformTensorWithImageDirectionOn().
       By default it is disabled for backward compatibility.
    */
    itkGetMacro(TransformTensorWithImageDirection, unsigned int);
    itkSetClampMacro (TransformTensorWithImageDirection, unsigned int, 0, 1);
    itkBooleanMacro (TransformTensorWithImageDirection);
    /**
       Transform method for tensors. If PDD is 0, finite strain is used.
       default transformation is PPD
    */
    itkSetMacro (TransformTensorWithPDD, int);
    itkGetMacro (TransformTensorWithPDD, int);
    itkBooleanMacro (TransformTensorWithPDD);

    virtual void GenerateInputRequestedRegion() throw(InvalidRequestedRegionError);


  protected:
    SheetTrackingImageFilter();
    ~SheetTrackingImageFilter(){};
    

    /** Threaded implementation */
    void BeforeThreadedGenerateData (void) ITK_OVERRIDE;
    void DynamicThreadedGenerateData(const OutputImageRegionType &) ITK_OVERRIDE;
    void PrintSelf (std::ostream& os, Indent indent) const;


    void                   GetSheetInDirection (ContinuousIndexType ind, VectorType vec, OutputPixelType &sheet, int firstpoint=0, int sidewalk=0);
    inline InputPixelType  InterpolateTensorAt (const ContinuousIndexType&) const;
    inline InputPixelType  RotateTensor (const InputPixelType& T) const;
    inline VectorType      GetOutputDirection (const VectorType&, const TensorType&, int sidewalk) const;
    inline PointType       IntegrateWithInitialConditions(const PointType& pos, const VectorType& v, const TensorType& T, int sidewalk=0);
        
    inline PointType       FirstOrderEulerIntegration(const PointType& pos, const VectorType& v, const TensorType& T);
    inline PointType       SecondOrderRungeKuttaIntegration(const PointType& pos, const VectorType& v, const TensorType& T);
    inline PointType       FourthOrderRungeKuttaIntegration(const PointType& pos, const VectorType& v, const TensorType& T, int sidewalk=0);
    
  private:

    double m_Smoothness;
    double m_MinSurface;
    double m_MaxSurface;
    unsigned int m_MaxDepth;
    double m_FAThreshold;
    double m_FAThreshold2;
    double m_TimeStep;
    VectorType m_IntegrationStep;
    double m_OutputSheetSampling;    
    bool   m_UseTriLinearInterpolation;
    int    m_Sampling;
    int    m_IntegrationMethod;
    unsigned int m_TransformTensorWithImageDirection;
    int m_TransformTensorWithPDD;
    
    typename AffineTransformType::Pointer m_AffineTransform;
    typename AffineTensorTransformType::Pointer m_InternalAffineTransform;
    typename InterpolatorType::Pointer m_Interpolator;
    
    typename ImageType::Pointer m_SeedImage;

    typename ImageType::Pointer m_SheetsSeededImage;
	
  };
    

} // end of namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSheetTrackingImageFilter.txx"
#endif
