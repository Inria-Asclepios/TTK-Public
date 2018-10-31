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
#ifndef __itkAffineTensorTransform_h
#define __itkAffineTensorTransform_h

#include <iostream>

#include "itkMatrix.h"
#include "itkMatrixOffsetTensorTransformBase.h"
#include "itkExceptionObject.h"
#include "itkMacro.h"

namespace itk
{

  
  template <class TScalarType=double, unsigned int NDimensions=3>
    class AffineTensorTransform 
    : public MatrixOffsetTensorTransformBase< TScalarType, NDimensions, NDimensions >
    {
      
    public:
    
    /** Standard typedefs   */
    typedef AffineTensorTransform                           Self;
    typedef MatrixOffsetTensorTransformBase< TScalarType, NDimensions, NDimensions >  Superclass;
    
    typedef SmartPointer<Self>                        Pointer;
    typedef SmartPointer<const Self>                  ConstPointer;
    
    /** Run-time type information (and related methods).   */
    itkTypeMacro( AffineTensorTransform, MatrixOffsetTensorTransformBase );
    
    /** New macro for creation of through a Smart Pointer   */
    itkNewMacro( Self );
    
    /** Dimension of the domain space. */
    itkStaticConstMacro(InputSpaceDimension, unsigned int, NDimensions);
    itkStaticConstMacro(OutputSpaceDimension, unsigned int, NDimensions);
    itkStaticConstMacro(SpaceDimension, unsigned int, NDimensions);
    itkStaticConstMacro(ParametersDimension, unsigned int,
                        NDimensions*(NDimensions+1));

  
    /** Parameters Type   */
    typedef typename Superclass::ParametersType            ParametersType;
    typedef typename Superclass::JacobianType              JacobianType;
    typedef typename Superclass::ScalarType                ScalarType;
    typedef typename Superclass::InputPointType            InputPointType;
    typedef typename Superclass::OutputPointType           OutputPointType;
    typedef typename Superclass::InputVectorType           InputVectorType;
    typedef typename Superclass::OutputVectorType          OutputVectorType;
    typedef typename Superclass::InputVnlVectorType        InputVnlVectorType;
    typedef typename Superclass::OutputVnlVectorType       OutputVnlVectorType;
    typedef typename Superclass::InputCovariantVectorType  InputCovariantVectorType;
    typedef typename Superclass::OutputCovariantVectorType OutputCovariantVectorType;
    typedef typename Superclass::InputTensorType           InputTensorType;
    typedef typename Superclass::OutputTensorType          OutputTensorType;
    typedef typename Superclass::MatrixType                MatrixType;
    typedef typename Superclass::InverseMatrixType         InverseMatrixType;
    typedef typename Superclass::CenterType                CenterType;
    typedef typename Superclass::OffsetType                OffsetType;
    typedef typename Superclass::TranslationType           TranslationType;
        typedef typename Superclass::TransformCategoryType TransformCategoryType;
    

    void Translate(const OutputVectorType &offset, bool pre=0);

    
    void Scale(const OutputVectorType &factor, bool pre=0);
    void Scale(const TScalarType &factor, bool pre=0);
    
    void Rotate(int axis1, int axis2, TScalarType angle, bool pre=0);
    
    void Rotate2D(TScalarType angle, bool pre=0);
    
    void Rotate3D(const OutputVectorType &axis, TScalarType angle, bool pre=0);
    
    void Shear(int axis1, int axis2, TScalarType coef, bool pre=0);
    
    
    inline InputPointType     BackTransform(const OutputPointType  &point ) const;
    inline InputVectorType    BackTransform(const OutputVectorType &vector) const;
    inline InputVnlVectorType BackTransform(const OutputVnlVectorType &vector) const;
    inline InputCovariantVectorType BackTransform(const OutputCovariantVectorType &vector) const;
    inline InputTensorType    BackTransform(const OutputTensorType &vector) const;
    
    inline InputPointType     BackTransformPoint(const OutputPointType  &point) const;


    ScalarType Metric(const Self * other) const;

    
    ScalarType Metric(void) const;

    
    protected:

    AffineTensorTransform(const MatrixType &matrix, const OutputVectorType &offset);
    AffineTensorTransform(unsigned int outputDims, unsigned int paramDims);
    AffineTensorTransform();
    
    /** Destroy an AffineTensorTransform object   **/
    virtual ~AffineTensorTransform();
    
    /** Print contents of an AffineTensorTransform */
    void PrintSelf(std::ostream &s, Indent indent) const;
        
        virtual TransformCategoryType GetTransformCategory() const
        {
            return Self::Linear;
        }
    
    
    private:
    
    AffineTensorTransform(const Self & other);
    const Self & operator=( const Self & );
    
    }; //class AffineTensorTransform


  /** Back transform a tensor */
  template<class TScalarType, unsigned int NDimensions>
    inline
    typename AffineTensorTransform<TScalarType, NDimensions>::InputTensorType
    AffineTensorTransform<TScalarType, NDimensions>::
    BackTransform(const OutputTensorType &tensor ) const 
  {
    itkWarningMacro(<<"BackTransform(): This method is slated to be removed\
   from ITK. Instead, please use GetInverse() to generate an inverse    \
   transform and then perform the transform using that inverted transform.");
    return tensor.SetVnlMatrix ( tensor.ApplyMatrix ( this->GetRigid().GetTranspose() ) );
  }

  
  /** Back transform a vector */
  template<class TScalarType, unsigned int NDimensions>
    inline
    typename AffineTensorTransform<TScalarType, NDimensions>::InputVectorType
    AffineTensorTransform<TScalarType, NDimensions>::
    BackTransform(const OutputVectorType &vect ) const 
  {
    itkWarningMacro(<<"BackTransform(): This method is slated to be removed\
   from ITK. Instead, please use GetInverse() to generate an inverse    \
   transform and then perform the transform using that inverted transform.");
    return this->GetInverseMatrix() * vect;
  }
  
  
  /** Back transform a vnl_vector */
  template<class TScalarType, unsigned int NDimensions>
    inline
    typename AffineTensorTransform<TScalarType, NDimensions>::InputVnlVectorType
    AffineTensorTransform<TScalarType, NDimensions>::
    BackTransform(const OutputVnlVectorType &vect ) const 
  {
    itkWarningMacro(<<"BackTransform(): This method is slated to be removed\
   from ITK. Instead, please use GetInverse() to generate an inverse    \
    transform and then perform the transform using that inverted transform.");
    return this->GetInverseMatrix() * vect;
  }
  
  
  /** Back Transform a CovariantVector */
  template<class TScalarType, unsigned int NDimensions>
    inline
    typename AffineTensorTransform<TScalarType, NDimensions>::InputCovariantVectorType
    AffineTensorTransform<TScalarType, NDimensions>::
    BackTransform(const OutputCovariantVectorType &vec) const 
  {
    itkWarningMacro(<<"BackTransform(): This method is slated to be removed\
   from ITK. Instead, please use GetInverse() to generate an inverse\
   transform and then perform the transform using that inverted transform.");

    InputCovariantVectorType result;    // Converted vector
    
    for (unsigned int i = 0; i < NDimensions; i++) 
    {
      result[i] = NumericTraits<ScalarType>::Zero;
      for (unsigned int j = 0; j < NDimensions; j++) 
      {
        result[i] += this->GetMatrix()[j][i]*vec[j]; // Direct matrix transposed
      }
    }
    return result;
  }
  
  
  /** Back transform a given point which is represented as type PointType */
  template<class TScalarType, unsigned int NDimensions>
    inline
    typename AffineTensorTransform<TScalarType, NDimensions>::InputPointType
    AffineTensorTransform<TScalarType, NDimensions>::
    BackTransformPoint(const OutputPointType &point) const
  {
    return this->BackTransform(point);
  }
  
  /** Back transform a point */
  template<class TScalarType, unsigned int NDimensions>
    inline
    typename AffineTensorTransform<TScalarType, NDimensions>::InputPointType
    AffineTensorTransform<TScalarType, NDimensions>::
    BackTransform(const OutputPointType &point) const 
  {
    itkWarningMacro(<<"BackTransform(): This method is slated to be removed\
   from ITK.  Instead, please use GetInverse() to generate an inverse   \
   transform and then perform the transform using that inverted transform.");
    InputPointType result;       // Converted point
    ScalarType temp[NDimensions];
    unsigned int i, j;
    
    for (j = 0; j < NDimensions; j++) 
    {
      temp[j] = point[j] - this->GetOffset()[j];
    }
    
    for (i = 0; i < NDimensions; i++) 
    {
    result[i] = 0.0;
    for (j = 0; j < NDimensions; j++) 
    {
      result[i] += this->GetInverseMatrix()[i][j]*temp[j];
    }
    }
    return result;
  }
  
}  // namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_AffineTensorTransform(_, EXPORT, x, y) namespace itk { \
    _(2(class EXPORT AffineTensorTransform< ITK_TEMPLATE_2 x >))        \
      namespace Templates { typedef AffineTensorTransform< ITK_TEMPLATE_2 x > AffineTensorTransform##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkAffineTensorTransform+-.h"
#endif

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkAffineTensorTransform.txx"
#endif

#endif /* __itkAffineTensorTransform_h */
