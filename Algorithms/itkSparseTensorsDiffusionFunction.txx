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
#ifndef _itk_SparseTensorsDiffusionFunction_txx_
#define _itk_SparseTensorsDiffusionFunction_txx_
#include "itkSparseTensorsDiffusionFunction.h"


namespace itk
{
  
  template<class TImage>
  SparseTensorsDiffusionFunction<TImage>
  ::SparseTensorsDiffusionFunction()
  {
   
    // Set the radius
    RadiusType r;
    r.Fill(1);
    this->SetRadius(r);
   
    m_TimeStep  = 1.0;
    m_Lambda    = 1.0;
    m_Sigma     = 1.0;
    m_Energy    = 0.0;
    m_OldEnergy = 9999.0;

    m_DoNotDiffuseTangents = false;
        
  }
  
  
  template<class TImage>
  void
  SparseTensorsDiffusionFunction<TImage>
  ::SetTensors(const VectorOfTensorsType vec)
  {
    unsigned int n = vec.size();
    // take the log of the tensors once for all
    for( unsigned int i=0; i<n; i++ )
      // take the log of the tensors
      m_Tensors.push_back( vec[i].Log() );
  }

  
  template<class TImage>
  void
  SparseTensorsDiffusionFunction<TImage>
  ::InitializeIteration()
  {
    std::cout << "Global Energy: " << m_Energy << std::endl;
    ScalarType diff = m_Energy - m_OldEnergy;
    std::cout << "Diff: " << diff << std::endl;
    m_OldEnergy = m_Energy;
    m_Energy = 0.0;
    
  }

  
  template<class TImage>
  typename SparseTensorsDiffusionFunction<TImage>::PixelType
  SparseTensorsDiffusionFunction<TImage>
  ::ComputeUpdate(const NeighborhoodType &it, void * gd,
                const FloatOffsetType& itkNotUsed(offset))
  {
    // Get the spacing:
    typename ImageType::ConstPointer image = it.GetImagePointer();        
    
    GlobalDataStruct* data = static_cast<GlobalDataStruct*>(gd);
    
    SpacingType spacing;
    for( unsigned int i=0; i<ImageDimension; i++ )
      spacing[i] = image->GetSpacing()[i];
    
    double coef = 1.0/( pow(2*3.14159265, double(ImageDimension)/2.0 ) *
                        pow( m_Sigma, double(ImageDimension) ) ); // gaussian coeff
    double SIGMA2 = m_Sigma*m_Sigma; 
    
    // current log tensor and position
    PixelType L = it.GetCenterPixel();
    IndexType index = it.GetIndex();

    // will contain the output pixel
    PixelType out = L;

    // we do not want to extrapolate everywhere...
    // so we test for NULL tensors in the input data
    if (!L.IsZero())
    {
      
      // first term: regularization
      PixelType laplacian( static_cast<ScalarType>( 0.0 ) );
      
      
      for(unsigned int i=0; i< ImageDimension; i++)
      {
	
	PixelType Ln = it.GetNext (i);
	PixelType Lmn = it.GetPrevious (i);
	
	bool isNzero = Ln.IsZero();
	bool isMNzero = Lmn.IsZero();
	
	if( !isNzero || !isMNzero )
	{             
	  // neuman conditions
	  if(isNzero && !isMNzero) // 
	    Ln = Lmn;
	  if(isMNzero && !isNzero) // neuman conditions
	    Lmn = Ln;
	}
	
	
	laplacian += ( Ln - L * static_cast<ScalarType>(2.0) + Lmn ) /
	  static_cast<ScalarType>( spacing[i]*spacing[i] );
	
      }
      
      
      // 2nd term: data attach term
      PixelType Attach( static_cast<ScalarType>(0.0) );    
      unsigned int numTensors = m_Tensors.size();
      
      for( unsigned int n=0; n<numTensors; n++)
      {
	PointType ptn = m_Points[n];
	double dist2 = 0.0;
	PointType pt;
	image->TransformIndexToPhysicalPoint(index,pt);
	for( unsigned int m=0; m<ImageDimension ; m++ ) // calculate the euclidian distance
	  dist2 += double( (ptn[m]-pt[m])*(ptn[m]-pt[m]) );
	
	if(dist2 <= 9*SIGMA2) // threshold at 9 sigma square
	{		
	  double G = coef*exp(-0.5*dist2/SIGMA2);
	  PixelType W = m_Tensors[n] - L;
	  
	  if(m_DoNotDiffuseTangents)
	  {
	    PointType tn = m_Tangents[n];
	    // compute tn*tn' : need a tensor of dimensions [NxN], where N is the
	    // image dimension. I will take an OutputPixelType, but this is not
	    // the CORRECT solution...what happen if we take a 6x6 tensor on a 3D
	    // grid? 
	    PixelType X;
	    for( unsigned int nc=0; nc < ImageDimension; nc++ )
	    {
	      for( unsigned int nl=0; nl <= nc; nl++ )
		X.SetComponent( nl, nc, static_cast<ScalarType>( tn[nl]*tn[nc] ) );
	    }
	    
	    ScalarType dotprod = W.ScalarProductWith(X);
	    ScalarType norm2   = X.GetSquaredNorm();
	    
	    if( norm2 != static_cast<ScalarType>( 0.0 ) )
	      W = W - X * dotprod/norm2;
	    
	  }
	  
	  //W = W - L;
	  Attach += W * static_cast<ScalarType>(G);
	}
      }
      
      
      // Sum of the two terms weighted by lambda
      Attach += laplacian * static_cast<ScalarType>(m_Lambda);
      // Attach *= static_cast<ScalarType>(m_TimeStep);
    
      // calculate an energy criterion = trace(newL*newL)
      data->Energy += (L + Attach * static_cast<ScalarType>(m_TimeStep) ).GetSquaredNorm();
        
      out = Attach;
    }
    
    return out;
    
  }
  

} // end of namespace

#endif
