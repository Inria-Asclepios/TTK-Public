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
#ifndef __itkTensorTransform_h
#define __itkTensorTransform_h

#include "itkTransformBase.h"
#include "itkPoint.h"
#include "itkVector.h"
#include "itkCovariantVector.h"
#include "vnl/vnl_vector_fixed.h"
#include "itkTensor.h"
#include "itkArray.h"
#include "itkArray2D.h"

#include "itkObjectFactory.h"


namespace itk
{
  
  template <class TScalarType,
    unsigned int NInputDimensions=3, 
    unsigned int NOutputDimensions=3>
    class ITK_EXPORT TensorTransform : public TransformBaseTemplate <TScalarType>
    {
    public:
    
    /** Standard class typedefs. */
    typedef TensorTransform  Self;
    typedef TransformBaseTemplate <TScalarType> Superclass;
    typedef SmartPointer< Self >   Pointer;
    typedef SmartPointer< const Self >  ConstPointer;
  
    /** New method for creating an object using a factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro( TensorTransform, TransformBaseTemplate );
    
    /** Dimension of the domain space. */
    itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
    itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
    
    /** Get the size of the input space */
    unsigned int GetInputSpaceDimension(void) const {return NInputDimensions;}
    
    /** Get the size of the output space */
    unsigned int GetOutputSpaceDimension(void) const {return NOutputDimensions;}
    
    /** Type of the scalar representing coordinate and vector elements. */
    typedef  TScalarType     ScalarType;

    /** Type of the input parameters. */
    typedef  typename Superclass::ParametersType ParametersType;
    typedef  typename Superclass::ParametersValueType ParametersValueType;
    typedef  typename Superclass::FixedParametersValueType FixedParametersValueType;

    /** Type of the Jacobian matrix. */
    typedef  Array2D< double >                           JacobianType;

    /** Standard vector type for this class. */
    typedef Vector<TScalarType, NInputDimensions>  InputVectorType;
    typedef Vector<TScalarType, NOutputDimensions> OutputVectorType;
    
    /** Standard covariant vector type for this class */
    typedef CovariantVector<TScalarType, NInputDimensions>  InputCovariantVectorType;
    typedef CovariantVector<TScalarType, NOutputDimensions> OutputCovariantVectorType;
  
    /** Standard vnl_vector type for this class. */
    typedef vnl_vector_fixed<TScalarType, NInputDimensions>  InputVnlVectorType;
    typedef vnl_vector_fixed<TScalarType, NOutputDimensions> OutputVnlVectorType;
  
    /** Standard coordinate point type for this class */
    typedef Point<TScalarType, NInputDimensions>  InputPointType;
    typedef Point<TScalarType, NOutputDimensions> OutputPointType;

    /** Standard tensor type for this class. NEW!! */
    typedef Tensor<TScalarType, NInputDimensions>   InputTensorType;
    typedef Tensor<TScalarType, NOutputDimensions>  OutputTensorType;
  
    /**  Method to transform a point. */
    virtual OutputPointType TransformPoint(const InputPointType  & ) const
    { return OutputPointType(); } 

    /**  Method to transform a vector. */
    virtual OutputVectorType    TransformVector(const InputVectorType &) const
    { return OutputVectorType(); }

    /**  Method to transform a vnl_vector. */
    virtual OutputVnlVectorType TransformVector(const InputVnlVectorType &) const
    { return OutputVnlVectorType(); }

    /**  Method to transform a CovariantVector. */
    virtual OutputCovariantVectorType TransformCovariantVector(
                                                               const InputCovariantVectorType &) const
    { return OutputCovariantVectorType(); }

    /**  Method to transform a Tensor. NEW!! */
    virtual OutputTensorType TransformTensor (const InputTensorType &) const
    { return OutputTensorType(); }
		
	/**  Method to transform a Tensor. NEW!! */
	virtual OutputTensorType TransformTensorReverse (const InputTensorType &) const
	{ return OutputTensorType(); }
    
    /** Set the transformation parameters and update internal transformation.
     * SetParameters gives the transform the option to set it's
     * parameters by keeping a reference to the parameters, or by
     * copying.  To force the transform to copy it's parameters call
     * SetParametersByValue.
     * \sa SetParametersByValue
     */
    virtual void SetParameters( const ParametersType & ) 
    { itkExceptionMacro( << "Subclasses should override this method" ) };
    
    /** Set the transformation parameters and update internal transformation. 
     * This method forces the transform to copy the parameters.  The
     * default implementation is to call SetParameters.  This call must
     * be overridden if the transform normally implements SetParameters
     * by keeping a reference to the parameters.
     * \sa SetParameters
     */
    virtual void SetParametersByValue ( const ParametersType & p ) 
    { this->SetParameters ( p ); };

    /** Get the Transformation Parameters. */
    virtual const ParametersType& GetParameters(void) const
    { itkExceptionMacro( << "Subclasses should override this method" );
      return m_Parameters; };
    
    /** This function allow copying a range of values into the Parameters
      * The range of values must conform to std::copy(begin, end, m_Parameters)
      * requirements.
      */
    void CopyInParameters(const ParametersValueType * const begin,
                          const ParametersValueType * const end);

    /** This function allow copying a range of values into the FixedParameters
      * The range of values must conform to std::copy(begin, end, m_FixedParameters)
      * requirements.
      */
    void CopyInFixedParameters(const FixedParametersValueType * const begin,
                               const FixedParametersValueType * const end);

    /** Set the fixed parameters and update internal transformation. */
    virtual void SetFixedParameters( const ParametersType & ) 
    { itkExceptionMacro( << "Subclasses should override this method" ) };
    
    /** Get the Fixed Parameters. */
    virtual const ParametersType& GetFixedParameters(void) const
    { itkExceptionMacro( << "Subclasses should override this method" );
      return m_Parameters; };
    
    /** Compute the Jacobian of the transformation
     *
     * This method computes the Jacobian matrix of the transformation
     * at a given input point. The rank of the Jacobian will also indicate 
     * if the transform is invertible at this point.
     *
     * The Jacobian is be expressed as a matrix of partial derivatives of the
     * output point components with respect to the parameters that defined
     * the transform:
     *
     * \f[
     *
     J=\left[ \begin{array}{cccc}
     \frac{\partial x_{1}}{\partial p_{1}} & 
     \frac{\partial x_{1}}{\partial p_{2}} & 
     \cdots  & \frac{\partial x_{1}}{\partial p_{m}}\   \
     \frac{\partial x_{2}}{\partial p_{1}} & 
     \frac{\partial x_{2}}{\partial p_{2}} & 
     \cdots  & \frac{\partial x_{2}}{\partial p_{m}}\   \
     \vdots  & \vdots  & \ddots  & \vdots \             \
     \frac{\partial x_{n}}{\partial p_{1}} & 
     \frac{\partial x_{n}}{\partial p_{2}} & 
     \cdots  & \frac{\partial x_{n}}{\partial p_{m}}
     \end{array}\right] 
     *
     * \f]
     * **/
    virtual const JacobianType & GetJacobian(const InputPointType  &) const
    { itkExceptionMacro( << "Subclass should override this method" );
      return m_Jacobian; }; 
    
    
    /** Return the number of parameters that completely define the Transfom  */
    virtual typename Superclass::NumberOfParametersType GetNumberOfParameters(void) const
    { return m_Parameters.Size(); }
    
    /** Return the inverse of the transform.
     *  The inverse is recomputed if it has been modified */
    bool GetInverse(Self*) const {return false;}
    
    /** Generate a platform independant name */
    virtual std::string GetTransformTypeAsString() const;
    
    /** Indicates if this transform is linear. A transform is defined to be
     * linear if the transform of a linear combination of points is equal to the
     * linear combination (with the same coefficients) of the individual
     * transforms of each point. The transform T will be linear if given two
     * points P and Q, and scalar coefficients a and b, then
     *
     *           T( a*P + b*Q ) = a * T(P) + b * T(Q)
     * 
     * By default, we assume this to NOT be the case for most transforms.
     * However, transforms for which this is true will overload and reimplement
     * this method accordingly.
     * 
     **/
    virtual bool IsLinear() const { return false; }
        
    typedef typename Superclass::TransformCategoryType TransformCategoryType;
    
    virtual TransformCategoryType GetTransformCategory() const
    {
      return Self::UnknownTransformCategory;
    }
    
    protected:
    TensorTransform(); 
    TensorTransform(unsigned int Dimension, unsigned int NumberOfParameters);
    virtual ~TensorTransform() {};
    
    
    mutable ParametersType     m_Parameters;
    mutable ParametersType     m_FixedParameters;
    mutable JacobianType       m_Jacobian;
    
    private:
    TensorTransform(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented
    
    
    };
  
} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_TensorTransform(_, EXPORT, x, y) namespace itk {   \
    _(3(class EXPORT TensorTransform< ITK_TEMPLATE_3 x >))              \
      namespace Templates { typedef TensorTransform< ITK_TEMPLATE_3 x > TensorTransform##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkTensorTransform+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkTensorTransform.txx"
#endif

#endif



