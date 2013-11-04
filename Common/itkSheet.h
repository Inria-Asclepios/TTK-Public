/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: https://scm.gforge.inria.fr/svn/ttk/trunk/Common/itkSheet.h $
  Language:  C++
  Date:      $Date: 2011-04-04 16:38:45 +0100 (Mon, 04 Apr 2011) $
  Version:   $Revision: 150 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

  =========================================================================*/
#ifndef _itk_Sheets_h_
#define _itk_Sheets_h_

#include <itkPoint.h>
#include <itkTensor.h>
#include <ostream>

namespace itk
{
  /*! \class Sheet
    \ingroup TensorToolKit
    Convenient way to handle dti fiber data.
  */

  template <  class T , unsigned int NDimension, class TTensorCoord = float >
    class Sheet
    {
    public:
    typedef T                                ScalarType;
    typedef Sheet                            Self;
    typedef Point<ScalarType, NDimension>    PointType;
    typedef Vector<ScalarType, NDimension>   VectorType;
    typedef Tensor<TTensorCoord, NDimension> TensorType;
     
    struct SheetPoint
    {
      PointType  Point;
      VectorType Normal;
      TensorType Tensor;
      SheetPoint(): Tensor(0.0) 
      {
	for(unsigned int i=0; i<NDimension; i++)
	  Point[i] = 0.0;
	for(unsigned int i=0; i<NDimension; i++)
	  Normal[i] = 0.0;
      };
    };
    
    typedef std::vector<SheetPoint> SheetPointListType; 
    
    itkStaticConstMacro (Dimension, unsigned int, NDimension);
    
    /** add a point to the tail of the fiber. */
    void AddPoint ( const SheetPoint & );

    /** set the list of points */
    void SetPointList ( const SheetPointListType &l)
    { m_SheetPointList = l; }
    
    /** get the list of points */
    SheetPointListType GetPointList (void) const
    { return m_SheetPointList; }
    
    /** return the ith point (if it exists)*/
    SheetPoint GetPoint (const int) const;

    /** merge two fibers */
    void MergeWith (const Self& );

    /** return the geodesic length of the fiber*/
    double GetSurface (void) const;

    /** return the number of points */
    unsigned int GetNumberOfPoints (void) const
    { return m_SheetPointList.size(); }

    /** Empties the point list */
    void Clear (void)
    { m_SheetPointList.clear(); }
    
    /** Integrates the FA along the fiber */
    void GetFAStatistics (double &mean, double &min, double &max, double &var) const;

    /** Integrates the ADC along the fiber */
    void GetADCStatistics (double &mean, double &min, double &max, double &var) const;

    enum StatisticsType
    {
      FA,
      ADC
    };

    void GetStatistics (StatisticsType type, double &mean, double &min, double &max, double &var) const;


    Sheet(){};
    ~Sheet(){};
    Sheet (const Self& f)
    {
      m_SheetPointList  = f.GetPointList();
    }
    Self& operator=(const Self& f);

    protected:
    inline double GetTensorScalarValue (const TensorType &tensor, const StatisticsType &type) const;
    
    
    private:
    SheetPointListType  m_SheetPointList;

    };


  template <class T, unsigned int NDimension>
    std::ostream & operator<<(std::ostream &os, const Sheet<T,NDimension> &f)
  {
    for( unsigned int i=0; i<f.GetNumberOfPoints(); i++)
      os << f.GetPointList()[i].Point << " " << f.GetPointList()[i].Normal << " " << f.GetPointList()[i].Tensor << std::endl;

    return os;
  }
  

} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSheet.txx"
#endif

#endif
