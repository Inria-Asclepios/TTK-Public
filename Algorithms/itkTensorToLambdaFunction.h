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
#ifndef _itk_TensorToLambdaFunction_h_
#define _itk_TensorToLambdaFunction_h_

#include "itkTensorToScalarFunction.h"

namespace itk
{

  template <class TInputPixelType, class TOutputPixelType>
    class ITK_EXPORT TensorToLambdaFunction
    : public TensorToScalarFunction<TInputPixelType, TOutputPixelType>
  {
    
  public:
    typedef TensorToLambdaFunction Self;
    typedef TensorToScalarFunction<TInputPixelType,TOutputPixelType> Superclass;

    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkTypeMacro (TensorToLambdaFunction, TensorToScalarFunction);

    itkNewMacro (Self);


    typedef TInputPixelType   InputPixelType;
    typedef TOutputPixelType  OutputPixelType;    

    OutputPixelType ComputeScalar (const InputPixelType&) const;
    
	void SetLambdaIndex(const unsigned int &n)
	{ m_LambdaIndex = n;}
	unsigned int GetLambdaIndex( void ) const
	{ return m_LambdaIndex; }	

  protected:
    TensorToLambdaFunction()
	{
		m_LambdaIndex = 2;
	};
    ~TensorToLambdaFunction(){};

    void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf(os, indent);
    }


  private:
    TensorToLambdaFunction (const Self&);
    void operator=(const Self&);        

	unsigned int m_LambdaIndex;

  };  
  
  
} // end of namespace


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTensorToLambdaFunction.txx"
#endif


#endif
