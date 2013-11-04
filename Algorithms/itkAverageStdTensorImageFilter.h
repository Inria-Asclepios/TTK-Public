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
#ifndef _itk_AverageStdTensorImageFilter_h_
#define _itk_AverageStdTensorImageFilter_h_

#include <itkImageToImageFilter.h>

namespace itk
{

template <class TInputImage, class TOutputImage>
class ITK_EXPORT AverageStdTensorImageFilter :
public ImageToImageFilter<TInputImage, TOutputImage>
{

public:

typedef AverageStdTensorImageFilter Self;
typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
typedef SmartPointer<Self> Pointer;
typedef SmartPointer<const Self> ConstPointer;

itkNewMacro(Self);
itkTypeMacro(AverageStdTensorImageFilter, ImageToImageFilter);


typedef TInputImage InputImageType;
typedef TOutputImage OutputImageType;

typedef typename InputImageType::PixelType InputPixelType;
typedef typename OutputImageType::PixelType OutputPixelType;
typedef typename OutputImageType::RegionType    OutputImageRegionType;


typedef typename OutputPixelType::ValueType ScalarType;


itkStaticConstMacro( ImageDimension, unsigned int, InputImageType::ImageDimension);

itkStaticConstMacro( TensorDegreesOfFreedom, unsigned int,  InputPixelType::DegreesOfFreedom);

itkStaticConstMacro (CovarianceMatrixDOF, unsigned int, InputPixelType::DegreesOfFreedom*(InputPixelType::DegreesOfFreedom+1)/2);

typedef  Tensor<ScalarType, TensorDegreesOfFreedom> CovMatrixType;
typedef  Image <CovMatrixType, ImageDimension>    CovMatrixImageType;

typedef Vector<ScalarType, InputPixelType::DegreesOfFreedom> VectorType;

itkGetObjectMacro(CovarianceMatrixImage, CovMatrixImageType);


protected:
AverageStdTensorImageFilter()
{
	m_CovarianceMatrixImage = CovMatrixImageType::New();
}
~AverageStdTensorImageFilter(){};

void BeforeThreadedGenerateData(void);
void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, int threadId);
void PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

VectorType Tensor2Vec(const InputPixelType&) const;


private:

typename CovMatrixImageType::Pointer m_CovarianceMatrixImage;


};


} // end of namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAverageStdTensorImageFilter.txx"
#endif

#endif
