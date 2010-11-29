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
#ifndef _itk_Fiber_txx
#define _itk_Fiber_txx
#include "itkFiber.h"

namespace itk
{

  template <class T, unsigned int TDimension, class TTensorCoord>
  Fiber<T, TDimension, TTensorCoord>&
  Fiber<T, TDimension, TTensorCoord>::
  operator=(const Self& f)
  {
    this->SetPointList ( f.GetPointList() );
    return *this;
  }
  
  
  
  template <class T, unsigned int TDimension, class TTensorCoord>
  void
  Fiber<T, TDimension, TTensorCoord>  
  ::AddPoint (const FiberPoint &P)
  {
    m_FiberPointList.push_back(P);
  }



  template <class T, unsigned int TDimension, class TTensorCoord>
  typename Fiber<T, TDimension, TTensorCoord>::FiberPoint
  Fiber<T, TDimension, TTensorCoord>
  ::GetPoint (const int i) const
  {
    if(i<0 || i>(int)(m_FiberPointList.size()-1))
    {
      std::cerr << "Index i is out of range, returning 0" << std::endl;
      return FiberPoint();
    }

    return m_FiberPointList[i];
    
  }


  template <class T, unsigned int TDimension, class TTensorCoord>
  void
  Fiber<T, TDimension, TTensorCoord>
  ::MergeWith (const Self& F)
  {

    // first: check if both fibers are null
    if( this->GetNumberOfPoints()==0 && F.GetNumberOfPoints()==0 )
      return;

    if( F.GetNumberOfPoints()==0 )
      return;

    if( this->GetNumberOfPoints()==0 )
    {
      m_FiberPointList = F.GetPointList();
      return;
    }
    
    // normal cases:
    
    // second check whether the two first points are identical
    FiberPoint firstPoint1 = this->GetPoint (0);
    FiberPoint firstPoint2 = F.GetPoint (0);

    FiberPointListType  P;
    FiberPointListType  P1 = F.GetPointList();
    
    if(firstPoint1.Point == firstPoint2.Point)
    {
      for(int i=(int)(P1.size()-1);i>0;i--)
        P.push_back (P1[i]);
    }
    else
    {
      for(int i=(int)(P1.size()-1);i>=0;i--)
        P.push_back (P1[i]);
    }

    for(unsigned int i=0;i<m_FiberPointList.size();i++)
      P.push_back (m_FiberPointList[i]);

    m_FiberPointList  = P;
  }


  
  template <class T, unsigned int TDimension, class TTensorCoord>
  double
  Fiber<T, TDimension, TTensorCoord>
  ::GetLength (void) const
  {

    double length = 0.0;
    if( m_FiberPointList.size() < 2 )
      return length;
    
    for(unsigned int i=0;i<m_FiberPointList.size()-1;i++)
    {
      PointType current = m_FiberPointList[i].Point;
      PointType next = m_FiberPointList[i+1].Point;
      double dist = 0.0;
      
      for(unsigned int j=0;j<TDimension;j++)
        dist += (next[j] - current[j])*(next[j] - current[j]);

      length += sqrtf (dist);
      
    }

    return length;
  }

  

  template <class T, unsigned int TDimension, class TTensorCoord>
  double
  Fiber<T, TDimension, TTensorCoord>
  ::GetEuclideanLength (void) const
  {
    PointType Start = m_FiberPointList[0];
    PointType End = m_FiberPointList[ m_FiberPointList.size()-1 ];

    double dist = 0.0;

    for(unsigned int i=0;i<TDimension;i++)
      dist += ( End[i] - Start[i] )*( End[i] - Start[i] );

    return sqrt (dist);
    
  }
  
  
  

} // end of namespace


#endif
