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
#ifndef _itk_TensorNormalizeTensorImageFilter_h_
#define _itk_TensorNormalizeTensorImageFilter_h_


namespace itk
{


  template <class TTensorInput, class TTensorOutput>
    class ITK_EXPORT TensorNormalizeTensorImageFilter :
  public ImageToImageFilter<TTensorInput, TTensorOutput>
  {

  public:
    typedef TensorNormalizeTensorImageFilter                Self;
    typedef ImageToImageFilter<TTensorInput, TTensorOutput> Superclass;
    typedef SmartPointer <Self>       Pointer;
    typedef SmartPointer <const Self> ConstPointer;

    itkNewMacro  (Self);
    itkTypeMacro (TensorNormalizeTensorImageFilter, ImageToImageFilter);
    
    typedef TTensorInput                           InputImageType;
    typedef typename InputImageType::PixelType     InputPixelType;
    typedef TTensorOutput                          OutputImageType;
    typedef typename OutputImageType::PixelType    OutputPixelType;
    typedef typename OutputImageType::RegionType   OutputRegionType;

    itkSetMacro (NormalizationFactor, double);
    itkGetMacro (NormalizationFactor, double);
    
  protected:
    TensorNormalizeTensorImageFilter()
    {
      m_NormalizationFactor = 1.0;
    }
    ~TensorNormalizeTensorImageFilter(){};

    void ThreadedGenerateData(const OutputRegionType &, int);

  private:
    TensorNormalizeTensorImageFilter (const Self&);
    void operator=(const Self&);

    double m_NormalizationFactor;
    
  };
  
  
} // end of namespace


#include "itkTensorNormalizeTensorImageFilter.txx"

#endif
