#ifndef __itkIdentityTensorTransform_h
#define __itkIdentityTensorTransform_h

#include "itkObject.h"
#include "itkPoint.h"
#include "itkVector.h"
#include "itkCovariantVector.h"
#include "vnl/vnl_vector_fixed.h"
#include "itkArray.h"
#include "itkArray2D.h"
#include "itkTensorTransform.h"

#include "itkObjectFactory.h"


namespace itk
{
  
  
  template <class TScalarType,
    unsigned int NDimensions=3>
    class ITK_EXPORT  IdentityTensorTransform
    : public TensorTransform<TScalarType,NDimensions,NDimensions>
    {
      
    public:
    
    /** Standard class typedefs. */
    typedef IdentityTensorTransform  Self;
    typedef TensorTransform<TScalarType,NDimensions,NDimensions> Superclass;
    typedef SmartPointer< Self >   Pointer;
    typedef SmartPointer< const Self >  ConstPointer;
    
    /** New method for creating an object using a factory. */
    itkNewMacro(Self);
    
    /** Run-time type information (and related methods). */
    itkTypeMacro( IdentityTensorTransform, TensorTransform );

    /** Dimension of the domain space. */
    itkStaticConstMacro(InputSpaceDimension, unsigned int, NDimensions);
    itkStaticConstMacro(OutputSpaceDimension, unsigned int, NDimensions);
    
    /** Type of the input parameters. */
    typedef  TScalarType     ScalarType;
    
    /** Type of the input parameters. */
    typedef  typename Superclass::ParametersType                 ParametersType;
    
    /** Type of the Jacobian matrix. */
    typedef  typename Superclass::JacobianType                   JacobianType;
    
    /** Standard vector type for this class. */
    typedef Vector<TScalarType,
    itkGetStaticConstMacro(InputSpaceDimension)>  InputVectorType;

    typedef Vector<TScalarType,
    itkGetStaticConstMacro(OutputSpaceDimension)> OutputVectorType;
  

    /** Standard covariant vector type for this class */
    typedef CovariantVector<TScalarType,
    itkGetStaticConstMacro(InputSpaceDimension)>  InputCovariantVectorType;

    typedef CovariantVector<TScalarType,
    itkGetStaticConstMacro(OutputSpaceDimension)> OutputCovariantVectorType;
  

    /** Standard vnl_vector type for this class. */
    typedef vnl_vector_fixed<TScalarType,
    itkGetStaticConstMacro(InputSpaceDimension)>  InputVnlVectorType;

    typedef vnl_vector_fixed<TScalarType,
    itkGetStaticConstMacro(OutputSpaceDimension)> OutputVnlVectorType;

    
    /** Standard coordinate point type for this class */
    typedef Point<TScalarType,
    itkGetStaticConstMacro(InputSpaceDimension)> InputPointType;

    typedef Point<TScalarType,
    itkGetStaticConstMacro(OutputSpaceDimension)> OutputPointType;

    
    /** Standard tensor type for this class. NEW!! */
    typedef Tensor<TScalarType,
    itkGetStaticConstMacro(InputSpaceDimension)>   InputTensorType;
    
    typedef Tensor<TScalarType,
    itkGetStaticConstMacro(OutputSpaceDimension)>  OutputTensorType;

    
    /**  Method to transform a point. */
    virtual OutputPointType TransformPoint(const InputPointType  &point ) const
    { return point; }

    
    /**  Method to transform a vector. */
    virtual OutputVectorType TransformVector(const InputVectorType &vector) const
    { return vector; }

    
    /**  Method to transform a vnl_vector. */
    virtual OutputVnlVectorType TransformVector(const InputVnlVectorType &vector) const
    { return vector; }

    
    /**  Method to transform a CovariantVector. */
    virtual OutputCovariantVectorType TransformCovariantVector(
    const InputCovariantVectorType &vector) const
    { return vector; }


    /**  Method to transform a Tensor. NEW!! */
    virtual OutputTensorType TransformTensor (const InputTensorType &tensor) const
    { return tensor; }


    
    /** Set the transformation to an Identity
     *
     * This is a NULL operation in the case of this particular transform.
     The method is provided only to comply with the interface of other transforms. */
    void SetIdentity( void ) { }
    

    
    /** Set the Transformation Parameters
     * and update the internal transformation. */
    virtual void SetParameters(const ParametersType &) {};

    
    /** Compute the Jacobian of the transformation
     *
     * This method computes the Jacobian matrix of the transformation.
     * given point or vector, returning the transformed point or
     * vector. The rank of the Jacobian will also indicate if the transform
     * is invertible at this point.
     *
     * The Jacobian can be expressed as a set of partial derivatives of the
     * output point components with respect to the parameters that defined
     * the transform:
     *
     * \f[
     *
     J=\left[ \begin{array}{cccc}
     \frac{\partial x_{1}}{\partial p_{1}} & 
     \frac{\partial x_{2}}{\partial p_{1}} & 
     \cdots  & \frac{\partial x_{n}}{\partial p_{1}}\   \
     \frac{\partial x_{1}}{\partial p_{2}} & 
     \frac{\partial x_{2}}{\partial p_{2}} & 
     \cdots  & \frac{\partial x_{n}}{\partial p_{2}}\   \
     \vdots  & \vdots  & \ddots  & \vdots \             \
     \frac{\partial x_{1}}{\partial p_{m}} & 
     \frac{\partial x_{2}}{\partial p_{m}} & 
     \cdots  & \frac{\partial x_{n}}{\partial p_{m}}
     \end{array}\right] 
     *
     * \f]
     * **/
    virtual const JacobianType & GetJacobian(const InputPointType  & ) const
    { 
      this->m_Jacobian = JacobianType(NDimensions,1); 
      this->m_Jacobian.Fill(0.0); 
      return this->m_Jacobian;
    }

    
    /** Indicates that this transform is linear. That is, given two
     * points P and Q, and scalar coefficients a and b, then
     *
     *           T( a*P + b*Q ) = a * T(P) + b * T(Q)
     */
    virtual bool IsLinear() const { return true; }

    
    protected:

    IdentityTensorTransform():TensorTransform<TScalarType,NDimensions,NDimensions>(NDimensions,1) {}; 
    virtual ~IdentityTensorTransform() {};
    
    
    private:
    IdentityTensorTransform(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented
    
    
    };
  
} // end namespace itk


#endif



