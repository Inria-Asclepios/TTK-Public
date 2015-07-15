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
#ifndef _itkTensorTransform_txx
#define _itkTensorTransform_txx

#include "itkTensorTransform.h"

namespace itk
{


/**
 * Constructor
 */
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions >
TensorTransform< TScalarType,NInputDimensions,NOutputDimensions>
::TensorTransform():
  m_Parameters(1),
  m_FixedParameters(1),
  m_Jacobian(NOutputDimensions,1)
{
  itkWarningMacro(<< "Using default transform constructor.  Should specify NOutputDims and NParameters as args to constructor.");
}



/**
 * Constructor
 */
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions >
TensorTransform< TScalarType,NInputDimensions,NOutputDimensions>
::TensorTransform(unsigned int dimension,unsigned int numberOfParameters):
  m_Parameters(numberOfParameters),
  m_FixedParameters(numberOfParameters),
  m_Jacobian(dimension,numberOfParameters)
{
}


/**
 * GenerateName
 */
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions >
std::string TensorTransform< TScalarType,NInputDimensions,NOutputDimensions>
::GetTransformTypeAsString () const
{
    std::ostringstream n;
  n << GetNameOfClass();
  n << "_";
  if ( typeid ( TScalarType ) == typeid ( float ) )
    {
    n << "float";
    }
  else if ( typeid ( TScalarType ) == typeid ( double ) )
    {
      n << "double";
    }
  else
    {
      n << "other";
    }
  n << "_" << InputSpaceDimension << "_" << OutputSpaceDimension;
  return n.str();
}

template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions >
void
TensorTransform< TScalarType,NInputDimensions,NOutputDimensions>
::CopyInParameters(const ParametersValueType * const begin,
                   const ParametersValueType * const end)
{
   //Ensure that we are not copying onto self
   if( begin != &(this->m_Parameters[0]) )
   {
   //Copy raw values array
   std::copy(begin,end,this->m_Parameters.data_block() );
   }
  //Now call child class set parameter to interpret raw values
  this->SetParameters(this->m_Parameters);
}

template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions >
void
TensorTransform< TScalarType,NInputDimensions,NOutputDimensions>
::CopyInFixedParameters(const FixedParametersValueType * const begin,
                        const FixedParametersValueType * const end)
{
   //Ensure that we are not copying onto self
   if( begin != &(this->m_FixedParameters[0]) )
   {
   //Copy raw values array
   std::copy(begin,end,this->m_FixedParameters.data_block() );
   }
  //Now call child class set parameter to interpret raw values
  this->SetFixedParameters(this->m_FixedParameters);
}

} // end namespace itk


#endif

