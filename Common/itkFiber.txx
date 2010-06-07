/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL:$
  Language:  C++
  Date:      $Date:$
  Version:   $Revision:$

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_Fiber_txx
#define _itk_Fiber_txx
#include "itkFiber.h"

namespace itk
{

  template <class T, unsigned int TDimension>
  Fiber<T, TDimension>&
  Fiber<T, TDimension>::
  operator=(const Self& f)
  {
    this->SetPointList ( f.GetPointList() );
    return *this;
  }
  
  
  
  template <class T, unsigned int TDimension>
  void
  Fiber<T, TDimension>  
  ::AddPoint (const PointType &P)
  {
    m_PointList.push_back(P);
  }



  template <class T, unsigned int TDimension>
  typename Fiber<T, TDimension>::PointType
  Fiber<T, TDimension>
  ::GetPoint (const int i) const
  {
    if(i<0 || i>(int)(m_PointList.size()-1))
    {
      std::cerr << "Index i is out of range, returning 0" << std::endl;
      PointType pt;
      for( unsigned int j=0; j<TDimension; j++)
        pt[j]= static_cast<ScalarType>( 0.0 );
      return pt;
    }

    return m_PointList[i];
    
  }


  template <class T, unsigned int TDimension>
  void
  Fiber<T, TDimension>
  ::MergeWith (const Self& F)
  {

    // first: check if both fibers are null
    if( this->GetNumberOfPoints()==0 && F.GetNumberOfPoints()==0 )
      return;

    if( F.GetNumberOfPoints()==0 )
      return;

    if( this->GetNumberOfPoints()==0 )
    {
      m_PointList = F.GetPointList();
      return;
    }
    
    // normal cases:
    
    // second check whether the two first points are identical
    PointType firstPoint1 = this->GetPoint (0);
    PointType firstPoint2 = F.GetPoint (0);

    PointListType  P;
    PointListType  P1 = F.GetPointList();
    
    if(firstPoint1 == firstPoint2)
    {
      for(int i=(int)(P1.size()-1);i>0;i--)
        P.push_back (P1[i]);
    }
    else
    {
      for(int i=(int)(P1.size()-1);i>=0;i--)
        P.push_back (P1[i]);
    }

    for(unsigned int i=0;i<m_PointList.size();i++)
      P.push_back (m_PointList[i]);

    m_PointList  = P;
  }


  
  template <class T, unsigned int TDimension>
  double
  Fiber<T, TDimension>
  ::GetLength (void) const
  {

    double length = 0.0;
    if( m_PointList.size() < 2 )
      return length;
    
    for(unsigned int i=0;i<m_PointList.size()-1;i++)
    {
      PointType current = m_PointList[i];
      PointType next = m_PointList[i+1];
      double dist = 0.0;
      
      for(unsigned int j=0;j<TDimension;j++)
        dist += (next[j] - current[j])*(next[j] - current[j]);

      length += sqrtf (dist);
      
    }

    return length;
  }

  

  template <class T, unsigned int TDimension>
  double
  Fiber<T, TDimension>
  ::GetEuclideanLength (void) const
  {
    PointType Start = m_PointList[0];
    PointType End = m_PointList[ m_PointList.size()-1 ];

    double dist = 0.0;

    for(unsigned int i=0;i<TDimension;i++)
      dist += ( End[i] - Start[i] )*( End[i] - Start[i] );

    return sqrt (dist);
    
  }
  
  
  

} // end of namespace


#endif
