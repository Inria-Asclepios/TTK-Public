#ifndef _itkExpTensorImageFilter_h
#define _itkExpTensorImageFilter_h

#include <itkImageToImageFilter.h>
#include <itkImage.h>
#include <itkTensor.h>

namespace itk
{

  /*! \class ExpTensorImageFilter
    \ingroup TensorToolKit
    A filter that exponentiates a itk tensor image.
  */
  
  template <class TInputImage, class TOutputImage>
  class ITK_EXPORT ExpTensorImageFilter :
    public ImageToImageFilter<TInputImage, TOutputImage>
  {

  public:
    
    typedef ExpTensorImageFilter                          Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    typedef TInputImage                          InputImageType;
    typedef typename InputImageType::PixelType   InputPixelType;
    typedef TOutputImage                         OutputImageType;
    typedef typename OutputImageType::PixelType  OutputPixelType;
    typedef typename OutputImageType::RegionType OutputRegionType;
    
    itkStaticConstMacro(ImageDimension, unsigned int,
                        OutputImageType::ImageDimension);
    
    itkNewMacro(Self);
    
  protected:
    ExpTensorImageFilter(){};
    ~ExpTensorImageFilter(){};
    
    void ThreadedGenerateData(const OutputRegionType &, int);
    void PrintSelf(std::ostream &os, Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    }
    
  private:
    ExpTensorImageFilter(const Self&);
    void operator=(const Self&);
    
  };


} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkExpTensorImageFilter.txx"
#endif


#endif
