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
    void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, ThreadIdType threadId);
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
