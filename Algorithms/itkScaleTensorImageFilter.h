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
#ifndef _itk_ScaleTensorImageFilter_h_
#define _itk_ScaleTensorImageFilter_h_

#include "itkImageToImageFilter.h"

namespace itk
{


  template <class TTensorInput, class TTensorOutput>
    class ITK_EXPORT ScaleTensorImageFilter :
  public ImageToImageFilter<TTensorInput, TTensorOutput>
  {

  public:
    typedef ScaleTensorImageFilter                Self;
    typedef ImageToImageFilter<TTensorInput, TTensorOutput> Superclass;
    typedef SmartPointer <Self>       Pointer;
    typedef SmartPointer <const Self> ConstPointer;

    itkNewMacro  (Self);
    itkTypeMacro (ScaleTensorImageFilter, ImageToImageFilter);
    
    typedef TTensorInput                           InputImageType;
    typedef typename InputImageType::PixelType     InputPixelType;
    typedef TTensorOutput                          OutputImageType;
    typedef typename OutputImageType::PixelType    OutputPixelType;
    typedef typename OutputImageType::RegionType   OutputRegionType;

    /**
       Compute exp( scale * log(T) ) instead of scale * T
     */
    itkSetMacro (UseLogScale, bool);
    itkGetMacro (UseLogScale, bool);
    itkBooleanMacro (UseLogScale);

    itkSetMacro (ScalingFactor, double);
    itkGetMacro (ScalingFactor, double);
    
  protected:
    ScaleTensorImageFilter()
    {
      m_UseLogScale = false;
      m_ScalingFactor = 1.0;
    }
    ~ScaleTensorImageFilter(){};

    void DynamicThreadedGenerateData(const OutputRegionType &);

  private:
    ScaleTensorImageFilter (const Self&);
    void operator=(const Self&);

    bool m_UseLogScale;
    double m_ScalingFactor;
  };
  
  
} // end of namespace


#include "itkScaleTensorImageFilter.txx"

#endif
