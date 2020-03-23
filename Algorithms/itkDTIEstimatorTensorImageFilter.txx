#pragma once

#include "itkDTIEstimatorTensorImageFilter.h"

#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIterator.h>
#include <cstdio>

namespace itk
{

  template<class TInputImage, class TOutputImage>
  DTIEstimatorTensorImageFilter<TInputImage,TOutputImage>
  ::DTIEstimatorTensorImageFilter()
  {
    m_BST = 100.0;
  }
  

  template<class TInputImage, class TOutputImage>
  void
  DTIEstimatorTensorImageFilter<TInputImage,TOutputImage>
  ::BeforeThreadedGenerateData()
  {
  
    if( m_GradientList.size() != this->GetNumberOfIndexedInputs() ) {
      char message[512];
      sprintf (message, "Error: The lengths of the gradient list (%u) and number of inputs (%u) does not match.",(int)(m_GradientList.size()), (int)(this->GetNumberOfIndexedInputs()));
      throw itk::ExceptionObject(__FILE__,__LINE__, message);
    }

    // creation of the gradient matrix
    // the internal gradient list contains only non-zero gradients
    m_InternalGradientList.clear();
    unsigned int n = m_GradientList.size();
    int nonZeroGradientCount = 0;
    for (unsigned int i=0; i<n; i++)
    {
      GradientType g = m_GradientList[i];
      if (g.GetNorm()>0.001)
      {
	m_InternalGradientList.push_back (g);
        nonZeroGradientCount++;
      }
    }

    if( nonZeroGradientCount<6 )
    {
      throw ExceptionObject (__FILE__,__LINE__,"At least 6 non-zero gradients are required");
    }
    
    n = m_InternalGradientList.size();
    
    m_G   = InternalMatrixType(6, n, 0.0);
    m_IG2 = InternalMatrixType(6, 6, 0.0);
    
    for( unsigned int j=0; j<n; j++ )
    {
      GradientType g = m_InternalGradientList[j];
      g.Normalize();
      VectorType Gn = this->Matrix2Vec( this->TensorProduct(g) );
      for(int i=0; i<6; i++)
	  m_G(i,j) = Gn[i];
    }
    
    // invert m_G*m_G
    InternalMatrixType m_G2 = m_G*m_G.transpose();
    InverseType invG(m_G2);
    m_IG2 = invG.inverse();
    
  }


  template<class TInputImage, class TOutputImage>
  void
  DTIEstimatorTensorImageFilter<TInputImage,TOutputImage>
  ::DynamicThreadedGenerateData(const OutputImageRegionType &outputRegionForThread)
  {  
    typedef ImageRegionIterator<OutputImageType>      IteratorOutputType;
    typedef ImageRegionConstIterator<InputImageType>  IteratorInputType;

    unsigned long numPixels = outputRegionForThread.GetNumberOfPixels();
    unsigned long step = numPixels/1000;
    unsigned long progress = 0;
    
    int n = (int)(this->GetNumberOfIndexedInputs());
    double a = 1.0/sqrt(2.0);
    
    IteratorOutputType itOut(this->GetOutput(), outputRegionForThread);

    // create a list of iterators for each input
    std::vector<IteratorInputType> ListOfInputIterators;
    for(int i=0; i<n; i++)
    {
	IteratorInputType it(this->GetInput(i),outputRegionForThread);
	ListOfInputIterators.push_back(it);
    }

    int nonZeroGradientCount = (int)(m_InternalGradientList.size());
    
    while(!itOut.IsAtEnd())
    {

      if (this->GetAbortGenerateData())
        throw itk::ProcessAborted(__FILE__,__LINE__);

      OutputPixelType out( static_cast<ScalarType>( 0.0 ) );

      double b0 = 0.0;
      int   nB0 = 0;

      // average all b0
      for (int i=0; i<n; i++)
      {
        if (m_GradientList[i].GetNorm()<=0.001 )
        {
          b0 += ListOfInputIterators[i].Get();
          nB0++;
        }
      }
      if (nB0>0)
        b0 /= static_cast<double> (nB0);
 
      // threshold b0
      if (b0>m_BST)
      {	
        InternalMatrixType B(nonZeroGradientCount, 1);
	int gradCount = 0;

	// any occurence of a null gradient will be considered as a B0
	// and averaged to have an average B0
        for(int i=0; i<n; i++)
        {
          if (m_GradientList[i].GetNorm()>0.001 )
	  {	    
	    double bi = static_cast<double> (ListOfInputIterators[i].Get());
	    if( bi<0.001 )
              bi = 0.001;
	    
	    //B(i-1,0) = log( b0  / bi );
	    B(gradCount++, 0) = bi;
	  }
        }

	for (int i=0; i<nonZeroGradientCount; i++)
	{
	  // it happens that diffusion is greater than the B0: contribution of such gradient is canceled
	  if (b0>B (i,0))
	    B (i,0) = log ( b0 / B (i,0) );
          else
            B (i,0) = 0.0;
	}


        // if a B(i,0) is null, then we replace it by the closest non-null gradient
        for (int i=0; i<nonZeroGradientCount; i++)
        {
          if (B(i,0)==0.0)
          {
            GradientType grad = m_InternalGradientList[i];
            int closestIndex = -1;
            double dist = -1.0;
            for (int j=0; j<nonZeroGradientCount; j++)
            {
              if (B(j,0)>0.0)
              {
                double dotProduct = grad * m_InternalGradientList[j];
                if (dotProduct>0.98 && dotProduct>dist)
                {
                  closestIndex = j;
                  dist = dotProduct;
                }
              }
            }

            if (closestIndex != -1)
              B(i,0) = B(closestIndex, 0);
          }
        }
	
        InternalMatrixType D = m_IG2*m_G*B;
	
        out.SetNthComponent( 0, static_cast<ScalarType>( D(0,0) ));
        out.SetNthComponent( 1, static_cast<ScalarType>( a*D(1,0) ));
        out.SetNthComponent( 2, static_cast<ScalarType>( D(2,0) ));
        out.SetNthComponent( 3, static_cast<ScalarType>( a*D(3,0) ));
        out.SetNthComponent( 4, static_cast<ScalarType>( a*D(4,0) ));
        out.SetNthComponent( 5, static_cast<ScalarType>( D(5,0)) );
      }
            
      itOut.Set(out);
      ++itOut;
      ++progress;
      for(int i=0;i<n;i++)
        ++(ListOfInputIterators[i]);
    }
  }
  
  
  template<class TInputImage, class TOutputImage>
  typename DTIEstimatorTensorImageFilter<TInputImage,TOutputImage>::InternalMatrixType
  DTIEstimatorTensorImageFilter<TInputImage,TOutputImage>
  ::Vec2Matrix(const VectorType &vec) const
  {    
    InternalMatrixType mat(3,3,0.0);
    double a = 1.0/sqrt(2.0);
    mat(0,0)=vec[0];
    mat(0,1)=a*vec[1];
    mat(1,1)=vec[2];
    mat(0,2)=a*vec[3];
    mat(1,2)=a*vec[4];
    mat(2,2)=vec[5];
    mat(1,0)=mat(0,1);
    mat(2,0)=mat(0,2);
    mat(2,1)=mat(1,2);
    return mat;
  }
  
  template<class TInputImage, class TOutputImage>
  typename DTIEstimatorTensorImageFilter<TInputImage,TOutputImage>::VectorType
  DTIEstimatorTensorImageFilter<TInputImage,TOutputImage>
  ::Matrix2Vec(const InternalMatrixType &mat) const
  {
    VectorType vec;
    double a = sqrt(2.0);
    vec[0] = mat(0,0);
    vec[1] = a*mat(0,1);
    vec[2] = mat(1,1);
    vec[3] = a*mat(0,2);
    vec[4] = a*mat(1,2);
    vec[5] = mat(2,2);
    return vec;
  }

  template<class TInputImage, class TOutputImage>
  typename DTIEstimatorTensorImageFilter<TInputImage,TOutputImage>::InternalMatrixType
  DTIEstimatorTensorImageFilter<TInputImage,TOutputImage>
  ::TensorProduct(const GradientType &g) const
  {
    InternalMatrixType mat (3, 3, 0.0);
    for(int nl=0; nl<3; nl++)
    {
      for(int nc=0; nc<3; nc++)
	mat(nl,nc) = g[nl]*g[nc];
    }
    return mat;
  }


} // end of namespace
