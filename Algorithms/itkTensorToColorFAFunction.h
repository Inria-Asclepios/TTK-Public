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
#ifndef _itk_TensorToColorFAFunction_h_
#define _itk_TensorToColorFAFunction_h_

#include "itkTensorToScalarFunction.h"
#include <itkImageBase.h>

namespace itk
{

  template <class TInputPixelType, class TOutputPixelType>
    class ITK_EXPORT TensorToColorFAFunction
    : public TensorToScalarFunction<TInputPixelType, TOutputPixelType>
  {

  public:
    typedef TensorToColorFAFunction Self;
    typedef TensorToScalarFunction<TInputPixelType, TOutputPixelType> Superclass;

    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkTypeMacro (TensorToColorFAFunction, TensorToScalarFunction);
    itkNewMacro (Self);

    typedef TInputPixelType   InputPixelType;
    typedef TOutputPixelType  OutputPixelType;    

    OutputPixelType ComputeScalar (const InputPixelType&) const;
      
      void SetTransformColorWithDirection (bool value) {m_TransformColorWithDirection = value;}
      
      typedef itk::ImageBase <3>::DirectionType MatrixType;
      //! Transform used if TransformColorWithDirection is on, has to be the image direction matrix
      void SetDirection(const MatrixType &matrix) {m_Direction = matrix;}

  protected:
    TensorToColorFAFunction(){};
    ~TensorToColorFAFunction(){};
        
     void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf(os, indent);
    }

  private:
     TensorToColorFAFunction (const Self&);
     void operator=(const Self&);
    
      bool m_TransformColorWithDirection;
      MatrixType m_Direction;
      
      
  };
  

} // end of namespace


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTensorToColorFAFunction.txx"
#endif

#endif
