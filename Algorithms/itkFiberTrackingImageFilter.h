#ifndef _itk_FiberTrackingImageFilter_h_
#define _itk_FiberTrackingImageFilter_h_


#include <itkImageToImageFilter.h>
#include <itkContinuousIndex.h>
#include <itkAffineTransform.h>
#include "itkTensorLinearInterpolateImageFunction.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkLogTensorImageFilter.h"

namespace itk
{

  /*! \class FiberTrackingImageFilter
    \ingroup TensorToolKit
    
    Description: Track fibers in a tensor image based
    on the tensorline algorithm proposed by Weinstein.
    This filter is multithreaded.
           
   */

  template <class TInputImage, class TOutputImage>
    class ITK_EXPORT FiberTrackingImageFilter:
  public ImageToImageFilter <TInputImage, TOutputImage>
  {

  public:

    typedef FiberTrackingImageFilter                      Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    itkNewMacro (Self);
    itkTypeMacro (FiberTrackingImageFilter,
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
    typedef typename OutputImageType::IndexType  IndexType;
    typedef ContinuousIndex<ScalarType, ImageDimension>                      ContinuousIndexType;
    //typedef ContinuousIndex< InputScalarType, TInputImage::ImageDimension > ContinuousIndexType;
    typedef typename OutputPixelType::VectorType VectorType;
    typedef AffineTransform<ScalarType, TInputImage::ImageDimension>   AffineTransformType;
	
    typedef Image<ScalarType, ImageDimension> ImageType;
    

    typedef LogTensorImageFilter<TInputImage,TInputImage> LogFilterType;

    // for tensor interpolation
    typedef InterpolateImageFunction<TInputImage, ScalarType>                InterpolatorType;
    typedef TensorLinearInterpolateImageFunction<TInputImage, ScalarType>    LinearInterpolatorType;
    typedef NearestNeighborInterpolateImageFunction<TInputImage, ScalarType> NNInterpolatorType;

    itkSetMacro (Smoothness, double);
    itkGetMacro (Smoothness, double);
    itkSetMacro (MinLength, double);
    itkGetMacro (MinLength, double);
    itkSetMacro (MaxLength, double);
    itkGetMacro (MaxLength, double);
    itkSetMacro (FAThreshold, double);
    itkGetMacro (FAThreshold, double);
    itkSetMacro (FAThreshold2, double);
    itkGetMacro (FAThreshold2, double);
    itkSetMacro (TimeStep, double);
    itkGetMacro (TimeStep, double);
    itkSetMacro (UseTriLinearInterpolation, bool);
    itkGetMacro (UseTriLinearInterpolation, bool);
    itkSetMacro (IntegrationMethod, int);
    itkGetMacro (IntegrationMethod, int);
    itkSetMacro (Sampling, int);
    itkGetMacro (Sampling, int);
    itkSetObjectMacro (AffineTransform, AffineTransformType);
    itkGetObjectMacro (AffineTransform, AffineTransformType);
    itkSetConstObjectMacro (LogTensorImage, InputImageType);
    itkGetConstObjectMacro (LogTensorImage, InputImageType);
    itkSetObjectMacro(SeedImage, ImageType);
    itkGetObjectMacro(SeedImage, ImageType);
    itkGetObjectMacro(FibersSeededImage, ImageType);


    virtual void GenerateInputRequestedRegion() throw(InvalidRequestedRegionError);


  protected:
    FiberTrackingImageFilter()
    {
      m_Sampling = 1;
      m_Smoothness = 0.0;
      m_MinLength = 10.0;
      m_MaxLength = 200.0;
      m_FAThreshold = 0.3;
      m_FAThreshold2 = 0.01;
      m_TimeStep = 1.0;
      m_UseTriLinearInterpolation = true;
      m_AffineTransform = AffineTransformType::New();
      m_AffineTransform->SetIdentity();
      m_Interpolator = NULL;
      m_IntegrationMethod = 0;
      m_SeedImage = 0;
      m_FibersSeededImage = 0;
      for( unsigned int i=0; i<ImageDimension; i++)
      {
	m_IntegrationStep[i] = 1.0;
      }
    }
    ~FiberTrackingImageFilter(){};
    

    /** Threaded implementation */
    void BeforeThreadedGenerateData (void);
    void ThreadedGenerateData(const OutputImageRegionType &,int);
    void PrintSelf (std::ostream& os, Indent indent) const;


    OutputPixelType GetFiberInDirection (ContinuousIndexType, VectorType, int firstPoint=0);
    InputPixelType  InterpolateTensorAt (const ContinuousIndexType&);
    VectorType      GetOutputDirection (const VectorType&, const TensorType&);
    PointType       IntegrateWithInitialConditions(const PointType& pos, const VectorType& v, const TensorType& T);

        
    PointType       FirstOrderEulerIntegration(const PointType& pos, const VectorType& v, const TensorType& T);
    PointType       SecondOderRungeKuttaIntegration(const PointType& pos, const VectorType& v, const TensorType& T);
    PointType       FourthOderRungeKuttaIntegration(const PointType& pos, const VectorType& v, const TensorType& T);
    
  private:

    double m_Smoothness;
    double m_MinLength;
    double m_MaxLength;
    double m_FAThreshold;
    double m_FAThreshold2;
    double m_TimeStep;
    VectorType m_IntegrationStep;
    bool   m_UseTriLinearInterpolation;
    int    m_Sampling;
    int    m_IntegrationMethod;
    typename AffineTransformType::Pointer m_AffineTransform;
    typename InterpolatorType::Pointer m_Interpolator;

    typename InputImageType::ConstPointer m_LogTensorImage;
    
    typename ImageType::Pointer m_SeedImage;

    typename ImageType::Pointer m_FibersSeededImage;
	
  };
    

} // end of namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFiberTrackingImageFilter.txx"
#endif

#endif
