#ifndef _itk_CrossingTensorImageSource_h_
#define _itk_CrossingTensorImageSource_h_

#include "itkImageSource.h"
#include "itkFixedArray.h"
#include "itkSize.h"

namespace itk
{

  template <class TTensorImage>
    class CrossingTensorImageSource :
  public ImageSource<TTensorImage>
  {

  public:
    typedef CrossingTensorImageSource Self;
    typedef ImageSource<TTensorImage> Superclass;

    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkNewMacro (Self);
    itkTypeMacro (CrossingTensorImageSource, ImageSource);

    typedef TTensorImage                         TensorImageType;
    typedef typename TensorImageType::PixelType  TensorType;
    typedef typename TensorImageType::RegionType RegionType;
    typedef typename TensorImageType::PointType  PointType;
    typedef typename TensorImageType::SpacingType SpacingType;
    
    typedef typename TensorType::VectorType      VectorType;


    itkSetMacro (NumberOfCrossings, unsigned int);
    itkGetMacro (NumberOfCrossings, unsigned int);


    /** Dimensionality of the output image */
    itkStaticConstMacro(NDimensions, unsigned int, TensorImageType::ImageDimension);

    /** Type used to store gaussian parameters. */
    typedef FixedArray<double, itkGetStaticConstMacro(NDimensions)> ArrayType;
    
    /** Size type matches that used for images */
    typedef Size<itkGetStaticConstMacro(NDimensions)> SizeType;

    /** Specify the size of the output image. */
    virtual void SetSize( const unsigned long* values);
    
    /** Specify the size of the output image. */
    virtual void SetSize( const SizeType values);
    
    /** Get the size of the output image. */
    itkGetVectorMacro(Size,const unsigned long,NDimensions);
    
    /** Specify the spacing of the output image. */
    virtual void SetSpacing( const SpacingType& values);
    virtual void SetSpacing( const float* values);
    virtual void SetSpacing( const double* values);
    
    /** Get the spacing of the output image. */
    itkGetMacro(Spacing,const SpacingType);
    
    /** Specify the origin of the output image. */
    virtual void SetOrigin( const PointType& values);
    virtual void SetOrigin( const float* values);
    virtual void SetOrigin( const double* values);
    
    /** Get the origin of the output image. */
    itkGetMacro(Origin,PointType);

    itkSetMacro (Radius, double);
    itkGetMacro (Radius, double);
    
  protected:
    CrossingTensorImageSource()
    {
      m_NumberOfCrossings = 0;
      for (unsigned int i=0; i<TensorImageType::GetImageDimension(); i++)
      {
	m_Size[i] = 16;
	m_Spacing[i] = 1.0;
	m_Origin[i] = 0.0;
      }
      m_Radius = 1.0;
    }
    ~CrossingTensorImageSource(){};

    virtual void ThreadedGenerateData (const RegionType&, int);
    virtual void GenerateOutputInformation();

  private:
    CrossingTensorImageSource(const Self&);
    void operator=(const Self&);

    unsigned int m_NumberOfCrossings;

    unsigned long  m_Size[NDimensions];    //size of the output image
    SpacingType    m_Spacing; //spacing
    PointType      m_Origin;  //origin
    double         m_Radius;
    

  };
  

} // end of namespace


#include "itkCrossingTensorImageSource.txx"

#endif
