#ifndef _itk_AddGaussianNoiseImageFilter_h_
#define _itk_AddGaussianNoiseImageFilter_h_

#include <itkImageToImageFilter.h>
#include <itkImage.h>
#include <itkNormalVariateGenerator.h>

namespace itk
{


  template <class TInputImage, class TOutputImage>
    class ITK_EXPORT AddGaussianNoiseImageFilter :
  public ImageToImageFilter<TInputImage, TOutputImage>
  {

  public:
    typedef AddGaussianNoiseImageFilter Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;

    itkNewMacro(Self);
    itkTypeMacro (AddGaussianNoiseTensorImageFilter, ImageToImageFilter);

    typedef TInputImage                           InputImageType;
    typedef typename InputImageType::PixelType    InputPixelType;
    typedef TOutputImage                          OutputImageType;
    typedef typename OutputImageType::PixelType   OutputPixelType;
    typedef typename OutputImageType::RegionType  OutputRegionType;

    itkSetMacro (Variance, double);
    itkGetMacro (Variance, double);
    
  protected:
    AddGaussianNoiseImageFilter()
    {
      m_Variance = 1.0;
      m_NormalGenerator = Statistics::NormalVariateGenerator::New();      
    }
    ~AddGaussianNoiseImageFilter(){};

    void BeforeThreadedGenerateData(void);
    void ThreadedGenerateData(const OutputRegionType &, int);

    
  private:
    AddGaussianNoiseImageFilter (const Self&);
    void operator=(const Self&);

    double m_Variance;

    typename Statistics::NormalVariateGenerator::Pointer m_NormalGenerator;
    
  };
  

} // end of namespace


#include "itkAddGaussianNoiseImageFilter.txx"

#endif
