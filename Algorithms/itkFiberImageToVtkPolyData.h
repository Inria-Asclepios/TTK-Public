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
#ifndef _itk_FiberImageToVtkPolyData_h_
#define _itk_FiberImageToVtkPolyData_h_

#include <itkProcessObject.h>
#include <vtkPolyData.h>
#include <itkTensor.h>
#include <itkImage.h>
#include <itkAffineTransform.h>

#include "itkTensorLinearInterpolateImageFunction.h"
#include "itkInterpolateImageFunction.h"
#include "itkLogTensorImageFilter.h"

namespace itk
{
  /*! \class FiberImageToVtkPolyData
    \ingroup TensorToolKit
    Transforms a fiber image into a vtkPolyData.
   */

  template <class TInputImage, class TensorImageType>
    class ITK_EXPORT FiberImageToVtkPolyData: public ProcessObject
  {

  public:

    typedef FiberImageToVtkPolyData  Self;
    typedef Object                   Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkNewMacro (Self);
    itkTypeMacro (FiberImageToVtkPolyData, Object);

    typedef TInputImage                        InputImageType;
    typedef typename InputImageType::PixelType FiberType;
    typedef typename FiberType::PointType      PointType;
    typedef typename FiberType::VectorType     VectorType;
    typedef typename FiberType::PointListType  PointListType;
    typedef typename FiberType::ScalarType     ScalarType;
    typedef vtkPolyData                        OutputType;


    typedef TensorLinearInterpolateImageFunction<TensorImageType, ScalarType>  InterpolatorType;
    typedef LogTensorImageFilter<TensorImageType, TensorImageType> LogFilterType;
    typedef AffineTransform<ScalarType, InputImageType::ImageDimension>   AffineTransformType;

    void SetInput (typename InputImageType::Pointer input)
    { m_Input = input; }    
    typename InputImageType::Pointer GetInput (void) const
    { return m_Input; }
    OutputType* GetOutput (void) const
    { return m_Output; }
    
    itkSetConstObjectMacro(TensorImage, TensorImageType);
    itkGetConstObjectMacro(TensorImage, TensorImageType);

    itkSetConstObjectMacro(LogTensorImage, TensorImageType);
    itkGetConstObjectMacro(LogTensorImage, TensorImageType);

    itkSetObjectMacro (AffineTransform, AffineTransformType);
    itkGetObjectMacro (AffineTransform, AffineTransformType);
	
    void Update (void);
        

  protected:
    FiberImageToVtkPolyData()
    {
      m_Input = 0;
      m_Output = OutputType::New();
      m_Output->Allocate();
      m_TensorImage = 0;
	  m_LogTensorImage = 0;
      m_AffineTransform = 0;
    };
    ~FiberImageToVtkPolyData()
    {
      m_Output->Delete();
    };
        
    

  private:
    FiberImageToVtkPolyData (const Self&);
    void operator=(const Self&);

    typename InputImageType::Pointer m_Input;
    OutputType*                      m_Output;
	
    typename TensorImageType::ConstPointer m_TensorImage;
    typename TensorImageType::ConstPointer m_LogTensorImage;
    typename AffineTransformType::Pointer  m_AffineTransform;
    

  };
    

} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFiberImageToVtkPolyData.txx"
#endif

#endif
