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
#ifndef __itkMatrixOffsetTensorTransformBase_h
#define __itkMatrixOffsetTensorTransformBase_h

#include <iostream>

#include "itkMatrix.h"
#include "itkTensorTransform.h"
#include "itkMacro.h"

namespace itk
{

  
  template <class TScalarType=double, unsigned int NInputDimensions=3, unsigned int NOutputDimensions=3>
    class MatrixOffsetTensorTransformBase 
    : public TensorTransform< TScalarType, NInputDimensions, NOutputDimensions >
    {
      
    public:
    
    /** Standard typedefs   */
    typedef MatrixOffsetTensorTransformBase                                      Self;
    typedef TensorTransform< TScalarType, NInputDimensions, NOutputDimensions >  Superclass;
    typedef SmartPointer<Self>                    Pointer;
    typedef SmartPointer<const Self>              ConstPointer;
  
    /** Run-time type information (and related methods).   */
    itkTypeMacro( MatrixOffsetTensorTransformBase, TensorTransform );

    /** New macro for creation of through a Smart Pointer   */
    itkNewMacro( Self );
    
    /** Dimension of the domain space. */
    itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
    itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
    itkStaticConstMacro(ParametersDimension, unsigned int,
                        NOutputDimensions*(NInputDimensions+1));
    
    
    /** Parameters Type   */
    typedef typename Superclass::ParametersType                  ParametersType;
    
    /** Jacobian Type   */
    typedef typename Superclass::JacobianType                    JacobianType;
    
    /** Standard scalar type for this class */
    typedef typename Superclass::ScalarType                      ScalarType;
    
    /** Standard vector type for this class   */
    typedef Vector<TScalarType,
                  itkGetStaticConstMacro(InputSpaceDimension)>  InputVectorType;
    typedef Vector<TScalarType,
                  itkGetStaticConstMacro(OutputSpaceDimension)> OutputVectorType;
  
    /** Standard covariant vector type for this class   */
    typedef CovariantVector<TScalarType,
                            itkGetStaticConstMacro(InputSpaceDimension)>  
                                                   InputCovariantVectorType;
    typedef CovariantVector<TScalarType,
                            itkGetStaticConstMacro(OutputSpaceDimension)>  
                                                   OutputCovariantVectorType;
  
    /** Standard vnl_vector type for this class   */
    typedef vnl_vector_fixed<TScalarType,
                             itkGetStaticConstMacro(InputSpaceDimension)> 
                                                    InputVnlVectorType;
    typedef vnl_vector_fixed<TScalarType,
                             itkGetStaticConstMacro(OutputSpaceDimension)> 
                                                    OutputVnlVectorType;
  
    /** Standard coordinate point type for this class   */
    typedef Point<TScalarType,
                  itkGetStaticConstMacro(InputSpaceDimension)>   
                                                    InputPointType;
    typedef Point<TScalarType,
                  itkGetStaticConstMacro(OutputSpaceDimension)>  
                                                    OutputPointType;


    /** Standard tensor type for this class   */
    typedef Tensor<TScalarType,
                   itkGetStaticConstMacro(InputSpaceDimension)>   
                                                    InputTensorType;
    typedef Tensor<TScalarType,
                   itkGetStaticConstMacro(OutputSpaceDimension)>  
                                                    OutputTensorType;
  
    /** Standard matrix type for this class   */
    typedef Matrix<TScalarType, itkGetStaticConstMacro(OutputSpaceDimension),
                   itkGetStaticConstMacro(InputSpaceDimension)>  
                                                    MatrixType;

    /** Standard inverse matrix type for this class   */
    typedef Matrix<TScalarType, itkGetStaticConstMacro(InputSpaceDimension),
                   itkGetStaticConstMacro(OutputSpaceDimension)> 
                                                    InverseMatrixType;

    typedef InputPointType                            CenterType;
    
    typedef OutputVectorType                          OffsetType;
    
    typedef OutputVectorType                          TranslationType;
    
    virtual typename Superclass::TransformCategoryType GetTransformCategory() const
    {
      return TransformCategoryType::Linear;
    }

    virtual void SetIdentity( void );
    

    virtual void SetMatrix(const MatrixType &matrix)
    {
      m_Matrix = matrix;
      this->ComputeOffset();
      this->ComputeMatrixParameters();
      this->ComputeRigidPartOfTransform();
      m_MatrixMTime.Modified();
      this->Modified();
      return;
    }

    
    const MatrixType & GetMatrix() const
    { return m_Matrix; }

    
    const MatrixType & GetRigid() const
    { return m_Rigid; }
    
    
    void SetOffset(const OutputVectorType &offset)
    {
      m_Offset = offset;
      this->ComputeTranslation();
      this->Modified();
      return;
    }
    
    
    const OutputVectorType & GetOffset(void) const
    { return m_Offset; }
    
    
    void SetCenter(const InputPointType & center)
    { m_Center = center;
      this->ComputeOffset();
      this->Modified();
      return;
    }

    
    const InputPointType & GetCenter() const
    { return m_Center; }
    
    
    void SetTranslation(const OutputVectorType & translation)
    {
      m_Translation = translation;
      this->ComputeOffset();
      this->Modified();
      return;
    }

    
    const OutputVectorType & GetTranslation(void) const
    { return m_Translation; }
    
    
    void SetParameters( const ParametersType & parameters );
    
    /** Get the Transformation Parameters. */
    const ParametersType& GetParameters(void) const;
    
    /** Set the fixed parameters and update internal transformation. */
    virtual void SetFixedParameters( const ParametersType & );
    
    /** Get the Fixed Parameters. */
    virtual const ParametersType& GetFixedParameters(void) const;
    
    
    void Compose(const Self * other, bool pre=0);
    
    
    OutputPointType     TransformPoint(const InputPointType & point) const;
    OutputVectorType    TransformVector(const InputVectorType & vector) const;
    OutputVnlVectorType TransformVector(const InputVnlVectorType & vector) const;
    OutputVectorType    InverseTransformVector(const InputVectorType & vector) const;
    OutputVnlVectorType InverseTransformVector(const InputVnlVectorType & vector) const;   
    OutputCovariantVectorType TransformCovariantVector(const InputCovariantVectorType &vector) const;
    OutputTensorType    TransformTensor (const InputTensorType& tensor) const;
	OutputTensorType    TransformTensorReverse (const InputTensorType& tensor) const;
    OutputTensorType    TransformTensorWithFS (const InputTensorType& tensor) const;
    OutputTensorType    TransformTensorWithPPD (const InputTensorType& tensor) const;
  

    const JacobianType & GetJacobian(const InputPointType & point ) const;

    
    bool GetInverse(Self * inverse) const;
    
    
    const InverseMatrixType & GetInverseMatrix( void ) const;

    
    virtual bool IsLinear() const { return true; }
    

    protected:
    
    MatrixOffsetTensorTransformBase(const MatrixType &matrix,
                                    const OutputVectorType &offset);
    MatrixOffsetTensorTransformBase(unsigned int outputDims,
                                    unsigned int paramDims);
    MatrixOffsetTensorTransformBase();      
    
    /** Destroy an MatrixOffsetTensorTransformBase object   **/
    virtual ~MatrixOffsetTensorTransformBase();

    /** Print contents of an MatrixOffsetTensorTransformBase */
    void PrintSelf(std::ostream &s, Indent indent) const;

    InverseMatrixType GetVarInverseMatrix( void ) const
    { return m_InverseMatrix; };
    void SetVarInverseMatrix(const InverseMatrixType & matrix) const
    { m_InverseMatrix = matrix; m_InverseMatrixMTime.Modified(); };
    bool InverseMatrixIsOld(void) const
    { if(m_MatrixMTime != m_InverseMatrixMTime)
      { return true; } else { return false; } };

    virtual void ComputeMatrixParameters(void);
    
    virtual void ComputeMatrix(void);
    void SetVarMatrix(const MatrixType & matrix)
    { m_Matrix = matrix; m_MatrixMTime.Modified(); };
    
    virtual void ComputeTranslation(void);
    void SetVarTranslation(const OutputVectorType & translation)
    { m_Translation = translation; };
    
    virtual void ComputeOffset(void);
    void SetVarOffset(const OutputVectorType & offset)
    { m_Offset = offset; };
    
    void SetVarCenter(const InputPointType & center)
    { m_Center = center; };

    /** Pre-compute the rigid part of the transform */
    virtual void ComputeRigidPartOfTransform (void);

    
    private:

    MatrixOffsetTensorTransformBase(const Self & other);
    const Self & operator=( const Self & );
    
    
    MatrixType                  m_Matrix;         // Matrix of the transformation
    OutputVectorType            m_Offset;         // Offset of the transformation
    mutable InverseMatrixType   m_InverseMatrix;  // Inverse of the matrix
    mutable bool                m_Singular;       // Is m_Inverse singular?
    MatrixType                  m_Rigid;          // Rigid part of the linear matrix
    
    InputPointType              m_Center;
    OutputVectorType            m_Translation;
    
    /** To avoid recomputation of the inverse if not needed */
    TimeStamp                   m_MatrixMTime;
    mutable TimeStamp           m_InverseMatrixMTime;
    
    }; //class MatrixOffsetTensorTransformBase
  
}  // namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_MatrixOffsetTensorTransformBase(_, EXPORT, x, y) namespace itk { \
    _(3(class EXPORT MatrixOffsetTensorTransformBase< ITK_TEMPLATE_3 x >)) \
      namespace Templates { typedef MatrixOffsetTensorTransformBase< ITK_TEMPLATE_3 x > MatrixOffsetTensorTransformBase##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkMatrixOffsetTensorTransformBase+-.h"
#endif

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkMatrixOffsetTensorTransformBase.txx"
#endif

#endif /* __itkMatrixOffsetTensorTransformBase_h */

