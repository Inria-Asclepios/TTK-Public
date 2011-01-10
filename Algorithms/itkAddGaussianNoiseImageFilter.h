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
    typedef typename InputImageType::Pointer      InputImagePointer;

    itkSetMacro (Variance, double);
    itkGetMacro (Variance, double);

    itkGetObjectMacro (NormalGenerator, Statistics::NormalVariateGenerator);
    itkSetObjectMacro (NormalGenerator, Statistics::NormalVariateGenerator);
    
    
  protected:
    AddGaussianNoiseImageFilter()
    {
      m_NormalGenerator = Statistics::NormalVariateGenerator::New();      
      m_Variance = 1.0;
    }
    ~AddGaussianNoiseImageFilter(){};

    void GenerateData(void);
    void GenerateOutputInformation();
    void GenerateInputRequestedRegion() throw (InvalidRequestedRegionError);
    
  private:
    AddGaussianNoiseImageFilter (const Self&);
    void operator=(const Self&);

    double m_Variance;
    unsigned int m_UseSeed;
    unsigned long m_Seed;
    
    typename Statistics::NormalVariateGenerator::Pointer m_NormalGenerator;
    
  };
  

} // end of namespace


#include "itkAddGaussianNoiseImageFilter.txx"

#endif
