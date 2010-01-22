#ifndef _itkNormalizeTensorImageFilter_h
#define _itkNormalizeTensorImageFilter_h

#include <itkImageToImageFilter.h>
#include <itkImage.h>
#include <itkTensor.h>


namespace itk
{

  /**
   * \ Class NormalizeTensorImageFilter
   *
   **/
  
  template <class TInputImage, class TOutputImage>
  class ITK_EXPORT NormalizeTensorImageFilter :
    public ImageToImageFilter<TInputImage, TOutputImage>
  {

  public:
    
    typedef NormalizeTensorImageFilter Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    typedef TInputImage                           InputImageType;
    typedef typename InputImageType::PixelType    InputPixelType;
    typedef typename InputPixelType::ValueType    InputValueType;
    typedef TOutputImage                          OutputImageType;
    typedef typename OutputImageType::PixelType   OutputPixelType;
    typedef typename OutputImageType::RegionType  OutputRegionType;
    
    itkStaticConstMacro(ImageDimension, unsigned int,
                        OutputImageType::ImageDimension);
    
    itkNewMacro(Self);
    
  protected:
    NormalizeTensorImageFilter(){};
    ~NormalizeTensorImageFilter(){};
    
    void ThreadedGenerateData(const OutputRegionType &, int);
    void PrintSelf(std::ostream &os, Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    }
    
  private:
    NormalizeTensorImageFilter(const Self&);
    void operator=(const Self&);
    
  };


} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkNormalizeTensorImageFilter.txx"
#endif


#endif
