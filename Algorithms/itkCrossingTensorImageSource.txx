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
#ifndef _itk_CrossingTensorImageSource_txx_
#define _itk_CrossingTensorImageSource_txx_

#include "itkCrossingTensorImageSource.h"
#include "itkImageRegionIteratorWithIndex.h"

#define PI 3.14159265


namespace itk
{
  
  template <class TTensorImage>
  void
  CrossingTensorImageSource<TTensorImage>
  ::GenerateOutputInformation()
  {

    TensorImageType* output = this->GetOutput (0);

    typename TensorImageType::IndexType index = {{0}};
    typename TensorImageType::SizeType size = {{0}};

    TensorImageType::SizeType::SizeValueType localSize[NDimensions];
    for (int i=0; i<NDimensions; ++i)
        localSize[i]  = m_Size[i];

    size.SetSize( localSize );
    
    RegionType region;
    region.SetIndex(index);
    region.SetSize (size);

    output->SetLargestPossibleRegion (region);
    output->SetSpacing (m_Spacing);
    output->SetOrigin (m_Origin);
  }



  template <class TTensorImage>
  void
  CrossingTensorImageSource<TTensorImage>
  ::ThreadedGenerateData( const RegionType &region, ThreadIdType threadId)
  {

    typedef ImageRegionIteratorWithIndex<TTensorImage> IteratorType;
    IteratorType it (this->GetOutput (0), region);


    // determine center point
    PointType center;
    center[0] = this->GetOutput()->GetOrigin()[0] + 0.5*(this->GetOutput()->GetLargestPossibleRegion().GetSize()[0]-1)*this->GetOutput()->GetSpacing()[0];
    center[1] = this->GetOutput()->GetOrigin()[1] + 0.5*(this->GetOutput()->GetLargestPossibleRegion().GetSize()[1]-1)*this->GetOutput()->GetSpacing()[1];
    center[2] = this->GetOutput()->GetOrigin()[2] + 0.5*(this->GetOutput()->GetLargestPossibleRegion().GetSize()[2]-1)*this->GetOutput()->GetSpacing()[2];
    
    

    while( !it.IsAtEnd() )
    {

      typename TensorImageType::IndexType index = it.GetIndex();
      
      PointType pt;
      this->GetOutput()->TransformIndexToPhysicalPoint (index, pt);



      VectorType v = pt - center;
      TensorType T (0.0);

      double contrib = 0.0;
      
      
      for( unsigned int i=0; i<=m_NumberOfCrossings; i++)
      {
	double alpha = (double)(i) * PI / (double)(m_NumberOfCrossings + 1);
	VectorType u;
	u[0] = cos ( alpha );
	u[1] = sin ( alpha );
	u[2] = 0.0;
	
	VectorType vn = CrossProduct (v, u);
	
	double dist = vn.GetNorm();

	if( dist < this->GetRadius() )
	{
	  VectorType u2;
	  u2[0] = -u[1];
	  u2[1] = u[0];
	  u2[2] = 0.0;

	  VectorType u3 = CrossProduct (u, u2);

	  TensorType TT = 0.8*TensorType (u) + 0.2 * TensorType (u2) + 0.2 * TensorType (u3);

	  T += TT.Log();
	  contrib += 1.0;
	}
	
      }


      if( !T.IsZero() )
      {
	T /= contrib;
	T = T.Exp();
      }


      it.Set (T);
      
      ++it;
    }

  }


  
  template<typename TTensorImage>
  void 
  CrossingTensorImageSource<TTensorImage>
  ::SetSpacing(const SpacingType& spacing )
  {
    unsigned int i; 
    for (i=0; i<TTensorImage::ImageDimension; i++)
    {
      if ( (double)spacing[i] != m_Spacing[i] )
      {
	break;
      }
    } 
    if ( i < TTensorImage::ImageDimension ) 
    { 
      for (i=0; i<TTensorImage::ImageDimension; i++)
      {
	m_Spacing[i] = spacing[i];
      }
      this->Modified();
    }
  }
  
  template<typename TTensorImage>
  void 
  CrossingTensorImageSource<TTensorImage>
  ::SetSpacing(const float* spacing)
  {
    unsigned int i; 
    for (i=0; i<TTensorImage::ImageDimension; i++)
    {
      if ( (double)spacing[i] != m_Spacing[i] )
      {
	break;
      }
    } 
    if ( i < TTensorImage::ImageDimension ) 
    { 
      for (i=0; i<TTensorImage::ImageDimension; i++)
      {
	m_Spacing[i] = spacing[i];
      }
      this->Modified();
    }
  }
  
  template<typename TTensorImage>
  void 
  CrossingTensorImageSource<TTensorImage>
  ::SetSpacing(const double* spacing)
  {
    unsigned int i; 
    for (i=0; i<TTensorImage::ImageDimension; i++)
    {
      if ( spacing[i] != m_Spacing[i] )
      {
	break;
      }
    } 
    if ( i < TTensorImage::ImageDimension ) 
    { 
      for (i=0; i<TTensorImage::ImageDimension; i++)
      {
	m_Spacing[i] = spacing[i];
      }
      this->Modified();
    }
  }
  
  template<typename TTensorImage>
  void 
  CrossingTensorImageSource<TTensorImage>
  ::SetOrigin(const PointType& origin)
  {
    unsigned int i; 
    for (i=0; i<TTensorImage::ImageDimension; i++)
    {
      if ( (double)origin[i] != m_Origin[i] )
      {
	break;
      }
    } 
    if ( i < TTensorImage::ImageDimension ) 
    { 
      for (i=0; i<TTensorImage::ImageDimension; i++)
      {
	m_Origin[i] = origin[i];
      }
      this->Modified();
    }
  }
  
  template<typename TTensorImage>
  void 
  CrossingTensorImageSource<TTensorImage>
  ::SetOrigin(const float* origin)
  {
    unsigned int i; 
    for (i=0; i<TTensorImage::ImageDimension; i++)
    {
      if ( (double)origin[i] != m_Origin[i] )
      {
	break;
      }
    } 
    if ( i < TTensorImage::ImageDimension ) 
    { 
      for (i=0; i<TTensorImage::ImageDimension; i++)
      {
	m_Origin[i] = origin[i];
      }
      this->Modified();
    }
  }
  
  template<typename TTensorImage>
  void 
  CrossingTensorImageSource<TTensorImage>
  ::SetOrigin(const double* origin)
  {
    unsigned int i; 
    for (i=0; i<TTensorImage::ImageDimension; i++)
    {
      if ( origin[i] != m_Origin[i] )
      {
	break;
      }
    } 
    if ( i < TTensorImage::ImageDimension ) 
    { 
      for (i=0; i<TTensorImage::ImageDimension; i++)
      {
	m_Origin[i] = origin[i];
      }
      this->Modified();
    }
  }
  
  template<typename TTensorImage>
  void 
  CrossingTensorImageSource<TTensorImage>
  ::SetSize(const unsigned long* size)
  {
    unsigned int i; 
    for (i=0; i<TTensorImage::ImageDimension; i++)
    {
      if ( size[i] != m_Size[i] )
      {
	break;
      }
    } 
    if ( i < TTensorImage::ImageDimension ) 
    { 
      for (i=0; i<TTensorImage::ImageDimension; i++)
      {
	m_Size[i] = size[i];
      }
      this->Modified();
    }
  }
  
  template<typename TTensorImage>
  void 
  CrossingTensorImageSource<TTensorImage>
  ::SetSize(const SizeType size )
  {
    unsigned int i; 
    for (i=0; i<TTensorImage::ImageDimension; i++)
    {
      if ( size[i] != m_Size[i] )
      {
	break;
      }
    } 
    if ( i < TTensorImage::ImageDimension ) 
    { 
      for (i=0; i<TTensorImage::ImageDimension; i++)
      {
	m_Size[i] = size[i];
      }
      this->Modified();
    }
  }


}


#endif
