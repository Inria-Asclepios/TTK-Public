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
#ifndef __itkTensor_h
#define __itkTensor_h

#include "itkFixedArray.h"
#include "itkNumericTraits.h"
#include "itkIndent.h"
#include "itkVector.h"
#include "itkMatrix.h"

namespace itk
{

  /*! \class Tensor itkTensor.h itkTensor.h
    \ingroup TensorToolKit
    An itk templated class to handle tensors, do eigendecomposition and so on.
    This class is interfaced with (either): vnl, MKL or ACML.
   */

  template<class T, unsigned int NDimension>
    class Tensor : public FixedArray<T,NDimension*(NDimension+1)/2>
  {

    public:
    /** Standard typedefs */
    using Self = Tensor;

    itkStaticConstMacro (NDegreesOfFreedom, unsigned int, NDimension*(NDimension+1)/2);
    //static const unsigned int NDegreesOfFreedom = NDimension*(NDimension+1)/2;

    using Superclass = FixedArray<T,NDegreesOfFreedom>;

    /** ValueType can be used to declare a variable that is the same type
     * as a data element held in an Vector.   */
    using ValueType     = T;
    using RealType      = typename NumericTraits< ValueType >::RealType;
    using RealValueType = typename NumericTraits< ValueType >::RealType;
    using VectorType    = Vector<T, NDimension>;
    using MatrixType    = Matrix<T, NDimension, NDimension>;

    /** Dimension of the tensor space. */
    itkStaticConstMacro(Dimension, unsigned int, NDimension);
    /** Number of DOFs */
    itkStaticConstMacro(DegreesOfFreedom, unsigned int, NDegreesOfFreedom);

    using TensorType = Self;

    /** Component value type */
    using ComponentType = T;

    /** The Array type from which this vector is derived. */
    using  BaseArray = FixedArray<T, NDegreesOfFreedom>;

    /** itk Vectors are used to store eigenvectors */
    //    typedef Vector<T, NDimension>             VectorType;

    /** Get the dimension of the tensor. */
    inline static unsigned int GetTensorDimension()
    {return NDimension;}

    /** Set a vnl_matrix_ref referencing the same memory block. */
    inline void SetVnlMatrix( const vnl_matrix<T> & );

    /** Get a vnl_matrix with a copy of the internal memory  block. */
    inline vnl_matrix<T> GetVnlMatrix () const;

    /** Defaut constructor and copy constructors. */
    inline Tensor(): BaseArray(){};
    inline Tensor (const ValueType& r);
    /** Initialize the tensor with the tensor product of v: T = v*v'. */
    inline Tensor (const VectorType& v);


    /** Pass-through constructor for the Array base class. */
    inline Tensor (const Self& r): BaseArray (r){}
    template< class TTensorValueType >
      inline Tensor(const Tensor< TTensorValueType, NDimension>& v): BaseArray(v) {}
    inline Tensor (const ValueType r[NDegreesOfFreedom]): BaseArray (r){}

    /** Pass-through assignment operator for the Array base class. */
    inline Tensor& operator= (const Self& r);
    inline Tensor& operator= (const ValueType r[NDegreesOfFreedom]);

    /** Scalar operator*=.  Scales elements by a scalar. */
    inline const Self& operator*=(const ValueType &value);

    /** Scalar operator/=.  Scales (divides) elements by a scalar. */
    inline const Self& operator/=(const ValueType &value);

    /** Tensor operator+=.  Adds a tensor to the current tensor. */
    inline const Self& operator+=(const Self &t);

    /** Vector operator-=.  Subtracts a tensor from a current tensor. */
    inline const Self& operator-=(const Self &t);

    /** Tensor multiplication by a vector - returns a vector*/
    inline VectorType operator*(const VectorType&) const;

    /** Inner product: returns trace(this*A). Also known as the scalar product. */
    /**
       Matrix multiplication. BE CAREFUL: the multiplication of two tensors
       does not give in general a tensor as a result. An exception will be
       thrown in that case. To be used only when one is sure that the two
       matrices commute (like multiplication with a diagonal matrix).
    */
    inline Self operator*(const Self& A) const;

   /** Tensor negation.  Negate all the elements of a tensor. Return a new
    *  tensor */
   //Self operator-() const;

   /** Tensor addition. Add two tensors. Return a new tensor. */
   inline Self operator+(const Self &t) const;

   /** Tensor subtraction. Subtract two tensors. Return a new tensor. */
   inline Self operator-(const Self &t) const;

   /** Tensor multiplication.  Performs the product of two tensors.
    *  Return a new tensor. */
   //Self operator*(const Self &t) const;

   /** Scalar operator*. Scale the elements of a tensor by a scalar.
    * Return a new tensor. */
   inline Self operator*(const ValueType& val) const;

   /** Scalar operator/. Scale (divide) the elements of a tensor by a scalar.
    * Return a new tensor. */
   inline Self operator/(const ValueType& val) const;

  /** Operators == and != compare a tensor component by component. All
   * components must be equal for two tensors to be equal. (Of course
   * compile-time constraints on the template parameters length and type
   * prevent comparisons between tensors of different type and length.) */
   inline bool operator==(const Self& t) const
   { return Superclass::operator==(t); }
   inline bool operator!=(const Self& t) const
   { return !operator==(t); }

  /** Returns the Froebenius Norm of the tensor  */
  inline RealValueType GetNorm(  ) const;

  /** Returns tensor's Squared Euclidean Norm  */
  inline RealValueType GetSquaredNorm(  ) const;

  /** Returns the number of independent components in this tensor type */
  inline static int GetNumberOfComponents() { return NDegreesOfFreedom;}

  // Divides the tensor components by the tensor norm */
  //void Normalize(void);

  /** Set the Nth component */
  inline void SetNthComponent(int c, const ComponentType& v)
    {  this->operator[](c) = v; }

  /** Get the Nth component*/
  inline ValueType GetNthComponent (int c) const
  { return this->operator[](c); }

  /** Set the Nth component in vectorial mode*/
  inline void SetNthComponentAsVector (int c, const ComponentType& v);

  /** Get the Nth component in vectorial mode*/
  inline ValueType GetNthComponentAsVector (int c) const;

  /** Set the (i,j) component of the tensor. */
  inline void SetComponent (int i, int j, const ComponentType &v);

  /** Get the (i,j) component */
  inline ValueType GetComponent (int, int) const;

  // OTHERS OPERATORS HERE: (i,j)

  /** Return the matrix logarithm */
  inline Self Log() const;

  /** Return the matrix exponential */
  inline Self Exp () const;

  /** Power of the tensor */
  inline Self Pow (double n) const;

  /** Inverse of the tensor */
  inline Self Inv () const;

  /** Square root of the tensor */
  inline Self Sqrt () const;

  /** Inverse square root of the tensor */
  inline Self InvSqrt () const;

  /** Get a specific eigenvalue in ascending order */
  inline T GetEigenvalue (unsigned int n) const;

  /** Get a specific eigenvector in ascending order */
  inline VectorType GetEigenvector (unsigned int n) const;

  /** Get determinant of the tensor */
  inline T GetDeterminant () const;

  /** Test wether the tensor is null or not */
  inline bool IsZero () const;

  /** Test wether the tensor is positive or not. */
  inline bool IsPositive () const;

  /** Test wether the tensor is negative or not. */
  inline bool IsNegative () const;

  /** Test wether the tensor is finite or not. */
  inline bool IsFinite () const;

  /** Test wether the tensor has nans or not. */
  inline bool HasNans () const;
  
  /** Compute the scalar product with another tensor: < T1 | T2 >  = trace (T1*T2) */
  inline ValueType ScalarProductWith (const Self&) const;

  /** Compute the scalar product with a vector: < T | t >  = t'*T*t */
  inline ValueType ScalarProductWith (const VectorType&) const;

  /** Compute the differential of the exponential taken at T. */
  inline Self DifferentialExp (const Self&) const;

  /** Compute the trace */
  inline ValueType GetTrace () const;

  /** Compute the FA */
  inline ValueType GetFA () const;

  /** Compute the GA */
  inline ValueType GetGA () const;

  /** Compute the RA */
  inline ValueType GetRA () const;

  /** Compute the VR */
  inline ValueType GetVR () const;

  /** Compute the Cl coeff */
  inline ValueType GetCl() const;

  /** Compute the Cp coeff */
  inline ValueType GetCp() const;

  /** Compute the Cs coeff */
  inline ValueType GetCs() const;

  /** Transform the tensor with a matrix */
  inline Self ApplyMatrix (const MatrixType&) const;

  /** Transform the tensor with a matrix */
  inline vnl_matrix<T> ApplyMatrix (const vnl_matrix<T> &) const;

  /** Compute the differential of the logarithm taken at T. */
  //inline Self DifferentialLog (const Self& T);

  /** Copy from another Vector with a different representation type.
   *  Casting is done with C-Like rules  */
  template < typename TCoordRepB >
  inline void CastFrom( const Tensor<TCoordRepB,NDimension> & pa )
  {
    for(unsigned int i=0; i<NDegreesOfFreedom; i++ )
    {
      (*this)[i] = static_cast<T>( pa[i] );
    }
  }


  protected:

  double sx ( const double&, const double&) const;


  };


  /** Premultiply Operator for product of a tensor and a scalar.
   *  Tensor< T, N >  =  T * Tensor< T,N > */
  template< class T, unsigned int NDimension >
    inline
    Tensor<T,NDimension>
    operator*(const T &scalar, const Tensor<T,NDimension> & t)
  {
    return t * scalar;
  }


  template <typename T, unsigned int NDimension>
    std::ostream & operator<<(std::ostream &os, const Tensor<T,NDimension> &t)
  {
    os << "[";
    for (int j=0; j< static_cast<int>(NDimension); j++)
    {
      for(int i=0; i<=j ; i++)
      {
        if(j==0)
          os << t[ j*(j+1)/2 + i ];
        else
          os << " " << t[ j*(j+1)/2 + i ];
        if( j==static_cast<int>(NDimension)-1 && i==static_cast<int>(NDimension)-1)
      os << "]" << std::endl;
      }
      for(int i=j+1; i< static_cast<int>(NDimension); i++)
         os << " " << t[ i*(i+1)/2 + j];
      os << std::endl;
    }
    return os;
  }

} // end of namespace itk

#include "itkNumericTraitsTensorPixel2.h"

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTensor.txx"
#endif

#endif
