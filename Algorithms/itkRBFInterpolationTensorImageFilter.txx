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
#ifndef _itk_RBFInterpolationTensorImageFilter_txx_
#define _itk_RBFInterpolationTensorImageFilter_txx_
#include "itkRBFInterpolationTensorImageFilter.h"

#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIteratorWithIndex.h>

#ifdef TTK_USE_MKL
#include <mkl_lapack.h>
#else
#include <vnl/algo/vnl_matrix_inverse.h>
#endif

namespace itk
{


  template< class TInputImage, class TOutputImage>
  void
  RBFInterpolationTensorImageFilter<TInputImage, TOutputImage>
  ::BeforeThreadedGenerateData()
  {

    if( ( m_Tensors.size()==0 ) || ( m_Tensors.size()!=m_Points.size() ) )
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Tensors (or Points) not (or badly) set.");
    
    // compute the tensors mean:
    m_Mean = this->ComputeMeanOfTensors( m_Tensors );

    // Solve the system
    unsigned int numTensors = m_Tensors.size();

	unsigned int N = numTensors+1;
	
#ifdef TTK_USE_MKL
    ScalarType *H;
    ScalarType *T;    
        
    H = new ScalarType[ N*N ];
    T = new ScalarType[ N * OutputPixelType::NDegreesOfFreedom ];
#else
    GeneralMatrixType H(N, N, 0.0);
    GeneralMatrixType T(N, OutputPixelType::NDegreesOfFreedom, 0.0);
#endif
    

    for( unsigned int j=0;j<numTensors;j++)
    {
#ifdef TTK_USE_MKL
      H[j] = 1.0;
#else
      H(j,0) = 1.0;
#endif
    }

#ifdef TTK_USE_MKL
    H[numTensors]=0.0;
#else
    H(numTensors,0) = 0.0;
#endif

    
    for(unsigned int j=1; j<N; j++)
    {
      PointType xj = m_Points[j-1];
      
      for(unsigned int i=0; i<numTensors; i++)
      {
        PointType xi = m_Points[i];
#ifdef TTK_USE_MKL
        H[ j*N + i ] = this->h(xi,xj, m_Sigma, m_Gamma);
#else
        H(j,i) = this->h(xj,xi, m_Sigma, m_Gamma);
#endif
      }

#ifdef TTK_USE_MKL
      H[ j*N + numTensors ] = 1.0;
#else
      H(j,numTensors) = 0.0;
#endif
      
    }

#ifdef TTK_USE_MKL

    
    // display H:
    std::cout << "H=[" << std::endl;
    for( unsigned int i=0;i<N;i++)
    {
      for (unsigned int j=0;j<N;j++)
        std::cout << H[j*(N)+i] << " ";
      std::cout << std::endl;
    }
    std::cout << "];" << std::endl;
    
    
#endif 

    
    for( unsigned int i=0; i<OutputPixelType::NDegreesOfFreedom; i++)
    {
      for( unsigned int j=0; j<numTensors; j++)
      {
        //OutputPixelType W = m_Tensors[j] - m_Mean;
        OutputPixelType W = m_Tensors[j];
#ifdef TTK_USE_MKL
        T[ i*(N) + j ] = W.GetNthComponent(i);
#else
        T(i,j) = W.GetNthComponent(i);
#endif
      }
      
#ifdef TTK_USE_MKL
      T[ i*N + numTensors ] = 0.0;
#else
      T(i,numTensors) = 0.0;
#endif
      
    }
    
    m_L = GeneralMatrixType ( N, OutputPixelType::NDegreesOfFreedom, 0.0 );
    
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
    int nrhs = OutputPixelType::NDegreesOfFreedom;
    info = -1;
    int ldt = N;
        

    dgetrs(&trans, &n, &nrhs, H, &ldh, ipiv, T, &ldt, &info);
    if( info != 0)
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Bad system solution.");

    
    for (unsigned int i=0; i<N; i++)
    {
      for( unsigned int j=0; j<OutputPixelType::NDegreesOfFreedom; j++)
        m_L (i,j) = T[j*(N) + i];
    }

    // check the m_L
    for( unsigned int i=0; i<OutputPixelType::NDegreesOfFreedom; i++ )
    {
      double sum=0.0;
      for( unsigned int j=1; j<(N); j++)
        sum += m_L (j,i);
      std::cout << "Lambda sum is: " << sum << std::endl;
    }
           
    
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
  RBFInterpolationTensorImageFilter<TInputImage, TOutputImage>
  ::ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, int threadId)
  {
  
    typedef ImageRegionIterator<OutputImageType>               OutputImageIteratorType;
    typedef ImageRegionConstIteratorWithIndex<InputImageType>  InputImageIteratorType;

    InputImageIteratorType itIn(this->GetInput(),outputRegionForThread);
    OutputImageIteratorType itOut(this->GetOutput(),outputRegionForThread);

    
    unsigned int numTensors = m_Tensors.size();
    unsigned int N = numTensors+1;
    
    while(!itOut.IsAtEnd())
      {

	OutputPixelType out = static_cast<ScalarType>( 0.0 );

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
            OutputPixelType M;

            M = res.data_block();

	    //out = (m_Mean + M);
            out = M;
	  }

	itOut.Set(out);
	
	++itIn;
	++itOut;
      }
    
  
  }


  template< class TInputImage, class TOutputImage>
  typename RBFInterpolationTensorImageFilter<TInputImage, TOutputImage>::OutputPixelType
  RBFInterpolationTensorImageFilter<TInputImage, TOutputImage>
  ::ComputeMeanOfTensors(const VectorOfPixelType& vec)
  {

    unsigned int numTensors = vec.size();
    OutputPixelType mean (static_cast<ScalarType>( 0.0 ) );
    
    for(unsigned int n=0; n<numTensors; n++)
    {

      //OutputPixelType W = m_Tensors[n].Log();
      OutputPixelType W = vec[n];

      /*
      if(m_DoNotDiffuseTangent)
      {        
        
        PointType tn = m_Tangents[n];
        OutputPixelType X( static_cast<ScalarType>(0.0) );
        for( unsigned int nc=0; nc < ImageDimension; nc++ )
        {
          for( unsigned int nl=0; nl <= nc; nl++ )
            X.SetComponent( nl, nc, static_cast<ScalarType>( tn[nl]*tn[nc] ) );
        }
        
        ScalarType dotprod = X.ScalarProductWith(W);
        ScalarType norm2   = X.GetSquaredNorm();
        
        if( norm2 != static_cast<ScalarType>( 0.0 ) )
          W = W - X * dotprod/norm2;
      }
      */
      
      mean += W;
    }
    
    if( numTensors != 0 )
      mean /= static_cast<ScalarType>( numTensors );

    return mean;
    
  }


  template< class TInputImage, class TOutputImage>
  typename RBFInterpolationTensorImageFilter<TInputImage,TOutputImage>::ScalarType
  RBFInterpolationTensorImageFilter<TInputImage, TOutputImage>
  ::h(const PointType& x1, const PointType& x2, const ScalarType& sigma, const ScalarType& gamma)
  {
    /*
      double dist2 = 0.0;
      for(unsigned int i=0; i<ImageDimension; i++)
      {
      dist2 += double( ( x1[i]-x2[i] )*( x1[i]-x2[i] ) );
      }
    */
    //dist2 = sqrt (dist2);
    
    double dist2 = x1.SquaredEuclideanDistanceTo (x2);
    
    //return static_cast<ScalarType>( 1.0/(1.0 + pow(dist2/(sigma*sigma),gamma)) );
    
    ScalarType res;// = static_cast<ScalarType>(dist2);
    
    if( dist2 == 0.0 )
    {
      res = static_cast<ScalarType>(0.0);
    }
    else
    {  
      res = static_cast<ScalarType>( dist2 * log (sqrt (dist2)) );
    }

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
  

  template< class TInputImage, class TOutputImage>
  typename RBFInterpolationTensorImageFilter<TInputImage,TOutputImage>::VectorOfPixelType
  RBFInterpolationTensorImageFilter<TInputImage,TOutputImage>::
  InterpolateValuesAt(const VectorOfPixelType& tensors, const VectorOfPointsType& points, const VectorOfPointsType& positions, const ScalarType& sigma, const ScalarType& gamma)
  {
    if( tensors.size()==0 || tensors.size()!=points.size() )
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error in RBFInterpolationTensorImageFilter::InterpolateValuesAt(): Sizes do not match.");

   
    // compute the tensors mean:
    OutputPixelType mean = Self::ComputeMeanOfTensors( tensors );

    // Solve the system
    unsigned int numTensors = tensors.size();

	unsigned int N = numTensors+1;
	
#ifdef TTK_USE_MKL
    ScalarType *H;
    ScalarType *T;    
        
    H = new ScalarType[ N*N ];
    T = new ScalarType[ N * OutputPixelType::NDegreesOfFreedom ];
#else
    GeneralMatrixType H(N, N, 0.0);
    GeneralMatrixType T(N, OutputPixelType::NDegreesOfFreedom, 0.0);
#endif
    

    for( unsigned int j=0;j<numTensors;j++)
    {
#ifdef TTK_USE_MKL
      H[j] = 1.0;
#else
      H(j,0) = 1.0;
#endif
    }

#ifdef TTK_USE_MKL
    H[numTensors]=0.0;
#else
    H(numTensors,0) = 0.0;
#endif

    
    for(unsigned int j=1; j<N; j++)
    {
      PointType xj = points[j-1];
      
      for(unsigned int i=0; i<numTensors; i++)
      {
        PointType xi = points[i];
#ifdef TTK_USE_MKL
        H[ j*N + i ] = Self::h(xi,xj, sigma, gamma);
#else
        H(j,i) = Self::h(xj,xi, sigma, gamma);
#endif
      }

#ifdef TTK_USE_MKL
      H[ j*N + numTensors ] = 1.0;
#else
      H(j,numTensors) = 0.0;
#endif
      
    }

#ifdef TTK_USE_MKL

    // display H:
    /*    std::cout << "H=[" << std::endl;
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

    
    for( unsigned int i=0; i<OutputPixelType::NDegreesOfFreedom; i++)
    {
      for( unsigned int j=0; j<numTensors; j++)
      {
        //OutputPixelType W = tensors[j] - mean;
        OutputPixelType W = tensors[j];
#ifdef TTK_USE_MKL
        T[ i*(N) + j ] = W.GetNthComponent(i);
#else
        T(i,j) = W.GetNthComponent(i);
#endif
      }
      
#ifdef TTK_USE_MKL
      T[ i*N + numTensors ] = 0.0;
#else
      T(i,numTensors) = 0.0;
#endif
      
    }
    
    GeneralMatrixType L = GeneralMatrixType ( N, OutputPixelType::NDegreesOfFreedom, 0.0 );
    
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
    int nrhs = OutputPixelType::NDegreesOfFreedom;
    info = -1;
    int ldt = N;
        

    dgetrs(&trans, &n, &nrhs, H, &ldh, ipiv, T, &ldt, &info);
    if( info != 0)
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Bad system solution.");

    
    for (unsigned int i=0; i<N; i++)
    {
      for( unsigned int j=0; j<OutputPixelType::NDegreesOfFreedom; j++)
        L (i,j) = T[j*(N) + i];
    }

    // check the L
    for( unsigned int i=0; i<OutputPixelType::NDegreesOfFreedom; i++ )
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
      OutputPixelType M;
      
      M = res.data_block();
      //M += mean;
      output.push_back (M);
    }

    return output;
        
    
  }
  
  
  
  
} // end of namespace itk


#endif
