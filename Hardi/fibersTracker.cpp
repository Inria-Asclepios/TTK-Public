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

#include "fibersTracker.hpp"

/////////////////////////////////////// NOTE ///////////////////////////////
// * management of seeds and origin and different coord sys. OK
// * all Euler based methods validated. OK
// * back-propagation OK
// * In general it is better to manually provide the origin since, SPM origin
//   being given in unsigned int and not in float, rounding may greatly influence the
//   position and thus the accuracy of the back-propagation.
//   This is also to take into account for the seeds position
// * the mask must have the same resolution than the DT image
// * SEEDS and ORIGIN can be given in any coordinate system
/////////////////////////////////////// TODO ///////////////////////////////
// * Christoffel symbols computation and use to perform fiber tracking

#ifdef WIN32
#define rint(x) floor((x)+0.5)
#pragma warning(disable:4661)
#endif

namespace AnatConnectivity {

  /*! Constructors - Destructor - Copy - Conversion */
  template<typename T>
  FibersTracker<T>::FibersTracker() {
    _width = _height = _depth = 0;

    _algo = EULER;
    _interp = EUCLIDEAN;
    _interpIters = 10;
    _numFibers = 0;
    _dt = 0.1;
    _valuesType = "FA";
    _anisoThreshold = 0.15;
    _angleThreshold = 60.0; // in degrees
    _angleConstraint = false;
    _puncture = 0.20;

    _ComputeFibers = false;
    _fiberMinimumLength = 1; // minimum euclidean length
    _verbose = false;
  }

  template<typename T>
  FibersTracker<T>::FibersTracker(const std::string& dti_f, const char* seeds_f,
				  const bool& verbose) {
    // Handle seed(s)
    std::ifstream seedPoints(seeds_f,ios::binary);
    //int voxel[3];

    _seeds = seeds_f;

    _algo = EULER;
    _interp = EUCLIDEAN;
    _interpIters = 10;
    _numFibers = 0;
    _dt = 0.1;
    _valuesType = "FA";

    _anisoImage = NULL;
    _anisoThreshold = 0.15;
    _angleThreshold = 60.0; // in degrees
    _angleConstraint = false;
    _puncture = 0.20;

    _ComputeFibers = false;
    _fiberMinimumLength = 1; // minimum euclidean length
    _verbose = verbose;
  }

  template<typename T>
  FibersTracker<T>::FibersTracker(const char* seeds_f, 
				  const bool& verbose) {
    // Handle seed(s)
    std::ifstream seedPoints(seeds_f,ios::binary);
    int voxel[3];

    if (!seedPoints.is_open()) {
      // Init image
      _seeds = MImg<T>(_width,_height,_depth);
      //_seeds.SetTransform(t);
      _seeds.fill(0);

      // Get voxel position
      sscanf(seeds_f,"%d,%d,%d",voxel,voxel+1,voxel+2);
      //cout << voxel[0] << "," << voxel[1] << "," << voxel[2] << "\n";
      _seeds(voxel[0],voxel[1],voxel[2]) = 1.0;
		
    } else
      _seeds = seeds_f;

    _algo = EULER;
    _interp = EUCLIDEAN;
    _interpIters = 10;
    _numFibers = 0;
    _dt = 0.1;

    _anisoThreshold = 0.15;
    _angleThreshold = 60.0; // in degrees
    _angleConstraint = false;
    _puncture = 0.20;

    _ComputeFibers = false;
    _fiberMinimumLength = 1; // minimum euclidean length
    _verbose = verbose;

    cout << "constructor FiberTracker out...\n";
  }


  template<typename T>
  FibersTracker<T>::FibersTracker(const FibersTracker<T>& ft) {
    _seeds = ft._seeds;
    _origin = ft._origin;
    _width = ft._width;
    _height = ft._height;
    _depth = ft._depth;

    _algo = ft._algo;
    _interp = ft._interp;
    _numFibers = ft._numFibers;
    _interpIters = ft._interpIters;
    _dt = ft._dt;
    _valuesType = ft._valuesType;

    _anisoImage = ft._anisoImage;
    _anisoThreshold = ft._anisoThreshold;
    _angleThreshold = ft._angleThreshold;
    _angleConstraint = ft._angleConstraint;
    _puncture = ft._puncture;

    _mask = ft._mask;
    _originPoints = ft._originPoints;

    _ComputeFibers = ft._ComputeFibers;
    _fiberMinimumLength = ft._fiberMinimumLength;
    _verbose = ft._verbose;

    _tract = ft._tract;
  }

  template<typename T>
  FibersTracker<T>::~FibersTracker() {}


  template<typename T>
  FibersTracker<T>& FibersTracker<T>::operator=(const FibersTracker<T>& ft) {
    _seeds = ft._seeds;
    _origin = ft._origin;
    _width = ft._width;
    _height = ft._height;
    _depth = ft._depth;

    _algo = ft._algo;
    _interp = ft._interp;
    _interpIters = ft._interpIters;
    _numFibers = ft._numFibers;
    _dt = ft._dt;
    _valuesType = ft._valuesType;
    _anisoImage = ft._anisoImage;

    _anisoThreshold = ft._anisoThreshold;
    _angleThreshold = ft._angleThreshold;
    _angleConstraint = ft._angleConstraint;
    _puncture = ft._puncture;

    _originPoints = ft._originPoints;

    _ComputeFibers = ft._ComputeFibers;
    _fiberMinimumLength = ft._fiberMinimumLength;
    _verbose = ft._verbose;

    _tract = ft._tract;
    return *this;
  }


  template<typename T>
  void FibersTracker<T>::setNumberOfFibers(const unsigned int & n) {
    _numFibers = n;
  }
	


  template<typename T>
  void FibersTracker<T>::setData(const MImg<T> & aniso, const char* seeds_f) {

    _anisoImage = aniso;
    _width = aniso.width;
    _height = aniso.height;
    _depth = aniso.depth;
	
    // Handle seed(s)
    std::ifstream seedPoints(seeds_f,ios::binary);
    int voxel[3];

    if (!seedPoints.is_open()) {
      // Init image
      _seeds = MImg<T>(_width,_height,_depth);
      _seeds.SetTransform(aniso.GetTransform());
      _seeds.fill(0);

      // Get voxel position
      sscanf(seeds_f,"%d,%d,%d",voxel,voxel+1,voxel+2);
      //cout << voxel[0] << "," << voxel[1] << "," << voxel[2] << "\n";
      _seeds(voxel[0],voxel[1],voxel[2]) = 1.0;
	
    }
    else
      _seeds = seeds_f;		
  }

   template<typename T>
  void FibersTracker<T>::setData(const MImg<T> & aniso, const MImg<T> & seeds) {

    _anisoImage = aniso;
    _width = aniso.width;
    _height = aniso.height;
    _depth = aniso.depth;

    _seeds = seeds;
   
  }

  template<typename T>
  void FibersTracker<T>::setTract(const Tract<T> & t) {
    _tract = t;  
  }


  template<typename T>
  void FibersTracker<T>::setTrackingMethod(const TrackingMethod& tm) {
    _algo = tm;
  }

  template<typename T>
  void FibersTracker<T>::setInterpMethod(const InterpMethod& im) {
    _interp = im;
  }

  template<typename T>
  void FibersTracker<T>::setInterpNumberOfIterations(const unsigned int& iter) {
    _interpIters = iter;
  }

  template<typename T>
  Tract<T> FibersTracker<T>::getTract() {
    if (!_ComputeFibers)
      std::cout << "Tract does not hold any fibers information since only connectivity waas computed !" << std::endl;
    return _tract;
  }



  template<typename T>
  void FibersTracker<T>::setIntegrationStep(const T& dt) {
    _dt = dt;
  }

  template<typename T>
  void FibersTracker<T>::setValuesType(const std::string& val) {
    _valuesType = val;
  }

  template<typename T>
  void FibersTracker<T>::FibersOn() {
    _ComputeFibers = true;
  }

  template<typename T>
  void FibersTracker<T>::FibersOff() {
    _ComputeFibers = false;
  }

  template<typename T>
  void FibersTracker<T>::verboseOn() {
    _verbose = true;
  }

  template<typename T>
  void FibersTracker<T>::verboseOff() {
    _verbose = false;
  }

  template<typename T>
  void FibersTracker<T>::setAnisoThreshold(const T& a) {
    _anisoThreshold = a;
  }

  template<typename T>
  void FibersTracker<T>::setFiberLengthThreshold(const int& l) {
    _fiberMinimumLength = l;
  }

  template<typename T>
  void FibersTracker<T>::setAngleThreshold(const T& a) {
    _angleThreshold = a;
  }

  template<typename T>
  void FibersTracker<T>::enableAngleConstraint() {
    _angleConstraint = true;
  }

  template<typename T>
  void FibersTracker<T>::disableAngleConstraint() {
    _angleConstraint = false;
  }

  template<typename T>
  void FibersTracker<T>::setPuncture(const T& p) {
    _puncture = p;
  }

  template<typename T>
  void FibersTracker<T>::clearShortFibers() {
    if (_verbose)
      std::cout << "Clearing fibers with short Euclidean length" << std::endl;
    std::cout.flush();
    unsigned int clearedFibers = 0;
    unsigned int fi, nfibers = _tract.getNumFibers();

    for (fi=0; fi<nfibers; fi++) {
      if (_tract[fi].euclideanLength() < _fiberMinimumLength) {
	_tract[fi].clear();
	clearedFibers++;
      }
    }

    if (_verbose)
      std::cout << clearedFibers << " fibers have been cleared" << std::endl;
    std::cout.flush();
  } 

  /*! Generate a list of vertices corresponding to the voxels of the origin ROI */
  template<typename T>
  void FibersTracker<T>::setOrigin(const char* origin_f) {
    double voxel[3], world[3];
    std::ifstream distFuncOrigin(origin_f,ios::binary);
	
    if (!distFuncOrigin.is_open()) {
      // This means the user provided only the coordinates of one voxel
      // this is supposed to be a valid voxel in the DTI volume
      sscanf(origin_f,"%lf,%lf,%lf",voxel,voxel+1,voxel+2);
      Vertex3<T> origin(voxel[0], voxel[1], voxel[2]);
      _originPoints.push_back(origin);
    } else {
      // the _origin volume can be defined in a different coordinates system, we go through millimetric coordinates
      _origin = origin_f;
      cimg_mapXYZ(_origin,x,y,z) {
	if (_origin(x,y,z) > 0) {
	  voxel[0] = x;
	  voxel[1] = y;
	  voxel[2] = z;

	  _origin.VoxelToWorld(voxel,world);

	  const unsigned int xx = (unsigned int) rint(voxel[0]);
	  const unsigned int yy = (unsigned int) rint(voxel[1]);
	  const unsigned int zz = (unsigned int) rint(voxel[2]);

	  _originPoints.push_back(Vertex3<T>(xx,yy,zz));
	}
      }
    }
    if (_originPoints.size() != 0)
      std::cerr << _originPoints.size() << " voxels in origin area" << std::endl;
    else
      throw CImgException("FibersTracker<T>::setOrigin() - No origin was found!");
  }

  template<typename T>
  bool FibersTracker<T>::_inVolume(const Vertex3<T>& pos) {
    T x = pos.GetX();
    T y = pos.GetY();
    T z = pos.GetZ();
    return (x>=0 && x<_width && y>=0 && y<_height && z>=0 && z<_depth);
  }

  template<typename T>
  bool FibersTracker<T>::_inVolume(const unsigned int& x, const unsigned int& y, const unsigned int& z) {
    return (x<_width && y<_height &&  z<_depth);
  }

  template class FibersTracker<float>;
  template class FibersTracker<double>;
}
