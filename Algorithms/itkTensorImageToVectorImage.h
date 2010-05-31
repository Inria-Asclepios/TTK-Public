#ifndef _itk_TensorImageToVectorImage_h_
#define _itk_TensorImageToVectorImage_h_

#include <itkImageToImageFilter.h>

namespace itk
{

  template <class TInputImage, class TOutputImage>
    class ITK_EXPORT TensorImageToVectorImage :
  public ImageToImageFilter<TInputImage, TOutputImage>
  {
    
  public:

    typedef TensorImageToVectorImage Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkNewMacro  (Self);
    itkTypeMacro (TensorImageToVectorImage, ImageToImageFilter);


    typedef TInputImage                           InputImageType;
    typedef typename InputImageType::PixelType    InputPixelType;
    typedef TOutputImage                          OutputImageType;
    typedef typename OutputImageType::PixelType   OutputPixelType;
    typedef typename OutputImageType::RegionType  OutputImageRegionType;
    

  protected:
    TensorImageToVectorImage(){};
    ~TensorImageToVectorImage(){};

    void BeforeThreadedGenerateData(void);
    void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, int threadId);
    void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    }
  

  private:
    
    
  };
  
  

} // end of namespace


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTensorImageToVectorImage.txx"
#endif

#endif
