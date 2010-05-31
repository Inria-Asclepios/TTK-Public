/***************************************************************************
 *   Copyright (C) 2004 by Christophe Lenglet                              *
 *   clenglet@sophia.inria.fr                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef FIBERSTRACKER_HPP
#define FIBERSTRACKER_HPP

#include <MImg.h>
#include "tract.hpp"
#include "vector3.hpp"
#include "Schemes.h"

namespace AnatConnectivity {

using Utils::Vector3;
using Utils::Vertex3;

/*! Definition of the various fiber tracking algorithms */
enum TrackingMethod {
    EULER,          // 0 Tensor Field Interpolation Tracking with Euler integration scheme
    RK2,            // 1 Tensor Field Interpolation Tracking with 2nd order Runge Kutta integration scheme
    RK4,            // 2 Tensor Field Interpolation Tracking with 4th order Runge Kutta integration scheme
    ADVEC,          // 3 Advection-Diffusion based intgeration
    PROBA,          // 9 Probabilistic ODF tracking
};

/*! Definition of the interpolation methods */
enum InterpMethod {
    EUCLIDEAN,      // Classical Tri-Linear Interpolation in \mathbb{R}^6
};


/*! Class describing a white matter fibers tracker */
template<typename T>
class FibersTracker {

public:
    /*! Constructors - Destructor - Copy - Conversion */
  FibersTracker();
  FibersTracker(const std::string& dti_f, 
		const char* seeds_f, const bool& verbose=false);
  FibersTracker(const char* seeds_f, const bool& verbose=false);
  FibersTracker(const FibersTracker<T>& ft);
  
  ~FibersTracker();
  
  FibersTracker<T>& operator=(const FibersTracker<T>& ft);
  
  void setTract(const Tract<T> &t);
  void setTrackingMethod(const TrackingMethod& tm);
  void setInterpMethod(const InterpMethod& im);
  void setInterpNumberOfIterations(const unsigned int& iter);
  void setValuesType(const std::string& val);
  void setNumberOfFibers(const unsigned int & n);
  
  Tract<T> getTract();
  void SaveScalarConnectivity();
  
  void setData(const std::string& dti_f, const char* seeds_f);
  void setData(const MImg<T> & aniso, const char* seeds_f);
  void setData(const MImg<T> & aniso, const MImg<T> & seeds_f);
  
  void setIntegrationStep(const T& dt);
  
    void FibersOn();
    void FibersOff();

    void verboseOn();
    void verboseOff();

    void setAnisoThreshold(const T& a);
    void setFiberLengthThreshold(const int& l);
    void setAngleThreshold(const T& a);

    void enableAngleConstraint();
    void disableAngleConstraint();

    void setPuncture(const T& p);

    /*! Geodesics computation specific parameters */
    void setOrigin(const char* origin_f);
    void setMask(const std::string& mask_f);

  void clearShortFibers();

protected:
    MImg<T> _seeds, _origin;
    unsigned int _width, _height, _depth;

    /*! Euler-based fiber tracking parameters */
    TrackingMethod _algo;
    InterpMethod _interp;
    unsigned int _interpIters;
    unsigned int _numFibers;
    T _dt;
    std::string _valuesType;

    MImg<T> _anisoImage;
    T _anisoThreshold;
    T _angleThreshold;
    T _angleConstraint;
    T _puncture;

    /*! Geodesic-based tracking parameters */
    MImg<T> _mask;

    std::vector< Vertex3<T> > _originPoints;

    /*! Other generic parameters */
    bool _ComputeFibers;
    T _fiberMinimumLength;
    bool _verbose;

  /*! Resulting tract */
  Tract<T> _tract;

  /*! Protected methods */
  bool _useMask;

  
  bool _inVolume(const Vertex3<T>& pos);
    bool _inVolume(const unsigned int& x, 
		   const unsigned int& y, const unsigned int& z);

};
}
#endif
