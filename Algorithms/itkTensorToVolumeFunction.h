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
#ifndef _itk_TensorToVolumeFunction_h_
#define _itk_TensorToVolumeFunction_h_

#include "itkTensorToScalarFunction.h"

namespace itk
{

  template <class TInputPixelType, class TOutputPixelType>
    class ITK_EXPORT TensorToVolumeFunction
    : public TensorToScalarFunction<TInputPixelType, TOutputPixelType>
  {
    
  public:
    typedef TensorToVolumeFunction Self;
    typedef TensorToScalarFunction<TInputPixelType,TOutputPixelType> Superclass;

    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkTypeMacro (TensorToVolumeFunction, TensorToScalarFunction);

    itkNewMacro (Self);


    typedef TInputPixelType   InputPixelType;
    typedef TOutputPixelType  OutputPixelType;    

    OutputPixelType ComputeScalar (const InputPixelType&) const;
    

  protected:
    TensorToVolumeFunction(){};
    ~TensorToVolumeFunction(){};

    void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf(os, indent);
    }


  private:
    TensorToVolumeFunction (const Self&);
    void operator=(const Self&);        
    

  };  
  
  
} // end of namespace


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTensorToVolumeFunction.txx"
#endif


#endif
