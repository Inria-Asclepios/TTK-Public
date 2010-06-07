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
#ifndef _itk_AverageStdImageFilter_h_
#define _itk_AverageStdImageFilter_h_

#include <itkImageToImageFilter.h>

namespace itk
{

template <class TInputImage, class TOutputImage>
class ITK_EXPORT AverageStdImageFilter :
public ImageToImageFilter<TInputImage, TOutputImage>
{

public:

typedef AverageStdImageFilter Self;
typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
typedef SmartPointer<Self> Pointer;
typedef SmartPointer<const Self> ConstPointer;

itkNewMacro(Self);
itkTypeMacro(AverageStdImageFilter, ImageToImageFilter);


typedef TInputImage InputImageType;
typedef TOutputImage OutputImageType;

typedef typename InputImageType::PixelType InputPixelType;
typedef typename OutputImageType::PixelType OutputPixelType;
typedef typename OutputImageType::RegionType    OutputImageRegionType;


typedef OutputPixelType ScalarType;


itkStaticConstMacro( ImageDimension, unsigned int, InputImageType::ImageDimension);

itkGetObjectMacro( VarianceImage, OutputImageType);

protected:
AverageStdImageFilter()
{
	m_VarianceImage = OutputImageType::New();
}
~AverageStdImageFilter(){};

void BeforeThreadedGenerateData(void);
void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, int threadId);
void PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

private:

typename OutputImageType::Pointer m_VarianceImage;


};


} // end of namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAverageStdImageFilter.txx"
#endif

#endif
