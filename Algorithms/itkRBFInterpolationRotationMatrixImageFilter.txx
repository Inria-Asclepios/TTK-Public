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
#ifndef _itk_RBFInterpolationRotationMatrixImageFilter_txx_
#define _itk_RBFInterpolationRotationMatrixImageFilter_txx_

#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIteratorWithIndex.h>

#ifdef TTK_USE_MKL
#include <mkl_lapack.h>
#else
#include <vnl/algo/vnl_matrix_inverse.h>
#endif

namespace itk
{

#ifndef M_PI
#define M_PI       3.1415926535897932385E0
#endif
  const double TH_TINY    = 1e-5;
  const double PI_TH_TINY = M_PI - TH_TINY;
  const double TH_SMALL   = 1e-1;
    
  template< class TInputImage, class TOutputImage>
  void
  RBFInterpolationRotationMatrixImageFilter<TInputImage, TOutputImage>
  ::BeforeThreadedGenerateData()
  {

    if( ( m_RotationMatrices.size()==0 ) || ( m_RotationMatrices.size()!=m_Points.size() ) )
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Rotation Matrices (or Points) not (or badly) set.");
    
    // compute the RotationMatrices mean:
    m_Mean = this->ComputeMeanOfRotationMatrices( m_RotationMatrices );
    //VectorType vMean = this->RotationMatrixToVector (m_Mean );

    // Solve the system
    unsigned int numMatrices = m_RotationMatrices.size();

	unsigned int N = numMatrices+1;

#ifdef TTK_USE_MKL
    ScalarType *H;
    ScalarType *T;    
        
    H = new ScalarType[ N*N ];
    T = new ScalarType[ N * 3 ];
#else
    GeneralMatrixType H(N, N, 0.0);
    GeneralMatrixType T(N, 3, 0.0);
#endif
    

    for( unsigned int j=0;j<numMatrices;j++)
    {
#ifdef TTK_USE_MKL
      H[j] = 1.0;
#else
      H(j,0) = 1.0;
#endif
    }

#ifdef TTK_USE_MKL
    H[numMatrices]=0.0;
#else
    H(numMatrices,0) = 0.0;
#endif

    
    for(unsigned int j=1; j<N; j++)
    {
      PointType xj = m_Points[j-1];
      
      for(unsigned int i=0; i<numMatrices; i++)
      {
        PointType xi = m_Points[i];
#ifdef TTK_USE_MKL
        H[ j*N + i ] = this->h(xi,xj, m_Sigma, m_Gamma);
#else
        H(j,i) = this->h(xj,xi, m_Sigma, m_Gamma);
#endif
      }

#ifdef TTK_USE_MKL
      H[ j*N + numMatrices ] = 1.0;
#else
      H(j,numMatrices) = 0.0;
#endif
      
    }

#ifdef TTK_USE_MKL

    /*
    // display H:
    std::cout << "H=[" << std::endl;
    for( unsigned int i=0;i<N;i++)
    {
      for (unsigned int j=0;j<N;j++)
        std::cout << H[j*(N)+i] << " ";
      std::cout << std::endl;
    }
    std::cout << "];" << std::endl;
    getchar();
    */
    
#endif 

    
    for( unsigned int i=0; i<3; i++)
    {
      for( unsigned int j=0; j<numMatrices; j++)
      {
        VectorType W = this->RotationMatrixToVector ( m_RotationMatrices[j] ); // - vMean;
#ifdef TTK_USE_MKL
        T[ i*(N) + j ] = W[i];
#else
        T(i,j) = W[i];
#endif
      }
      
#ifdef TTK_USE_MKL
      T[ i*N + numMatrices ] = 0.0;
#else
      T(i,numMatrices) = 0.0;
#endif
      
    }
    
    m_L = GeneralMatrixType ( N, 3, 0.0 );
    
#ifdef TTK_USE_MKL
    // 1. LU factorization of H :
    int m = N;
    int n = N;
    int *ipiv = new int[N];
    int info = -1;
    int ldh = N;
    
    dgetrf(&m, &n, H, &m, ipiv, &info);
    if( info != 0)
    {
      std::cerr << info << std::endl;
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Bad LU factorization.");
    }
    

    // 2. solve the system Ax = B :
    char trans = 'N';
    int nrhs = 3;
    info = -1;
    int ldt = N;
        

    dgetrs(&trans, &n, &nrhs, H, &ldh, ipiv, T, &ldt, &info);
    if( info != 0)
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Bad system solution.");

    
    for (unsigned int i=0; i<N; i++)
    {
      for( unsigned int j=0; j<3; j++)
        m_L (i,j) = T[j*(N) + i];
    }

    // check the m_L
    for( unsigned int i=0; i<3; i++ )
    {
      double sum=0.0;
      for( unsigned int j=1; j<(N); j++)
        sum += m_L (j,i);
      std::cout << "Lambda sum is: " << sum << std::endl;
    }

    /*
    std::cout << "L: " << m_L << std::endl;
    getchar();
    */
    
    // delete the shit
    delete [] H;
    delete [] T;
    delete [] ipiv;
        
#else
    
    vnl_matrix_inverse<ScalarType> IH(H);
    m_L = ( IH.inverse() ) * T;
    
#endif

  }




  template< class TInputImage, class TOutputImage>
  void
  RBFInterpolationRotationMatrixImageFilter<TInputImage, TOutputImage>
  ::DynamicThreadedGenerateData(const OutputImageRegionType &outputRegionForThread)
  {
  
    typedef ImageRegionIterator<OutputImageType>               OutputImageIteratorType;
    typedef ImageRegionConstIteratorWithIndex<InputImageType>  InputImageIteratorType;

    InputImageIteratorType itIn(this->GetInput(),outputRegionForThread);
    OutputImageIteratorType itOut(this->GetOutput(),outputRegionForThread);

    
    unsigned int numMatrices = m_RotationMatrices.size();
    unsigned int N = numMatrices+1;
    
    while(!itOut.IsAtEnd())
      {

	OutputPixelType out;
        for( int i=0; i<3; i++)
          for( int j=0; j<3; j++)
            out (i,j) = 0.0;

        // threshold the input image
	if( itIn.Value() > static_cast<InputPixelType>( 0.0 ) )
	  {
	   
	    IndexType index = itIn.GetIndex();
	    PointType pt;
	    this->GetInput()->TransformIndexToPhysicalPoint(index,pt);	    
	    
	    GeneralMatrixType H(1,N);
            H (0,0)=1.0;
	    for(unsigned int i=1; i<N; i++)
            {
              PointType pti = m_Points[i-1];
              H(0,i) = this->h(pti,pt,m_Sigma,m_Gamma);
            }

            // compute the interpolated value
	    GeneralMatrixType res = H*m_L;
            VectorType V;
            V[0] = res (0,0);
            V[1] = res (0,1);
            V[2] = res (0,2);
            
	    //out = (m_Mean + M);
            out = this->VectorToRotationMatrix (V);
	  }

	itOut.Set(out);
	
	++itIn;
	++itOut;
      }
    
  
  }
  


  
  template< class TInputImage, class TOutputImage>
  typename RBFInterpolationRotationMatrixImageFilter<TInputImage,TOutputImage>::ScalarType
  RBFInterpolationRotationMatrixImageFilter<TInputImage, TOutputImage>
  ::h(const PointType& x1, const PointType& x2, const ScalarType& sigma, const ScalarType& gamma)
  {
    double dist2 = x1.SquaredEuclideanDistanceTo (x2);
    double dist = sqrt (dist2);
    //return static_cast<ScalarType>( 1.0/(1.0 + pow(dist2/(sigma*sigma),gamma)) );

    ScalarType res;// = static_cast<ScalarType>(dist2);
    res = static_cast<ScalarType>(dist2 * log (dist) );

    /*
    if( ( (sigma/2.0) - (double)(int)(sigma/2.0) ) ==0.0 )
    {
      if( dist2 != 0.0 )
        res = static_cast<ScalarType>( -1.0*pow (dist2,sigma)*log (dist2) );
      else
        res = static_cast<ScalarType>(0.0);
    }
    else
      res = static_cast<ScalarType>( pow ((double)(-1.0), (double)ceil(sigma/2.0))*pow (dist2, (double)sigma) );
    */
    return res;
    
  }




  template <class TInputImage, class TOutputImage >
  typename RBFInterpolationRotationMatrixImageFilter<TInputImage,TOutputImage>::OutputPixelType
  RBFInterpolationRotationMatrixImageFilter<TInputImage,TOutputImage>::
  ComputeMeanOfRotationMatrices(const VectorOfPixelType& VEC)
  {

    VectorType V (0.0);

    double N = (double)(VEC.size());
    for( unsigned int i=0; i<VEC.size(); i++ )
      V += Self::RotationMatrixToVector (VEC[i]);

    V /= N;
    return Self::VectorToRotationMatrix (V);
  }
  




  // code based on Xavier Pennec's code (Basic/Rotation3D.C)
  template <class TInputImage, class TOutputImage >
  typename RBFInterpolationRotationMatrixImageFilter<TInputImage,TOutputImage>::VectorType
  RBFInterpolationRotationMatrixImageFilter<TInputImage,TOutputImage>::
  RotationMatrixToVector (const OutputPixelType& R)
  {
    VectorType V;

    VectorType sin_r;
    double d, sq, fact;
    double c = 0.5 * (R(0,0) + R(1,1) + R(2,2) - 1.0); // cos(theta)
    if ((d = ::fabs(c)) > 1.0)
      c /= d;
    double theta = acos(c);

    if ( ::fabs(theta) <= TH_TINY)
    {
      // R close to identity : DL around theta = 0
      fact = 0.5 + theta*theta/12.0;
      V[0] = ( R(2,1) - R(1,2) ) * fact;
      V[1] = ( R(0,2) - R(2,0) ) * fact;
      V[2] = ( R(1,0) - R(0,1) ) * fact;
      //return V;
    }
    else
      if ( ::fabs(theta - M_PI) <= TH_SMALL)
      {
        // R is close to a symetry : DL around theta = M_PI
        // set values with diag terms of n.nt
        for (int i = 0; i < 3; i++)
        {
          // approx order 0
          // c = (rot[i][i] - 1.0)*0.5;
          sq = 1.0+ (R(i,i) - 1.0)/(1-c);
          // 0 <=  [ sq = (n[i])^2 ] <= 1
          if ( sq < 0.0) sq = 0.;
          if (sq > 1.) sq = 1.;
          V[i] = ::sqrt(sq);
        }
        
      V *= theta/V.GetNorm();
      // set signs with off-diag terms of n.nt
      if ( (R(0,1) + R(1,0) ) < 0.0)
        V[1] = -1.0*V[1];
      if ( (R(0,2) + R(2,0) ) < 0.0)
        V[2] = -1.0*V[2];

      // determine wether r = +/- theta*n
      sin_r[0] = ( R(2,1) - R(1,2) );
      sin_r[1] = ( R(0,2) - R(2,0) );
      sin_r[2] = ( R(1,0) - R(0,1) );
      // determine the most significant term
      int k = 0;
      if ( ::fabs(sin_r[1]) > ::fabs(sin_r[k])) k = 1;
      if ( ::fabs(sin_r[2]) > ::fabs(sin_r[k])) k = 2;
      // choose the sign
      if ( sin_r[k] * V[k] < 0.0 )
        // They do not have the same signs : r = -v
        V *= -1;
      //return V;
      }
      else
      { // At last no numerical problem
        fact = 0.5 * theta / sin(theta);
        V[0] = ( R(2,1) - R(1,2) ) * fact;
        V[1] = ( R(0,2) - R(2,0) ) * fact;
        V[2] = ( R(1,0) - R(0,1) ) * fact;
        //return V;
      }

    return V;

  }



  // code based on Xavier Pennec's code (Basic/Rotation3D.C)
  template <class TInputImage, class TOutputImage >
  typename RBFInterpolationRotationMatrixImageFilter<TInputImage,TOutputImage>::OutputPixelType
  RBFInterpolationRotationMatrixImageFilter<TInputImage,TOutputImage>::
  VectorToRotationMatrix (const VectorType& V)
  {
    OutputPixelType R;
    
    double c, s, k1, k2;
    
    double theta2 = V.GetSquaredNorm();
    double theta = ::sqrt( theta2 );
    if (theta > TH_TINY)
    {
      c = cos(theta);
      s = sin(theta);
      k1 = s / theta;
      k2 = (1 - c) / theta2;
    }
    else {  // Taylor expension around theta = 0
      k2 = 1.0/2.0 - theta2/24.0;
      c = 1.0 - theta2*k2;
      k1 = 1.0 - theta2/6;
    }
    
    /* I + M*Mt */
    for (int i = 0; i < 3; i++)
      for (int j = 0; j <= i; j++)
      {
        R(i,j) = k2 * V[i] * V[j];
        if (i != j)
          R(j,i) = R(i,j);
        else
          R(i,i) = R(i,i) + c;
      }
    
    double aux = k1 * V[2];
    R(0,1) = R(0,1) - aux;
    R(1,0) = R(1,0) + aux;
    aux = k1 * V[1];
    R(0,2) = R(0,2) + aux;
    R(2,0) = R(2,0) - aux;
    aux = k1 * V[0];
    R(1,2) = R(1,2) - aux;
    R(2,1) = R(2,1) + aux;

    return R;
  }













  template< class TInputImage, class TOutputImage>
  typename RBFInterpolationRotationMatrixImageFilter<TInputImage,TOutputImage>::VectorOfPixelType
  RBFInterpolationRotationMatrixImageFilter<TInputImage,TOutputImage>::
  InterpolateValuesAt(const VectorOfPixelType& matrices, const VectorOfPointsType& points, const VectorOfPointsType& positions, const ScalarType& sigma, const ScalarType& gamma)
  {
    
    if( ( matrices.size()==0 ) || ( matrices.size()!=points.size() ) )
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Rotation Matrices (or Points) not (or badly) set.");
    
    // compute the RotationMatrices mean:
    OutputPixelType mean = Self::ComputeMeanOfRotationMatrices( matrices );
    VectorType vMean = Self::RotationMatrixToVector (mean );

    // Solve the system
    unsigned int numMatrices = matrices.size();
	unsigned int N = numMatrices+1;
	
#ifdef TTK_USE_MKL
    ScalarType *H;
    ScalarType *T;    
        
    H = new ScalarType[ N*N ];
    T = new ScalarType[ N * 3 ];
#else
    GeneralMatrixType H(N, N, 0.0);
    GeneralMatrixType T(N, 3, 0.0);
#endif
    

    for( unsigned int j=0;j<numMatrices;j++)
    {
#ifdef TTK_USE_MKL
      H[j] = 1.0;
#else
      H(j,0) = 1.0;
#endif
    }

#ifdef TTK_USE_MKL
    H[numMatrices]=0.0;
#else
    H(numMatrices,0) = 0.0;
#endif

    
    for(unsigned int j=1; j<N; j++)
    {
      PointType xj = points[j-1];
      
      for(unsigned int i=0; i<numMatrices; i++)
      {
        PointType xi = points[i];
#ifdef TTK_USE_MKL
        H[ j*N + i ] = Self::h(xi,xj, sigma, gamma);
#else
        H(j,i) = Self::h(xj,xi, sigma, gamma);
#endif
      }

#ifdef TTK_USE_MKL
      H[ j*N + numMatrices ] = 1.0;
#else
      H(j,numMatrices) = 0.0;
#endif
      
    }

#ifdef TTK_USE_MKL

    /*
    // display H:
    std::cout << "H=[" << std::endl;
    for( unsigned int i=0;i<N;i++)
    {
      for (unsigned int j=0;j<N;j++)
        std::cout << H[j*(N)+i] << " ";
      std::cout << std::endl;
    }
    std::cout << "];" << std::endl;
    getchar();
    */
    
#endif 

    
    for( unsigned int i=0; i<3; i++)
    {
      for( unsigned int j=0; j<numMatrices; j++)
      {
        VectorType W = Self::RotationMatrixToVector ( matrices[j] ); // - vMean;
#ifdef TTK_USE_MKL
        T[ i*(N) + j ] = W[i];
#else
        T(i,j) = W[i];
#endif
      }
      
#ifdef TTK_USE_MKL
      T[ i*N + numMatrices ] = 0.0;
#else
      T(i,numMatrices) = 0.0;
#endif
      
    }
    
   GeneralMatrixType L = GeneralMatrixType ( N, 3, 0.0 );
    
#ifdef TTK_USE_MKL
    // 1. LU factorization of H :
    int m = N;
    int n = N;
    int *ipiv = new int[N];
    int info = -1;
    int ldh = N;
    
    dgetrf(&m, &n, H, &m, ipiv, &info);
    if( info != 0)
    {
      std::cerr << info << std::endl;
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Bad LU factorization.");
    }
    

    // 2. solve the system Ax = B :
    char trans = 'N';
    int nrhs = 3;
    info = -1;
    int ldt = N;
        

    dgetrs(&trans, &n, &nrhs, H, &ldh, ipiv, T, &ldt, &info);
    if( info != 0)
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Bad system solution.");

    
    for (unsigned int i=0; i<N; i++)
    {
      for( unsigned int j=0; j<3; j++)
        L (i,j) = T[j*(N) + i];
    }

    // check the L
    for( unsigned int i=0; i<3; i++ )
    {
      double sum=0.0;
      for( unsigned int j=1; j<(N); j++)
        sum += L (j,i);
      std::cout << "Lambda sum is: " << sum << std::endl;
    }
           
    
    // delete the shit
    delete [] H;
    delete [] T;
    delete [] ipiv;
        
#else
    
    vnl_matrix_inverse<ScalarType> IH(H);
    L = ( IH.inverse() ) * T;
    
#endif


    VectorOfPixelType output;

    // now interpolate
    for( unsigned int j=0; j<positions.size(); j++)
    {
      GeneralMatrixType H2(1,N);
      H2 (0,0)=1.0;
      PointType pt = positions[j];
      
      for(unsigned int i=1; i<N; i++)
      {
        PointType pti = points[i-1];
        H2(0,i) = Self::h(pti,pt,sigma,gamma);
      }

      // compute the interpolated value
      GeneralMatrixType res = H2*L;
      VectorType V;
      V[0] = res (0,0);
      V[1] = res (0,1);
      V[2] = res (0,2);
      
      output.push_back ( Self::VectorToRotationMatrix (V) );
    }

    return output;
        
    
  }
  


  
} // end of namespace


#endif
