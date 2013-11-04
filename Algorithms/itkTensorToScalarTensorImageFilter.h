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
#ifndef _itk_TensorToScalarTensorImageFilter_h_
#define _itk_TensorToScalarTensorImageFilter_h_

#include "itkImageToImageFilter.h"
#include "itkTensorToScalarFunction.h"


namespace itk
{

  template <class TInputImage, class TOutputImage>
    class ITK_EXPORT TensorToScalarTensorImageFilter
    : public ImageToImageFilter<TInputImage, TOutputImage>
  {

  public:
    
    typedef TensorToScalarTensorImageFilter Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    
    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    itkTypeMacro (TensorToScalarTensorImageFilter, ImageToImageFilter);
    
    itkStaticConstMacro (ImageDimension, unsigned int,
                         TOutputImage::ImageDimension);
    
    itkNewMacro (Self);

    typedef TInputImage                           InputImageType;
    typedef typename InputImageType::PixelType    InputPixelType;    
    typedef TOutputImage                          OutputImageType;
    typedef typename OutputImageType::PixelType   OutputPixelType;
    typedef typename OutputImageType::RegionType  OutputImageRegionType;

    typedef TensorToScalarFunction<InputPixelType, OutputPixelType>      
      TensorToScalarFunctionType;

    itkGetConstReferenceObjectMacro (TensorToScalarFunction,
                                     TensorToScalarFunctionType );
    
    itkSetObjectMacro ( TensorToScalarFunction,
                        TensorToScalarFunctionType );
    
  protected:
    TensorToScalarTensorImageFilter();
    ~TensorToScalarTensorImageFilter(){};

    void PrintSelf (std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf (os, indent);      
    }
    

    void BeforeThreadedGenerateData (void);
    void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, ThreadIdType threadId);

    
  private:
    TensorToScalarTensorImageFilter (const Self&);
    void operator=(const Self&);

    typename TensorToScalarFunctionType::Pointer m_TensorToScalarFunction;
    
        

  };    


} // end of namespace


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTensorToScalarTensorImageFilter.txx"
#endif


#endif
