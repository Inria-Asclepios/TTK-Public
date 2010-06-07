/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL:$
  Language:  C++
  Date:      $Date:$
  Version:   $Revision:$

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkLogTensorImageFilter_h
#define _itkLogTensorImageFilter_h

#include <itkImageToImageFilter.h>
#include <itkImage.h>
#include <itkTensor.h>


namespace itk
{

  /**
   * \ Class LogTensorImageFilter
   *
   **/
  
  template <class TInputImage, class TOutputImage>
  class ITK_EXPORT LogTensorImageFilter :
    public ImageToImageFilter<TInputImage, TOutputImage>
  {

  public:
    
    typedef LogTensorImageFilter Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    typedef TInputImage                           InputImageType;
    typedef typename InputImageType::PixelType    InputPixelType;
    typedef TOutputImage                          OutputImageType;
    typedef typename OutputImageType::PixelType   OutputPixelType;
    typedef typename OutputImageType::RegionType  OutputRegionType;
    
    itkStaticConstMacro(ImageDimension, unsigned int,
                        OutputImageType::ImageDimension);
    
    itkNewMacro(Self);
    
  protected:
    LogTensorImageFilter(){};
    ~LogTensorImageFilter(){};
    
    void ThreadedGenerateData(const OutputRegionType &, int);
    void PrintSelf(std::ostream &os, Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    }
    
  private:
    LogTensorImageFilter(const Self&);
    void operator=(const Self&);
    
  };


} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLogTensorImageFilter.txx"
#endif


#endif
