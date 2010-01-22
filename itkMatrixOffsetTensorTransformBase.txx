#ifndef _itkMatrixOffsetTensorTransformBase_txx
#define _itkMatrixOffsetTensorTransformBase_txx

#include "itkNumericTraits.h"
#include "itkMatrixOffsetTensorTransformBase.h"
#include "vnl/algo/vnl_matrix_inverse.h"


namespace itk
{
  
  // Constructor with default arguments
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::MatrixOffsetTensorTransformBase()
    : Superclass(OutputSpaceDimension, ParametersDimension)
  {
    m_Matrix.SetIdentity();
    m_Rigid.SetIdentity();
    m_MatrixMTime.Modified();
    m_Offset.Fill( 0 );
    m_Center.Fill( 0 );
    m_Translation.Fill( 0 );
    m_Singular = false;
    m_InverseMatrix.SetIdentity();
    m_InverseMatrixMTime = m_MatrixMTime;
    this->m_FixedParameters.SetSize ( NInputDimensions );
    this->m_FixedParameters.Fill ( 0.0 );
  }
  

  // Constructor with default arguments
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::MatrixOffsetTensorTransformBase( unsigned int outputDims, 
                                     unsigned int paramDims   )
    : Superclass(outputDims, paramDims)
  {
    m_Matrix.SetIdentity();
    m_Rigid.SetIdentity();
    m_MatrixMTime.Modified();
    m_Offset.Fill( 0 );
    m_Center.Fill( 0 );
    m_Translation.Fill( 0 );
    m_Singular = false;
    m_InverseMatrix.SetIdentity();
    m_InverseMatrixMTime = m_MatrixMTime;
  }
  
  
  
  
  // Constructor with explicit arguments
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::MatrixOffsetTensorTransformBase(const MatrixType &matrix,
                                    const OutputVectorType &offset)
  {
    m_Matrix = matrix;
    m_MatrixMTime.Modified();
    m_Offset = offset;
    m_Center.Fill( 0 );
    m_Translation.Fill(0);
    for(unsigned int i=0; i<NOutputDimensions; i++)
    {
      m_Translation[i] = offset[i];
    }
    this->ComputeMatrixParameters();
    this->ComputeRigidPartOfTransform();
  }
  



  // Destructor
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::~MatrixOffsetTensorTransformBase()
  {
    return;
  }
  
  
  
  // Print self
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  void
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::PrintSelf(std::ostream &os, Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    
    unsigned int i, j;
    
    os << indent << "Matrix: " << std::endl;
    for (i = 0; i < NInputDimensions; i++) 
    {
      os << indent.GetNextIndent();
      for (j = 0; j < NOutputDimensions; j++)
      {
        os << m_Matrix[i][j] << " ";
      }
      os << std::endl;
    }

    os << indent << "Rigid: " << std::endl;
    for (i = 0; i < NInputDimensions; i++) 
    {
      os << indent.GetNextIndent();
      for (j = 0; j < NOutputDimensions; j++)
      {
        os << m_Rigid[i][j] << " ";
      }
      os << std::endl;
    }
    
    os << indent << "Offset: " << m_Offset << std::endl;
    os << indent << "Center: " << m_Center << std::endl;
    os << indent << "Translation: " << m_Translation << std::endl;
    
    os << indent << "Inverse: " << std::endl;
    for (i = 0; i < NInputDimensions; i++) 
    {
      os << indent.GetNextIndent();
      for (j = 0; j < NOutputDimensions; j++)
      {
        os << this->GetInverseMatrix()[i][j] << " ";
      }
      os << std::endl;
    }
    os << indent << "Singular: " << m_Singular << std::endl;
  }
  


  // Constructor with explicit arguments
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  void
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::SetIdentity( void )
  {
    m_Matrix.SetIdentity();
    m_Rigid.SetIdentity();
    m_MatrixMTime.Modified();
    m_Offset.Fill( 0.0 );
    m_Translation.Fill( 0.0 );
    m_Center.Fill( 0.0 );
    m_Singular = false;
    m_InverseMatrix.SetIdentity();
    m_InverseMatrixMTime = m_MatrixMTime;
    this->Modified();  
  }
  

  
  // Compose with another affine transformation
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  void
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::Compose(const Self * other, bool pre)
  {
    if (pre) 
    {
      m_Offset = m_Matrix * other->m_Offset + m_Offset;
      m_Matrix = m_Matrix * other->m_Matrix;
    }
    else 
    {
      m_Offset = other->m_Matrix * m_Offset + other->m_Offset;
      m_Matrix = other->m_Matrix * m_Matrix;
    }
    
    this->ComputeTranslation();
    this->ComputeMatrixParameters();
    this->ComputeRigidPartOfTransform();
    
    m_MatrixMTime.Modified();
    this->Modified();
    
    return;
  }
  

  
  // Transform a point
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  typename MatrixOffsetTensorTransformBase<TScalarType,
                                           NInputDimensions,
                                           NOutputDimensions>::OutputPointType
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::TransformPoint(const InputPointType &point) const 
  {
    return m_Matrix * point + m_Offset;
  }
  

  
  // Transform a vector
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  typename MatrixOffsetTensorTransformBase<TScalarType,
                                           NInputDimensions,
                                           NOutputDimensions>::OutputVectorType
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::TransformVector(const InputVectorType &vect) const 
  {
    return m_Matrix * vect;
  }
  

  
  // Transform a vnl_vector_fixed
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  typename MatrixOffsetTensorTransformBase<TScalarType,
                                           NInputDimensions,
                                           NOutputDimensions>::OutputVnlVectorType
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::TransformVector(const InputVnlVectorType &vect) const 
  {
    return m_Matrix * vect;
  }
  
  // Transform a vector
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  typename MatrixOffsetTensorTransformBase<TScalarType,
                                           NInputDimensions,
                                           NOutputDimensions>::OutputVectorType
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::InverseTransformVector(const InputVectorType &vect) const 
  {
    return this->GetInverseMatrix() * vect;
  }
  

  
  // Transform a vnl_vector_fixed
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  typename MatrixOffsetTensorTransformBase<TScalarType,
                                           NInputDimensions,
                                           NOutputDimensions>::OutputVnlVectorType
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::InverseTransformVector(const InputVnlVectorType &vect) const 
  {
    return this->GetInverseMatrix() * vect;
  }
  

  
  // Transform a CovariantVector
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  typename MatrixOffsetTensorTransformBase<TScalarType,
                                           NInputDimensions,
                                           NOutputDimensions>::OutputCovariantVectorType
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::TransformCovariantVector(const InputCovariantVectorType &vec) const 
  {
    OutputCovariantVectorType  result;    // Converted vector
    
    for (unsigned int i = 0; i < NOutputDimensions; i++) 
    {
      result[i] = NumericTraits<ScalarType>::Zero;
      for (unsigned int j = 0; j < NInputDimensions; j++) 
      {
        result[i] += this->GetInverseMatrix()[j][i]*vec[j]; // Inverse transposed
      }
    }
    return result;
  }



  // Transform a Tensor
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  typename MatrixOffsetTensorTransformBase<TScalarType,
                                           NInputDimensions,
                                           NOutputDimensions>::OutputTensorType
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::TransformTensor(const InputTensorType &tensor) const 
  {
    // rotate w.r.t the rigid part of the transform
    //std::cout << m_Rigid << std::endl;
    OutputTensorType TENS;
    TENS.SetVnlMatrix ( tensor.ApplyMatrix ( m_Rigid.GetTranspose() ) );
    //std::cout << TENS << std::endl;
    //getchar();
    return TENS;
    
  }

  // Transform a Tensor with Finiste Strain (FS) reorientation strategy
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  typename MatrixOffsetTensorTransformBase<TScalarType,
                                           NInputDimensions,
                                           NOutputDimensions>::OutputTensorType
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::TransformTensorWithFS(const InputTensorType &tensor) const 
  {
    // rotate w.r.t the rigid part of the transform
    //std::cout << m_Rigid << std::endl;
    OutputTensorType TENS;
    TENS.SetVnlMatrix ( tensor.ApplyMatrix ( m_Rigid.GetTranspose() ) );
    //std::cout << TENS << std::endl;
    //getchar();
    return TENS;
    
  }


  // Transform a Tensor with Preservation of Principal Direction (PPD) reorientation strategy
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  typename MatrixOffsetTensorTransformBase<TScalarType,
                                           NInputDimensions,
                                           NOutputDimensions>::OutputTensorType
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::TransformTensorWithPPD(const InputTensorType &tensor) const 
  {
    // rotate preserving the principal direction affine transformation
    //std::cout << m_Matrix << std::endl;
    
    // The PPD only works with 3D tensors!
    if ((NInputDimensions > 3) || (NOutputDimensions > 3) || (NInputDimensions != NOutputDimensions))
      return tensor;

    InputVectorType E1 = tensor.GetEigenvector(2);
    InputVectorType E2 = tensor.GetEigenvector(1);
    InputVectorType E3 = tensor.GetEigenvector(0);

    TScalarType e1 = tensor.GetEigenvalue(2);
    TScalarType e2 = tensor.GetEigenvalue(1);
    TScalarType e3 = tensor.GetEigenvalue(0);

    E1 = this->InverseTransformVector(E1);
    E1 /= E1.GetNorm();

    E2 = this->InverseTransformVector(E2);
    E2 = E2 - E1*(E1*E2);
    E2 /= E2.GetNorm();

    E3 = CrossProduct(E1,E2);

    OutputTensorType TENS;
    TENS = e1*Tensor<TScalarType,NOutputDimensions>(E1) + e2*Tensor<TScalarType,NOutputDimensions>(E2) + e3*Tensor<TScalarType,NOutputDimensions>(E3);
    //std::cout << TENS << std::endl;
    //getchar();
    return TENS;
  }


  // Recompute the inverse matrix (internal)
  template<class TScalarType, unsigned int NInputDimensions,
           unsigned int NOutputDimensions>
  const typename MatrixOffsetTensorTransformBase<TScalarType,
                               NInputDimensions,
                                                 NOutputDimensions>::InverseMatrixType &
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::GetInverseMatrix( void ) const
  {
    // If the transform has been modified we recompute the inverse
    if(m_InverseMatrixMTime != m_MatrixMTime)
    {
      m_Singular = false;
      try 
      {
        m_InverseMatrix  = m_Matrix.GetInverse();
      }
      catch(...) 
      {
        m_Singular = true;
      }
      m_InverseMatrixMTime = m_MatrixMTime;
    }
    
    return m_InverseMatrix;
  }


  
  // return an inverse transformation
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  bool
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::GetInverse( Self * inverse) const
  {
    if(!inverse)
    {
      return false;
    }
    
    this->GetInverseMatrix();
    if(m_Singular)
    {
      return false;
    }
    
    inverse->m_Matrix         = this->GetInverseMatrix();
    inverse->m_InverseMatrix  = m_Matrix;
    inverse->m_Offset         = -(this->GetInverseMatrix() * m_Offset);
    inverse->ComputeTranslation();
    inverse->ComputeMatrixParameters();
    inverse->ComputeRigidPartOfTransform();
    
    return true;
  }
  
  
  // Get fixed parameters
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  void
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::SetFixedParameters( const ParametersType & fp )
  {
    this->m_FixedParameters = fp;
    InputPointType c;
    for ( unsigned int i = 0; i < NInputDimensions; i++ )
    {
      c[i] = this->m_FixedParameters[i];
    }
    this->SetCenter ( c );
  }


  
  /** Get the Fixed Parameters. */
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  const typename MatrixOffsetTensorTransformBase<TScalarType,
                                                 NInputDimensions,
                                                 NOutputDimensions>::ParametersType &
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::GetFixedParameters(void) const
  {
    this->m_FixedParameters.SetSize ( NInputDimensions );
    for ( unsigned int i = 0; i < NInputDimensions; i++ )
    {
      this->m_FixedParameters[i] = this->m_Center[i];
    }
    return this->m_FixedParameters;
  }
  
  

  
  // Get parameters
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  const typename MatrixOffsetTensorTransformBase<TScalarType,
                                                 NInputDimensions,
                                                 NOutputDimensions>::ParametersType &
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::GetParameters( void ) const
  {
    // Transfer the linear part
    unsigned int par = 0;
    
    for(unsigned int row=0; row<NOutputDimensions; row++) 
    {
      for(unsigned int col=0; col<NInputDimensions; col++) 
      {
        this->m_Parameters[par] = m_Matrix[row][col];
        ++par;
      }
    }
    
    // Transfer the constant part
    for(unsigned int i=0; i<NOutputDimensions; i++) 
    {
      this->m_Parameters[par] = m_Translation[i];
      ++par;
    }
    
    return this->m_Parameters;
  }
  


  
  // Set parameters
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  void
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::SetParameters( const ParametersType & parameters )
  {
    unsigned int par = 0;
    
    this->m_Parameters = parameters;
    
    for(unsigned int row=0; row<NOutputDimensions; row++) 
    {
      for(unsigned int col=0; col<NInputDimensions; col++) 
      {
        m_Matrix[row][col] = this->m_Parameters[par];
        ++par;
      }
    }
    
    // Transfer the constant part
    for(unsigned int i=0; i<NOutputDimensions; i++) 
    {
      m_Translation[i] = this->m_Parameters[par];
      ++par;
    }
    
    m_MatrixMTime.Modified(); 
    
    this->ComputeMatrix();  // Not necessary since parameters explicitly define
    //    the matrix
    this->ComputeOffset();

    this->ComputeRigidPartOfTransform();
    
    // Modified is always called since we just have a pointer to the
    // parameters and cannot know if the parameters have changed.
    this->Modified();
    
  }
  


  
  // Compute the Jacobian in one position 
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  const typename MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>::JacobianType & 
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::GetJacobian( const InputPointType & p ) const
  {
    // The Jacobian of the affine transform is composed of
    // subblocks of diagonal matrices, each one of them having
    // a constant value in the diagonal.
    
    this->m_Jacobian.Fill( 0.0 );
    
    unsigned int blockOffset = 0;
    
    for(unsigned int block=0; block < NInputDimensions; block++) 
    {
      for(unsigned int dim=0; dim < NOutputDimensions; dim++ ) 
      {
        this->m_Jacobian( block , blockOffset + dim ) = p[dim];
      }
      
      blockOffset += NInputDimensions;
      
    }
    
    for(unsigned int dim=0; dim < NOutputDimensions; dim++ ) 
    {
      this->m_Jacobian( dim , blockOffset + dim ) = 1.0;
    }
    
    return this->m_Jacobian;
    
  }



  
  // Computes offset based on center, matrix, and translation variables
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  void
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::ComputeOffset( void ) 
  {
    const MatrixType & matrix = this->GetMatrix();
    
    OffsetType offset;
    for(unsigned int i=0; i<NOutputDimensions; i++)
    {
      offset[i] = m_Translation[i] + m_Center[i];
      for(unsigned int j=0; j<NInputDimensions; j++)
      {
        offset[i] -= matrix[i][j] * m_Center[j];
      }
    }
    
    m_Offset = offset ;
  }



  
  // Computes translation based on offset, matrix, and center
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  void
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::ComputeTranslation( void ) 
  {
    const MatrixType & matrix = this->GetMatrix();
    
    OffsetType translation;
    for(unsigned int i=0; i<NOutputDimensions; i++)
    {
      translation[i] = m_Offset[i] - m_Center[i];
      for(unsigned int j=0; j<NInputDimensions; j++)
      {
        translation[i] += matrix[i][j] * m_Center[j];
      }
    }
    
    m_Translation = translation ;
  }
  


  
  // Computes matrix - base class does nothing.  In derived classes is
  //    used to convert, for example, versor into a matrix
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  void
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::ComputeMatrix( void ) 
  {
    // Since parameters explicitly define the matrix in this base class, this
    // function does nothing.  Normally used to compute a matrix when
    // its parameterization (e.g., the class' versor) is modified.
  }
  


  
  // Computes parameters - base class does nothing.  In derived classes is
  //    used to convert, for example, matrix into a versor
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  void
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::ComputeMatrixParameters( void ) 
  {
    // Since parameters explicitly define the matrix in this base class, this
    // function does nothing.  Normally used to update the parameterization
    // of the matrix (e.g., the class' versor) when the matrix is explicitly
    // set.
  }




  
  template<class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  void
  MatrixOffsetTensorTransformBase<TScalarType, NInputDimensions, NOutputDimensions>
  ::ComputeRigidPartOfTransform( void ) 
  {
    //std::cout << m_Matrix << std::endl;

    vnl_matrix_fixed <TScalarType, NInputDimensions, NInputDimensions >
      MMt = m_Matrix.GetVnlMatrix() * m_Matrix.GetTranspose();

    //std::cout << MMt << std::endl;
    
    InputTensorType T;
    T.SetVnlMatrix (MMt);

    T = T.InvSqrt();

    //std::cout << T << std::endl;

    vnl_matrix<TScalarType> res = T.GetVnlMatrix()*m_Matrix.GetVnlMatrix();

    //std::cout << res << std::endl;
    //getchar();
    
    for( unsigned int i=0; i<NInputDimensions; i++)
    {
      for( unsigned int j=0; j<NOutputDimensions; j++)
      {
        m_Rigid (i,j) = res (i,j);
      }
    }    
  }
  
} // namespace

#endif

