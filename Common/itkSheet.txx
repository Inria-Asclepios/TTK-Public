/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: https://scm.gforge.inria.fr/svn/ttk/trunk/Common/itkSheet.txx $
  Language:  C++
  Date:      $Date: 2011-04-04 16:38:45 +0100 (Mon, 04 Apr 2011) $
  Version:   $Revision: 150 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_Sheet_txx
#define _itk_Sheet_txx
#include "itkSheet.h"

namespace itk
{

  template <class T, unsigned int TDimension, class TTensorCoord>
  Sheet<T, TDimension, TTensorCoord>&
  Sheet<T, TDimension, TTensorCoord>::
  operator=(const Self& f)
  {
    this->SetPointList ( f.GetPointList() );
    return *this;
  }
  
  template <class T, unsigned int TDimension, class TTensorCoord>
  void
  Sheet<T, TDimension, TTensorCoord>  
  ::AddPoint (const SheetPoint &P)
  {
    m_SheetPointList.push_back(P);
  }

  template <class T, unsigned int TDimension, class TTensorCoord>
  typename Sheet<T, TDimension, TTensorCoord>::SheetPoint
  Sheet<T, TDimension, TTensorCoord>
  ::GetPoint (const int i) const
  {
    if(i<0 || i>(int)(m_SheetPointList.size()-1))
    {
      std::cerr << "Index i is out of range, returning 0" << std::endl;
      return SheetPoint();
    }

    return m_SheetPointList[i];
    
  }

    template <class T, unsigned int TDimension, class TTensorCoord>
  void
  Sheet<T, TDimension, TTensorCoord>
  ::MergeWith (const Self& F)
  {

    // first: check if both fibers are null
    if( this->GetNumberOfPoints()==0 && F.GetNumberOfPoints()==0 )
      return;

    if( F.GetNumberOfPoints()==0 )
      return;

    if( this->GetNumberOfPoints()==0 )
    {
      m_SheetPointList = F.GetPointList();
      return;
    }
    
    // normal cases:
    
    // second check whether the two first points are identical
    SheetPoint firstPoint1 = this->GetPoint (0);
    SheetPoint firstPoint2 = F.GetPoint (0);

    SheetPointListType  P;
    SheetPointListType  P1 = F.GetPointList();
    
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

    for(unsigned int i=0;i<m_SheetPointList.size();i++)
      P.push_back (m_SheetPointList[i]);

    m_SheetPointList  = P;
  }
  
  
  template <class T, unsigned int TDimension, class TTensorCoord>
  double
  Sheet<T, TDimension, TTensorCoord>
  ::GetSurface (void) const
  {
    double surface = 10.0;
    return surface;
  }
  

  template <class T, unsigned int TDimension, class TTensorCoord>
  void
  Sheet<T, TDimension, TTensorCoord>
  ::GetStatistics (StatisticsType type, double &mean, double &min, double &max, double &var) const
  {
      if (m_SheetPointList.size()==0)
      {
          mean = 0.0;
          min  = 0.0;
          max  = 0.0;
          var  = 0.0;
          return;
      }

      // first point
      PointType current        = m_SheetPointList[0].Point;
      TensorType currentTensor = m_SheetPointList[0].Tensor;

      if (m_SheetPointList.size()==1)
      {
          mean = this->GetTensorScalarValue (currentTensor, type);
          min  = mean;
          max  = mean;
          var  = 0.0;
          return;
      }

      unsigned int pointCount = m_SheetPointList.size();

      double minStat       = 99999999.9;
      double maxStat       = -1.0;
      double totalLength   = 0.0;
      double totalStat     = 0.0;
      double dist          = 0.0;

      PointType next        = m_SheetPointList[1].Point;
      TensorType nextTensor = m_SheetPointList[1].Tensor;

      dist = current.EuclideanDistanceTo (next) * 0.5;

      double stat = this->GetTensorScalarValue (currentTensor, type);

      if (stat<minStat)
          minStat = stat;
      if (stat>maxStat)
          maxStat = stat;

      totalStat += stat * dist;

      totalLength += dist;

      for(unsigned int i=1; i<m_SheetPointList.size()-1; i++)
      {
          current       = next;
          currentTensor = nextTensor;

          next       = m_SheetPointList[i+1].Point;
          nextTensor = m_SheetPointList[i+1].Tensor;

          double oldDist = dist;

          dist = current.EuclideanDistanceTo (next) * 0.5;

          stat = this->GetTensorScalarValue (currentTensor, type);;

          if (stat<minStat)
              minStat = stat;
          if (stat>maxStat)
              maxStat = stat;

          totalStat += stat * (dist + oldDist);

          totalLength += dist + oldDist;
      }

      // case of last point
      current = next;
      currentTensor = nextTensor;

      next       = m_SheetPointList[pointCount-1].Point;
      nextTensor = m_SheetPointList[pointCount-1].Tensor;

      dist = current.EuclideanDistanceTo (next) * 0.5;

      stat = this->GetTensorScalarValue (currentTensor, type);;

      if (stat<minStat)
          minStat = stat;
      if (stat>maxStat)
          maxStat = stat;

      totalStat += stat * dist;

      totalLength += dist;

      if (totalLength>0.0)
          totalStat /= totalLength;

      mean = totalStat;
      min  = minStat;
      max  = maxStat;
      var  = 0.0;
  }

  template <class T, unsigned int TDimension, class TTensorCoord>
  double
  Sheet<T, TDimension, TTensorCoord>
  ::GetTensorScalarValue(const TensorType &tensor, const StatisticsType &type) const
  {
      double scalar = 0.0;
      switch(type)
      {
      case ADC:
          scalar = tensor.GetTrace();
          break;

      case FA:
      default:
          scalar = tensor.GetFA();
          break;
      }

      return scalar;
  }

  template <class T, unsigned int TDimension, class TTensorCoord>
  void
  Sheet<T, TDimension, TTensorCoord>
  ::GetFAStatistics (double &mean, double &min, double &max, double &var) const
  {
      this->GetStatistics(FA, mean, min, max, var);
  }
  
  template <class T, unsigned int TDimension, class TTensorCoord>
  void
  Sheet <T, TDimension, TTensorCoord>
  ::GetADCStatistics(double &mean, double &min, double &max, double &var) const
  {
      this->GetStatistics(ADC, mean, min, max, var);
  }
  

} // end of namespace


#endif
