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
#ifndef _itkPermute23CoefficientsTensorImageFilter_h
#define _itkPermute23CoefficientsTensorImageFilter_h

#include <itkImageToImageFilter.h>
#include <itkImage.h>
#include <itkTensor.h>


namespace itk
{

  /**
   * \ Class Permute23CoefficientsTensorImageFilter
   *
   **/
  
  template <class TInputImage, class TOutputImage>
  class ITK_EXPORT Permute23CoefficientsTensorImageFilter :
    public ImageToImageFilter<TInputImage, TOutputImage>
  {

  public:
    
    typedef Permute23CoefficientsTensorImageFilter Self;
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
    Permute23CoefficientsTensorImageFilter(){};
    ~Permute23CoefficientsTensorImageFilter(){};
    
    void ThreadedGenerateData(const OutputRegionType &, int);
    void PrintSelf(std::ostream &os, Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    }
    
  private:
    Permute23CoefficientsTensorImageFilter(const Self&);
    void operator=(const Self&);
    
  };


} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkPermute23CoefficientsTensorImageFilter.txx"
#endif


#endif
