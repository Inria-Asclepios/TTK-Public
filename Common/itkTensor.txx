#ifndef _itkTensor_txx
#define _itkTensor_txx

#include "itkTensor.h"
#include <vnl/vnl_math.h>
#include "vnl/vnl_vector.h"
#include "vnl/vnl_trace.h"
#include "vnl/algo/vnl_determinant.h"
#include "itkObject.h"

#include <math.h>

#include "ttkConfigure.h"

#ifdef TTK_USE_MKL
  #include <mkl_lapack.h>
#else
#ifdef TTK_USE_ACML
  #include <acml.h>
#else
  #include <vnl/algo/vnl_symmetric_eigensystem.h>
#endif
#endif

namespace itk
{


  /** Constructor */
  template<class T, unsigned int NDimension>
  Tensor<T, NDimension>
  ::Tensor (const ValueType& r)
  {
    for(typename BaseArray::Iterator i = BaseArray::Begin(); i!=BaseArray::End(); i++)
    {
      *i = static_cast<T>(0.0);
    }

    for( unsigned int i=0; i<NDimension; i++)
    {
      (*this)[i*(i+3)/2] = r;
    }


  }


  /** Constructor */
  template<class T, unsigned int NDimension>
  Tensor<T, NDimension>
  ::Tensor (const VectorType& v)
  {
    for( unsigned int nc=0; nc<NDimension; nc++ )
    {
      for( unsigned int nl=0; nl<=nc; nl++ )
        this->SetComponent(nl, nc, v[nl]*v[nc]);
    }

  }



  template<class T, unsigned int NDimension>
  Tensor<T, NDimension>&
  Tensor<T, NDimension>
  ::operator=(const Self& r)
  {
    BaseArray::operator=(r);
    return *this;
  }


  template<class T, unsigned int NDimension>
  Tensor<T, NDimension>&
  Tensor<T, NDimension>
  ::operator=(const ValueType r[NDegreesOfFreedom])
  {
    BaseArray::operator=(r);
    return *this;
  }


  template<class T, unsigned int NDimension>
  const Tensor<T, NDimension>&
  Tensor<T, NDimension>
  ::operator*=(const ValueType &value)
  {
    for( unsigned int i=0; i<NDegreesOfFreedom; i++)
    {
    (*this)[i] *= value;
    }
    return *this;
  }

  template<class T, unsigned int NDimension>
  const Tensor<T, NDimension>&
  Tensor<T, NDimension>
  ::operator/=(const ValueType &value)
  {
    for( unsigned int i=0; i<NDegreesOfFreedom; i++)
    {
    (*this)[i] /= value;
    }
    return *this;
  }


  template<class T, unsigned int NDimension>
  const Tensor<T, NDimension>&
  Tensor<T, NDimension>
  ::operator+=(const Self &t)
  {
    for( unsigned int i=0; i<NDegreesOfFreedom; i++)
    {
      (*this)[i] += t[i];
    }
    return *this;
  }


  template<class T, unsigned int NDimension>
  const Tensor<T, NDimension>&
  Tensor<T, NDimension>
  ::operator-=(const Self &t)
  {
    for( unsigned int i=0; i<NDegreesOfFreedom; i++)
    {
      (*this)[i] -= t[i];
    }
    return *this;
  }


  template<class T, unsigned int NDimension>
  Tensor<T, NDimension>
  Tensor<T, NDimension>
  ::operator+(const Self &t) const
  {
    Self result;
    for( unsigned int i=0; i<NDegreesOfFreedom; i++)
    {
      result[i] = (*this)[i] + t[i];
    }
    return result;
  }


  template<class T, unsigned int NDimension>
  Tensor<T, NDimension>
  Tensor<T, NDimension>
  ::operator-(const Self &t) const
  {
    Self result;
    for( unsigned int i=0; i<NDegreesOfFreedom; i++)
    {
      result[i] = (*this)[i] - t[i];
    }
    return result;
  }


  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::Self
  Tensor<T, NDimension>
  ::operator*(const Self &t) const
  {
    Self res;
    res.SetVnlMatrix ( this->GetVnlMatrix() * t.GetVnlMatrix() );
    return res;
    //return this->ScalarProductWith (t);
  }


  template<class T, unsigned int NDimension>
  Tensor<T, NDimension>
  Tensor<T, NDimension>
  ::operator*(const ValueType& v) const
  {
    Self result;
    for( unsigned int i=0; i<NDegreesOfFreedom; i++)
    {
      result[i] = (*this)[i]*v;
    }
    return result;
  }


  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::VectorType
  Tensor<T, NDimension>
  ::operator*(const VectorType& v) const
  {
    VectorType result;

    for( unsigned int i=0; i<NDimension; i++ )
    {
      result[i]=0;
      for( unsigned int j=0; j<NDimension; j++)
        result[i] += this->GetComponent(i,j)*v[j];
    }


    return result;
  }



  template<class T, unsigned int NDimension>
  Tensor<T, NDimension>
  Tensor<T, NDimension>
  ::operator/(const ValueType& v) const
  {
    Self result;
    for( unsigned int i=0; i<NDegreesOfFreedom; i++)
    {
      result[i] = (*this)[i]/v;
    }
    return result;
  }


  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::RealValueType
  Tensor<T, NDimension>
  ::GetSquaredNorm( void ) const
  {
    typename NumericTraits<RealValueType>::AccumulateType sum = NumericTraits<T>::Zero;

    // diagonal elements:
    for( unsigned int j=0; j<NDimension; j++ )
    {
      ValueType value = this->GetNthComponent (j*(j+1)/2 + j);
      sum += value*value;
    }

    // off-diagonal elements
    for( unsigned int j=1; j<NDimension; j++ )
    {
      for( unsigned int i=0; i<j; i++ )
      {
        ValueType value = this->GetComponent (i,j);
        sum += 2.0*value*value;
      }

    }


    return sum;
  }


  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::RealValueType
  Tensor<T, NDimension>
  ::GetNorm( void ) const
  {
    return RealValueType( vcl_sqrt( double(this->GetSquaredNorm()) ));
  }


  template<class T, unsigned int NDimension>
  void
  Tensor<T, NDimension>
  ::SetComponent (int i, int j, const ComponentType &v)
  {
    if( i < j )
      this->SetNthComponent(j*(j+1)/2+i,v);
    else
      this->SetNthComponent (i*(i+1)/2+j,v);
  }


  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::ValueType
  Tensor<T, NDimension>
  ::GetComponent (int i, int j) const
  {
    if(j>i)
      return (*this)[j*(j+1)/2+i];
    else
      return (*this)[i*(i+1)/2+j];
  }


  template<class T, unsigned int NDimension>
  void
  Tensor<T, NDimension>
  ::SetVnlMatrix( const vnl_matrix<T> & mat)
  {
    for(unsigned int j=0;j<NDimension;j++)
    {
      for(unsigned int i=0;i<=j;i++)
        (*this)[j*(j+1)/2+i] = mat(i,j);
    }

  }


  template<class T, unsigned int NDimension>
  vnl_matrix<T>
  Tensor<T, NDimension>
  ::GetVnlMatrix(void) const
  {

    T* block = new T[NDimension*NDimension];
    for(unsigned int j=0;j<NDimension;j++)
    {
      for(unsigned int i=0;i<NDimension;i++)
        block[NDimension*j+i] = this->GetComponent (i,j);
    }

    vnl_matrix< T > result = vnl_matrix< T >(block, NDimension, NDimension);
    delete [] block;
    return result;

  }


  template<class T, unsigned int NDimension>
  Tensor<T,NDimension>
  Tensor<T,NDimension>
  ::Log (void) const
  {
    if (!this->IsFinite())
    {
      throw ExceptionObject (__FILE__,__LINE__,"Tensor is not finite");
    }
    Self result;
#if defined (TTK_USE_MKL) || defined (TTK_USE_ACML)
    char job = 'V';
    char uplo = 'U';
    double *ev  = new double[NDimension];
    double *work = new double[NDimension*NDimension];
    int ldz = NDimension;
    double *z = new double[NDimension*NDimension];
    int info = 0;
    int order = NDimension;

    // need to copy
    double *buffer = new double[NDegreesOfFreedom];
    for(unsigned int i=0;i<NDegreesOfFreedom;i++)
      buffer[i] = static_cast<double>((*this)[i]);

#ifdef TTK_USE_MKL
    dspev(&job,&uplo,&order,buffer,ev,z,&ldz,work,&info);
#else // ACML
    dspev(job,uplo,order,buffer,ev,z,ldz,&info);
#endif

    // logarithm of the eigenvalues:
    for(unsigned int i=0;i<NDimension;i++)
    {
      if( ev[i] < 0.0 )
      {
        //std::cerr << (*this) << std::endl;        // not thread safe
        throw itk::ExceptionObject (__FILE__,__LINE__,"Error: negative eigenvalue encountered.");
      }
      ev[i] = vcl_log (ev[i]);
    }


    // reconstitute the tensor
    for(unsigned int j=0;j<NDimension;j++)
    {
      for(unsigned int i=0;i<NDimension;i++)
      {
        double res = 0.0;
        for(unsigned int n=0;n<NDimension;n++)
          res += ev[n]*z[n*NDimension+i]*z[n*NDimension+j];

        result[j*(j+1)/2+i] = static_cast<T>(res);

      }
    }

    // free the buffers
    delete [] ev;
    delete [] work;
    delete [] z;
    delete [] buffer;

#else
    typedef vnl_symmetric_eigensystem< T >  SymEigenSystemType;
    SymEigenSystemType eig (this->GetVnlMatrix());
    for(unsigned int i=0;i<NDimension;i++)
    {
      if (eig.D[i] < 0.0 )
      {
        //std::cerr << (*this) << std::endl;        // not thread safe
        throw itk::ExceptionObject (__FILE__,__LINE__,"Error: negative eigenvalue encountered.");
      }
      eig.D[i] = vcl_log (eig.D[i]);
    }

    result.SetVnlMatrix ( eig.recompose() );

#endif
    return result;

  }



  template<class T, unsigned int NDimension>
  Tensor<T,NDimension>
  Tensor<T,NDimension>
  ::Exp (void) const
  {
    if (!this->IsFinite())
    {
      throw ExceptionObject (__FILE__,__LINE__,"Tensor is not finite");
    }
    Self result;
#if defined (TTK_USE_MKL) || defined (TTK_USE_ACML)
    char job = 'V';
    char uplo = 'U';
    double *ev  = new double[NDimension];
    double *work = new double[NDimension*NDimension];
    int ldz = NDimension;
    double *z = new double[NDimension*NDimension];
    int info = 0;
    int order = NDimension;

    // need to copy
    double *buffer = new double[NDegreesOfFreedom];
    for(unsigned int i=0;i<NDegreesOfFreedom;i++)
      buffer[i] = static_cast<double>((*this)[i]);

#ifdef TTK_USE_MKL
    dspev(&job,&uplo,&order,buffer,ev,z,&ldz,work,&info);
#else // ACML
    dspev(job,uplo,order,buffer,ev,z,ldz,&info);
#endif

    // exponential of the eigenvalues:
    for(unsigned int i=0;i<NDimension;i++)
      ev[i] = vcl_exp (ev[i]);

    // reconstitute the tensor
    for(unsigned int j=0;j<NDimension;j++)
    {
      for(unsigned int i=0;i<NDimension;i++)
      {
        double res = 0.0;
        for(unsigned int n=0;n<NDimension;n++)
          res += ev[n]*z[n*NDimension+i]*z[n*NDimension+j];

        result[j*(j+1)/2+i] = static_cast<T>(res);

      }
    }

    // free the buffers
    delete [] ev;
    delete [] work;
    delete [] z;
    delete [] buffer;

#else
    typedef vnl_symmetric_eigensystem< T >  SymEigenSystemType;
    SymEigenSystemType eig(this->GetVnlMatrix());
    for(unsigned int i=0;i<NDimension;i++)
      eig.D[i] = vcl_exp (eig.D[i]);
    result.SetVnlMatrix ( eig.recompose() );

#endif
    return result;

  }


  template<class T, unsigned int NDimension>
  Tensor<T,NDimension>
  Tensor<T,NDimension>
  ::Pow (double n) const
  {
    if (!this->IsFinite())
    {
      throw ExceptionObject (__FILE__,__LINE__,"Tensor is not finite");
    }
    Self result;
#if defined (TTK_USE_MKL) || defined (TTK_USE_ACML)
    char job = 'V';
    char uplo = 'U';
    double *ev  = new double[NDimension];
    double *work = new double[NDimension*NDimension];
    int ldz = NDimension;
    double *z = new double[NDimension*NDimension];
    int info = 0;
    int order = NDimension;

    // need to copy
    double *buffer = new double[NDegreesOfFreedom];
    for(unsigned int i=0;i<NDegreesOfFreedom;i++)
      buffer[i] = static_cast<double>((*this)[i]);

#ifdef TTK_USE_MKL
    dspev(&job,&uplo,&order,buffer,ev,z,&ldz,work,&info);
#else // ACML
    dspev(job,uplo,order,buffer,ev,z,ldz,&info);
#endif

    // power of the eigenvalues:
    for(unsigned int i=0;i<NDimension;i++)
      ev[i] = vcl_pow (ev[i], n);

    // reconstitute the tensor
    for(unsigned int j=0;j<NDimension;j++)
    {
      for(unsigned int i=0;i<NDimension;i++)
      {
        double res = 0.0;
        for(unsigned int n=0;n<NDimension;n++)
          res += ev[n]*z[n*NDimension+i]*z[n*NDimension+j];

        result[j*(j+1)/2+i] = static_cast<T>(res);

      }
    }

    // free the buffers
    delete [] ev;
    delete [] work;
    delete [] z;
    delete [] buffer;

#else
    typedef vnl_symmetric_eigensystem< T >  SymEigenSystemType;
    SymEigenSystemType eig (this->GetVnlMatrix());
    for(unsigned int i=0;i<NDimension;i++)
      eig.D[i] = static_cast<T> (vcl_pow (static_cast<double>(eig.D[i]), n));
    result.SetVnlMatrix ( eig.recompose() );

#endif
    return result;
  }


  template<class T, unsigned int NDimension>
  Tensor<T,NDimension>
  Tensor<T,NDimension>
  ::Inv (void) const
  {
    return this->Pow (-1.0);
  }


  template<class T, unsigned int NDimension>
  Tensor<T,NDimension>
  Tensor<T,NDimension>
  ::Sqrt (void) const
  {
    return this->Pow (0.5);
  }


  template<class T, unsigned int NDimension>
  Tensor<T,NDimension>
  Tensor<T,NDimension>
  ::InvSqrt (void) const
  {
    return this->Pow (-0.5);
  }


  template<class T, unsigned int NDimension>
  T
  Tensor<T,NDimension>
  ::GetEigenvalue (unsigned int n) const
  {
    if (!this->IsFinite())
    {
      throw ExceptionObject (__FILE__,__LINE__,"Tensor is not finite");
    }
    T result;
#if defined (TTK_USE_MKL) || defined (TTK_USE_ACML)
    char job = 'V';
    char uplo = 'U';
    double *ev  = new double[NDimension];
    double *work = new double[NDimension*NDimension];
    int ldz = NDimension;
    double *z = new double[NDimension*NDimension];
    int info = 0;
    int order = NDimension;

    // need to copy
    double *buffer = new double[NDegreesOfFreedom];
    for(unsigned int i=0;i<NDegreesOfFreedom;i++)
      buffer[i] = static_cast<double>((*this)[i]);

#ifdef TTK_USE_MKL
    dspev(&job,&uplo,&order,buffer,ev,z,&ldz,work,&info);
#else // ACML
    dspev(job,uplo,order,buffer,ev,z,ldz,&info);
#endif

    result = static_cast<T>(ev[n]);

    // free the buffers
    delete [] ev;
    delete [] work;
    delete [] z;
    delete [] buffer;

#else
    typedef vnl_symmetric_eigensystem< T >  SymEigenSystemType;
    SymEigenSystemType eig (this->GetVnlMatrix());
    result = eig.D[n];


#endif
    return result;

  }



  template<class T, unsigned int NDimension>
  typename Tensor<T,NDimension>::VectorType
  Tensor<T,NDimension>
  ::GetEigenvector (unsigned int n) const
  {
    if (!this->IsFinite())
    {
      throw ExceptionObject (__FILE__,__LINE__,"Tensor is not finite");
    }
    VectorType result;
#if defined (TTK_USE_MKL) || defined (TTK_USE_ACML)
    char job = 'V';
    char uplo = 'U';
    double *ev  = new double[NDimension];
    double *work = new double[NDimension*NDimension];
    int ldz = NDimension;
    double *z = new double[NDimension*NDimension];
    int info = 0;
    int order = NDimension;

    // need to copy
    double *buffer = new double[NDegreesOfFreedom];
    for(unsigned int i=0;i<NDegreesOfFreedom;i++)
      buffer[i] = static_cast<double>((*this)[i]);

#ifdef TTK_USE_MKL
    dspev(&job,&uplo,&order,buffer,ev,z,&ldz,work,&info);
#else // ACML
    dspev(job,uplo,order,buffer,ev,z,ldz,&info);
#endif

    for(unsigned int i=0;i<NDimension;i++)
      result[i] = z[n*NDimension + i];

    // free the buffers
    delete [] ev;
    delete [] work;
    delete [] z;
    delete [] buffer;

#else
    typedef vnl_symmetric_eigensystem< T >  SymEigenSystemType;
    SymEigenSystemType eig (this->GetVnlMatrix());
    for(unsigned int i=0;i<NDimension;i++)
      result[i] = eig.V(i,n);

#endif
    return result;

  }




  template<class T, unsigned int NDimension>
  T
  Tensor<T,NDimension>
  ::GetDeterminant () const
  {
    T result;
    vnl_matrix< T > M = this->GetVnlMatrix();
    result = vnl_determinant( M );

    return result;
  }





  template<class T, unsigned int NDimension>
  bool
  Tensor<T,NDimension>
  ::IsZero () const
  {
    for(typename BaseArray::ConstIterator i = BaseArray::Begin(); i!=BaseArray::End(); i++)
      if( (*i) != NumericTraits<T>::Zero )
        return false;

    return true;

  }


  template<class T, unsigned int NDimension>
  bool
  Tensor<T,NDimension>
  ::IsPositive () const
  {
    if (!this->IsFinite())
    {
      throw ExceptionObject (__FILE__,__LINE__,"Tensor is not finite");
    }
    bool result = true;
#if defined (TTK_USE_MKL) || defined (TTK_USE_ACML)
    char job = 'V';
    char uplo = 'U';
    double *ev  = new double[NDimension];
    double *work = new double[NDimension*NDimension];
    int ldz = NDimension;
    double *z = new double[NDimension*NDimension];
    int info = 0;
    int order = NDimension;

    // need to copy
    double *buffer = new double[NDegreesOfFreedom];
    for(unsigned int i=0;i<NDegreesOfFreedom;i++)
      buffer[i] = static_cast<double>((*this)[i]);

#ifdef TTK_USE_MKL
    dspev(&job,&uplo,&order,buffer,ev,z,&ldz,work,&info);
#else // ACML
    dspev(job,uplo,order,buffer,ev,z,ldz,&info);
#endif

    if( ev[0] <= NumericTraits<ValueType>::Zero )
      result = false;

    // free the buffers
    delete [] ev;
    delete [] work;
    delete [] z;
    delete [] buffer;

#else
    typedef vnl_symmetric_eigensystem< T >  SymEigenSystemType;
    SymEigenSystemType eig (this->GetVnlMatrix());
    if( eig.D[0] <= NumericTraits<double>::Zero )
      result = false;

#endif

    return result;

  }


  template<class T, unsigned int NDimension>
  bool
  Tensor<T,NDimension>
  ::IsNegative () const
  {
    if (!this->IsFinite())
    {
      throw ExceptionObject (__FILE__,__LINE__,"Tensor is not finite");
    }
    bool result = true;
#if defined (TTK_USE_MKL) || defined (TTK_USE_ACML)
    char job = 'V';
    char uplo = 'U';
    double *ev  = new double[NDimension];
    double *work = new double[NDimension*NDimension];
    int ldz = NDimension;
    double *z = new double[NDimension*NDimension];
    int info = 0;
    int order = NDimension;

    // need to copy
    double *buffer = new double[NDegreesOfFreedom];
    for(unsigned int i=0;i<NDegreesOfFreedom;i++)
      buffer[i] = static_cast<double>((*this)[i]);

#ifdef TTK_USE_MKL
    dspev(&job,&uplo,&order,buffer,ev,z,&ldz,work,&info);
#else // ACML
    dspev(job,uplo,order,buffer,ev,z,ldz,&info);
#endif

    if( ev[0] >= NumericTraits<ValueType>::Zero )
      result = false;

    // free the buffers
    delete [] ev;
    delete [] work;
    delete [] z;
    delete [] buffer;

#else
    typedef vnl_symmetric_eigensystem< T >  SymEigenSystemType;
    SymEigenSystemType eig (this->GetVnlMatrix());
    if( eig.D[0] <= NumericTraits<double>::Zero )
      result = false;

#endif

    return result;

  }

  template<class T, unsigned int NDimension>
  bool
  Tensor<T,NDimension>
  ::IsFinite () const
  {
    return this->GetVnlMatrix().is_finite();
  }


  template<class T, unsigned int NDimension>
  bool
  Tensor<T,NDimension>
  ::HasNans () const
  {
    return this->GetVnlMatrix().has_nans();
  }
  

  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::ValueType
  Tensor<T,NDimension>
  ::ScalarProductWith (const Self& W) const
  {
    // compute trace (This * W):
    // trace(A.B) = Sum_i ( [A.B]_ii ) = Sum_i Sum_k ( a_ik * b_ki  )
    ValueType res = static_cast<ValueType>( 0.0 );
    for(unsigned int i=0; i<NDimension; i++)
    {
      for(unsigned int k=0; k<NDimension; k++)
        res += this->GetComponent(i,k) * W.GetComponent(k,i);
    }

    return res;

  }


  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::ValueType
  Tensor<T,NDimension>
  ::ScalarProductWith (const VectorType& v) const
  {
    return this->ScalarProductWith ( Self (v) );
  }


  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::ValueType
  Tensor<T,NDimension>
  ::GetTrace (void) const
  {
    ValueType res = static_cast<ValueType>(0.0);
    for ( unsigned int i=0;i<NDimension; i++)
      res += this->GetNthComponent ( i*(i+3) / 2 );
    return res;
  }


  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::ValueType
  Tensor<T,NDimension>
  ::GetFA (void) const
  {
    vnl_matrix<ValueType> M = this->GetVnlMatrix();

    ValueType t  = vnl_trace(M);
    ValueType t2 = vnl_trace(M*M);

    ValueType tmp = (t*t)/(3.0*t2);

    if(tmp>1.0)
      tmp = 1.0;

    ValueType fa = static_cast<ValueType>
      ( vcl_sqrt ( 1.5* ( 1.0  - tmp  )) );

    return fa;
  }



  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::ValueType
  Tensor<T,NDimension>
  ::GetGA (void) const
  {

    vnl_matrix<ValueType> L = this->GetVnlMatrix();
    ValueType lt = vnl_trace(L) / static_cast<ValueType>(NDimension);

    Self eye;
    vnl_matrix<ValueType> EYE  = eye.GetVnlMatrix();
    EYE.set_identity();

    vnl_matrix<ValueType> TOT = lt*lt*EYE + L*L - lt*static_cast<ValueType>(2.0)*L;

    ValueType ga = static_cast<ValueType>( vcl_sqrt ( vnl_trace (TOT) ) );

    return ga;
  }



  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::ValueType
  Tensor<T,NDimension>
  ::GetRA (void) const
  {

    vnl_matrix<ValueType> M = this->GetVnlMatrix();

    ValueType t  = vnl_trace(M);
    ValueType t2 = vnl_trace(M*M);

    ValueType ra = static_cast<ValueType>( vcl_sqrt ( t2/t - t/3.0 ) );

    return ra;
  }



  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::ValueType
  Tensor<T,NDimension>
  ::GetVR (void) const
  {

    ValueType n = 1.0;
    ValueType d = 0.0;
    for( unsigned int i=0; i<Dimension; i++)
    {
      ValueType l = this->GetEigenvalue( i );
      n *= l;
      d += l;
    }

    return n/(d*d*d);

  }



  template<class T, unsigned int NDimension>
  Tensor<T,NDimension>
  Tensor<T,NDimension>
  ::DifferentialExp (const Self& G) const
  {
    if (!this->IsFinite())
    {
      throw ExceptionObject (__FILE__,__LINE__,"Tensor is not finite");
    }
    Self Result;
#if defined (TTK_USE_MKL) || defined (TTK_USE_ACML)
    char job = 'V';
    char uplo = 'U';
    double *ev  = new double[NDimension];
    double *work = new double[NDimension*NDimension];
    int ldz = NDimension;
    double *z = new double[NDimension*NDimension];
    int info = 0;
    int order = NDimension;

    // need to copy
    double *buffer = new double[NDegreesOfFreedom];
    for(unsigned int i=0;i<NDegreesOfFreedom;i++)
      buffer[i] = static_cast<double>((*this)[i]);

#ifdef TTK_USE_MKL
    dspev(&job,&uplo,&order,buffer,ev,z,&ldz,work,&info);
#else // ACML
    dspev(job,uplo,order,buffer,ev,z,ldz,&info);
#endif

    vnl_matrix<T> R (z, NDimension, NDimension);
    vnl_matrix<T> TMP1 = R * G.GetVnlMatrix() *R.transpose();

    vnl_matrix<T> TMP2 (NDimension, NDimension);
    for( unsigned int nc=0; nc<NDimension; nc++ )
    {
      for( unsigned int nl=0; nl<NDimension; nl++ )
        TMP2 (nl, nc) = static_cast<T>( sx (ev[nl],ev[nc]) ) * TMP1(nl,nc);
    }

    TMP2 = R.transpose() * TMP2 * R;
    Result.SetVnlMatrix (TMP2);

    // free the buffers
    delete [] ev;
    delete [] work;
    delete [] z;
    delete [] buffer;

#else
    typedef vnl_symmetric_eigensystem< T >  SymEigenSystemType;
    SymEigenSystemType eig (this->GetVnlMatrix());
    vnl_matrix<T> R = eig.V;

    vnl_matrix<T> TMP1 = R.transpose() * G.GetVnlMatrix() * R;
    vnl_matrix<T> TMP2 (NDimension, NDimension);

    for( unsigned int nc=0; nc<NDimension; nc++ )
    {
      for( unsigned int nl=0; nl<NDimension; nl++ )
        TMP2 (nl, nc) = static_cast<T>( sx (eig.D[nl],eig.D[nc]) ) * TMP1(nl,nc);
    }

    TMP2 = R.transpose() * TMP2 * R;
    Result.SetVnlMatrix (TMP2);

#endif

    return Result;


  }



  template<class T, unsigned int NDimension>
  double
  Tensor<T,NDimension>
  ::sx (const double& s1, const double& s2) const
  {
    double s = 0.0;
    double diff = static_cast<double>( s1-s2 );
    double EPS = 0.00001;
    if( fabs ( diff ) < EPS )
      s =  vcl_exp (s1)*(1 + diff/2.0 + diff*diff/6.0  );
    else
      s = ( vcl_exp (s1) - vcl_exp (s2) )/(s1 - s2);

    return s;

  }


  template<class T, unsigned int NDimension>
  void
  Tensor<T,NDimension>
  ::SetNthComponentAsVector ( int c, const ComponentType& v )
  {

    ValueType factor = 1.0/vcl_sqrt (2.0);
    for( unsigned int i=0;i<NDimension;i++)
    {
      if ( c == (int)(i*(i+1)/2 + i) )
      {
        factor = 1.0;
        break;
      }
    }

    this->operator[](c) = v*factor;
  }


  template<class T, unsigned int NDimension>
  typename Tensor<T,NDimension>::ValueType
  Tensor<T,NDimension>
  ::GetNthComponentAsVector ( int c ) const
  {

    ValueType factor = vcl_sqrt (2.0);
    for( unsigned int i=0;i<NDimension;i++)
    {
      if ( c == (int)(i*(i+1)/2 + i) )
      {
        factor = 1.0;
        break;
      }
    }

    return this->operator[](c)*factor;
  }


  template<class T, unsigned int NDimension>
  Tensor<T,NDimension>
  Tensor<T,NDimension>
  ::ApplyMatrix (const MatrixType& R) const
  {
    Self res;
    res.SetVnlMatrix ( this->ApplyMatrix (R.GetVnlMatrix() ) );
    return res;
  }


  template<class T, unsigned int NDimension>
  vnl_matrix<T>
  Tensor<T,NDimension>
  ::ApplyMatrix (const vnl_matrix<T> & R) const
  {
    vnl_matrix<T> res;
    res = ( R*(this->GetVnlMatrix())*R.transpose() );
    return res;
  }


  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::ValueType
  Tensor<T,NDimension>
  ::GetCl (void) const
  {

    ValueType l1 = this->GetEigenvalue( Dimension - 1 );
    ValueType l2 = this->GetEigenvalue( Dimension - 2 );

    return (l1-l2)/this->GetTrace();

  }


  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::ValueType
  Tensor<T,NDimension>
  ::GetCp (void) const
  {
    ValueType l2 = this->GetEigenvalue( Dimension - 2 );
    ValueType l3 = this->GetEigenvalue( Dimension - 3 );

    return 2.0*(l2-l3)/this->GetTrace();
  }


  template<class T, unsigned int NDimension>
  typename Tensor<T, NDimension>::ValueType
  Tensor<T,NDimension>
  ::GetCs (void) const
  {
    ValueType l3 = this->GetEigenvalue( Dimension - 3 );

    return 3.0*l3/this->GetTrace();
  }

} // end of namespace itk



#endif
