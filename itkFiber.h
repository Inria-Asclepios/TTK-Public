#ifndef _itk_Fibers_h_
#define _itk_Fibers_h_


namespace itk
{
  /*! \class Fiber
    \ingroup TensorToolKit
    Convenient way to handle dti fiber data.
   */

  template <  class T , unsigned int NDimension >
    class ITK_EXPORT Fiber
  {

  public:

    typedef T                              ScalarType;
    typedef Fiber                          Self;
    typedef Point<ScalarType, NDimension>  PointType;
    typedef Vector<ScalarType, NDimension> VectorType;    
    typedef std::vector<PointType>         PointListType;
    typedef std::vector<VectorType>        VectorListType;


    itkStaticConstMacro (Dimension, unsigned int, NDimension);
    
    
    /** add a point to the tail of the fiber. */
    void AddPoint ( const PointType & );

    /** set the list of points */
    void SetPointList ( const PointListType l)
    { m_PointList = l; }
    
    /** get the list of points */
    PointListType GetPointList (void) const
    { return m_PointList; }
    
    /** return the ith point (if it exists)*/
    PointType GetPoint (const int) const;

    /** merge two fibers */
    void MergeWith (const Self& );

    /** return the geodesic length of the fiber*/
    double GetLength (void) const;

    /** Get the Euclidean length*/
    double GetEuclideanLength (void) const;

    /** return the number of points */
    unsigned int GetNumberOfPoints (void) const
    { return m_PointList.size(); }

    /** Empties the point list */
    void Clear (void)
    { m_PointList.clear(); }
    
    

    Fiber(){};
    ~Fiber(){};
    Fiber (const Self& f)
    {
      m_PointList  = f.GetPointList();
    }
    Self& operator=(const Self& f);

    
    
  private:

    PointListType  m_PointList;
    VectorListType m_VectorList;
    
    

  };


  template <class T, unsigned int NDimension>
    std::ostream & operator<<(std::ostream &os, const Fiber<T,NDimension> &f)
  {
    for( unsigned int i=0; i<f.GetNumberOfPoints(); i++)
      os << f.GetPointList()[i] << " " << std::endl;

    return os;
  }
  

} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFiber.txx"
#endif

#endif
