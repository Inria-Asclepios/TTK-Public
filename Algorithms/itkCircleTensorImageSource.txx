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
#ifndef _itk_CircleTensorImageSource_txx_
#define _itk_CircleTensorImageSource_txx_

#include "itkCircleTensorImageSource.h"
#include "itkImageRegionIteratorWithIndex.h"


#define PI 3.14159265


namespace itk
{
  
  template <class TTensorImage>
  void
  CircleTensorImageSource<TTensorImage>
  ::GenerateOutputInformation()
  {

    TensorImageType* output = this->GetOutput (0);

    typename TensorImageType::IndexType index = {{0}};
    typename TensorImageType::SizeType  size;
    typename TensorImageType::SpacingType spacing;
    typename TensorImageType::PointType origin;
    for(unsigned int i=0; i<TTensorImage::GetImageDimension(); i++)
    {
      size[i] = 16;
      spacing[i] = 1.0;
      origin[i] = 0.0;
    }

    size[TTensorImage::GetImageDimension()-1] = 4;
    
    RegionType region;
    region.SetIndex(index);
    region.SetSize (size);

    output->SetLargestPossibleRegion (region);
    output->SetSpacing (spacing);
    output->SetOrigin (origin);
  }



  template <class TTensorImage>
  void
  CircleTensorImageSource<TTensorImage>
  ::ThreadedGenerateData( const RegionType &region, ThreadIdType threadId)
  {

    typedef ImageRegionIteratorWithIndex<TTensorImage> IteratorType;
    IteratorType it (this->GetOutput (0), region);


    // determine center point
    PointType center;
    //center[0] = this->GetOutput()->GetOrigin()[0] + 0.5*(this->GetOutput()->GetLargestPossibleRegion().GetSize()[0]-1)*this->GetOutput()->GetSpacing()[0];
    center[0] = this->GetOutput()->GetOrigin()[0];
    center[1] = this->GetOutput()->GetOrigin()[1] + 0.5*(this->GetOutput()->GetLargestPossibleRegion().GetSize()[1]-1)*this->GetOutput()->GetSpacing()[1];
    center[2] = this->GetOutput()->GetOrigin()[2] + 0.5*(this->GetOutput()->GetLargestPossibleRegion().GetSize()[2]-1)*this->GetOutput()->GetSpacing()[2];
    
    

    while( !it.IsAtEnd() )
    {

      typename TensorImageType::IndexType index = it.GetIndex();
      
      PointType pt;
      this->GetOutput()->TransformIndexToPhysicalPoint (index, pt);



      VectorType v = pt - center;
      TensorType T (0.0);



      if( v.GetNorm()>=m_Radius1 && v.GetNorm()<=m_Radius2 )
      {

	v.Normalize();
	VectorType u2;
	u2[0] = -v[1];
	u2[1] = v[0];
	u2[2] = 0.0;

	VectorType u3 = CrossProduct (u2, v);
	
	T = 0.8*TensorType (u2) + 0.2 * TensorType (v) + 0.2 * TensorType (u3);
      }
      

      it.Set (T);
      
      ++it;
    }

  }
  

}


#endif
