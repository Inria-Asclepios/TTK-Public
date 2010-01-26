/*

File:                fibersTrackerHARDI.hpp

Author(s):           Maxime Descoteaux

Created:              7 Sep 2006

Last Revision:       $Date: 2007/02/20 17:59:21 $


I have commented everything concerning probabilistic
tracking...  But it works and could be used.

Copyright (c) 2006 by Maxime Descoteaux, Odyssee Group, INRIA
Sophia Antipolis, France.  Please see the copyright notice
included in this distribution for full details.

*/

#ifndef FIBERSTRACKERHARDI_HPP
#define FIBERSTRACKERHARDI_HPP


/**********************
     Include Files
**********************/
#include "defs.hpp"
#include "tract.hpp"
#include "vector3.hpp"

#include <itkProcessObject.h>
#include <itkImage.h>
#include <itkVectorImage.h>
#include <itkVector.h>
#include <itkImageRegionConstIteratorWithIndex.h>
#include <itkImageRegionIteratorWithIndex.h>

#include <vnl/vnl_matrix.h>

 
namespace AnatConnectivity {
  
  using Utils::Vector3;
  using Utils::Vertex3;
  using Utils::direction;
  using Utils::direction_value;
  using namespace itk;

  /*! Definition of the various fiber tracking algorithms */
  enum TrackingMethod {
    EULER, // 0 Field Interpolation with Euler integration scheme
    RK2,   // 1 Field Interpolation with 2nd order Runge Kutta integration scheme
    RK4,   // 2 Field Interpolation with 4th order Runge Kutta integration scheme
    PROBA, // 3 Probabilistic fiber ODF tracking
  };
  
  /*! Class describing a white matter fibers tracker */
  template<typename T>
  class FibersTrackerHARDI
  {
    
  public:
    //struct direction_value { double x,y,z,v; };

    typedef Image<float, 3>          FloatImageType;
    
    typedef T                        ScalarType;
    typedef itk::Vector<T, 3>        GradientType;
    //typedef DiffusionStudy::GradientType  GradientType;
    //typedef std::vector<DiffusionStudy::GradientType> GradientListType;
    typedef std::vector<GradientType> GradientListType;
    
    typedef itk::VectorImage<ScalarType, 3>    ODFImageType;
    typedef typename ODFImageType::PixelType   ODFType;

    typedef itk::ImageRegionConstIteratorWithIndex<ODFImageType> ODFIteratorType;
    typedef itk::ImageRegionIteratorWithIndex<FloatImageType>    FloatIteratorType;
        

    /*! Constructors - Destructor - Copy - Conversion */
    FibersTrackerHARDI();
    virtual ~FibersTrackerHARDI();

    FibersTrackerHARDI(ODFImageType * hardi, 
		       GradientListType grad,
		       vnl_matrix<T> SHMatrix,
		       FloatImageType::Pointer aniso,
		       FloatImageType::Pointer seeds);

    FibersTrackerHARDI(const FibersTrackerHARDI<T>& ft);		
	  
    FibersTrackerHARDI<T>& operator=(const FibersTrackerHARDI<T>& ft);
    
    void SetData(FloatImageType::Pointer aniso, FloatImageType::Pointer seeds);

    void SetAnisoThreshold(const T& a);
    void SetFiberLengthThreshold(const int& l);
    void SetAngleThreshold(const T& a);

    void EnableAngleConstraint();
    void DisableAngleConstraint();

    void SetTract(const Tract<T> &t);
    void SetTrackingMethod(const TrackingMethod& tm);
    void SetIntegrationStep(const T& dt);
    void SetValuesType(const std::string& val);
    void SetCurrentNumberOfFibers(const unsigned int & n);
    void SetNumberOfSeeds(const unsigned int & n);
  
    Tract<T> GetTract();
    void SaveScalarConnectivity();
  
    void VerboseOn();
    void VerboseOff();

    void CheckOn();
    void CheckOff(); // Check to make sure you start tracking from the
    // strongest ODF peak, in case there are many
    
    void ClearShortFibers();
    void SetColor(const T & color);
    void SetMask(const std::string & mask_f);
    void TrackDeterministic();


    /**
       A hack method to update an ITK object with update progress method.      
     */
    void SetProcessObject (ProcessObject* object)
    { m_ProcessObject = object; }
    ProcessObject* GetProcessObject (void) const
    { return m_ProcessObject; }

    /////////////////////////
    /* for proba tracking */
    /////////////////////////
    //void TrackProba();    
    //     void SetMolecules(const int & molecules);
    //     void SetNumberOfJumps(const int & jumps);
    //     void SetExponent(const int & exp);
    //     void SetStiffness(const int & stiff);
    //     void SetTractogramName(const std::string & f);
    //     void SaveTractogram(const std::string &tractogram);
    //void ClearMaskFibers(const T & value);
    //void ClearFibersNotReaching(const T & value);

  protected:
    ODFImageType * m_hardi;
    FloatImageType::Pointer m_seeds;
    FloatImageType::Pointer m_anisoImage;
    GradientListType m_grad;
    vnl_matrix<T> m_SHMatrix;

    unsigned int _width, _height, _depth;
    
    /*! Euler-based fiber tracking parameters */
    TrackingMethod _algo;
    T _dt;
    std::string _valuesType;
    
    T _anisoThreshold;
    T _angleThreshold;
    bool _angleConstraint;

    ////////////////////////////////////////////
    /*! Proba-based fiber tracking parameters */
    ////////////////////////////////////////////
    //     MImg<T> *_ntimes_touched;   // number of times voxel crossed
    //     MImg<T> *_trans_proba;      // volume of transition probability 
    //     CImg<T> _angles;
    //     int _molecules;   /* number of molecules in the PROBA tracking */
    //     int _njumps;   /* number of jumps per molecule */
    //     T _exponent;
    //     T _stiffness;
    //     std::string _tractogram_f;
    //     T *_get_proba(Vertex3<T> &pos, std::vector< Vertex3<T> > &neighbors, 
    // 		  const int &old_anglei, bool *wayout);
    
    //     int _mkrand(const T *proba, const int & size);
    //     T _get_angle(const int & anglei, const int & i);
    //void _proba_track();

    /*! Other generic parameters */
    T _fiberMinimumLength;
    bool _verbose;
    bool _color;
    bool _nocheck;   // check magnitude of ODF maxima to make
                     // sure we start from the max

    unsigned int _nSeeds;
    unsigned int _nFibers;
    T _setcolor;

    /*! Resulting tract */
    Tract<T> _tract;

    void _euler_track();
    void _eulerPropagate(ODFIteratorType itODF,
			  const int& direction, Fiber<T>& fiber,
			  const T &angle,
			  const std::vector<int> Nbors[]);

    std::vector< direction_value > _computeMaxima(
					     vnl_matrix<T> odf,
					     T max_thresh,
					     const T & angle,
					     const std::vector<int> Nbors[]);
    vnl_matrix<T> _interpolate(const T& ffx, 
			       const T& ffy, 
			       const T& ffz);

    bool _eulerIntegrate(Vertex3<T>& pos, Vector3<T>& speed, 
			 vnl_matrix<T> & s, const int& direction,
			 const T &angle,
			 const std::vector<int> Nbors[]);
		
    
    bool _inVolume(const Vertex3<T>& pos);

    T _get_min(const T &a, const T &b, const T &c);


    ProcessObject::Pointer m_ProcessObject;
    
  };
}
	
#endif  /* FIBERSTRACKERHARDI.H_HPP */
