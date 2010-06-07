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
#ifndef _itk_AddGaussianNoiseTensorImageFilter_h_
#define _itk_AddGaussianNoiseTensorImageFilter_h_

#include <itkImageToImageFilter.h>
#include <itkImage.h>
#include <itkTensor.h>
#include <itkNormalVariateGenerator.h>

namespace itk
{


  template <class TInputImage, class TOutputImage>
    class ITK_EXPORT AddGaussianNoiseTensorImageFilter :
  public ImageToImageFilter<TInputImage, TOutputImage>
  {

  public:
    typedef AddGaussianNoiseTensorImageFilter Self;
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
    AddGaussianNoiseTensorImageFilter()
    {
      m_Variance = 1.0;
      m_NormalGenerator = Statistics::NormalVariateGenerator::New();      
    }
    ~AddGaussianNoiseTensorImageFilter(){};

    void BeforeThreadedGenerateData(void);
    void ThreadedGenerateData(const OutputRegionType &, int);

    
  private:
    AddGaussianNoiseTensorImageFilter (const Self&);
    void operator=(const Self&);

    double m_Variance;

    typename Statistics::NormalVariateGenerator::Pointer m_NormalGenerator;
    
  };
  

} // end of namespace


#include "itkAddGaussianNoiseTensorImageFilter.txx"

#endif
