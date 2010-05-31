/*
  File:		fibersTrackerHARDI.cpp
  
  Author(s):		Maxime Descoteaux
  
  Created:		 7 Sep 2006
  
  Last Revision:	$Date: 2007/03/07 16:38:36 $
  
  To do:
  Attempt the commented Probabilistic tracking functions 
  to make it work...
  I don't know if medinria wants proba tracking
  (It is quite slow)

  Copyright (c) 2006 by Maxime Descoteaux, Odyssee Group, INRIA, 
  Sophia Antipolis, France.  Please see the copyright notice
  included in this distribution for full details.
*/

/**********************
     Include Files
**********************/
#include "fibersTrackerHARDI.hpp"

#include <stdlib.h>

#ifdef __APPLE__
using namespace std;
#endif

#ifdef WIN32
#define rint(x) floor((x) + 0.5)
#define srand48(x) srand(x)
#define drand48() rand()/float(RAND_MAX);
#define isnan(x) _isnan(x)
#pragma warning (disable:4661)
#endif

namespace AnatConnectivity {

  /*! Constructors - Destructor - Copy - Conversion */
  template<typename T>
  FibersTrackerHARDI<T>::FibersTrackerHARDI() {
    _width = _height = _depth = 0;
    
    _algo = EULER;
    _dt = 0.1;
    _valuesType = "GFA";
    _anisoThreshold = 0.05;
    _angleThreshold = 60.0;     // in degrees
    _angleConstraint = true;
    _fiberMinimumLength = 1;    // minimum euclidean length

    _verbose = false;
    _nocheck = true;
    _nFibers = 0;
    _nSeeds = 0;
    
    // for proba
    //     _exponent = 1;
    //     _stiffness = 0;
    
  }

  template<typename T>
  FibersTrackerHARDI<T>::~FibersTrackerHARDI() 
  {}
	
  


  template<typename T>
  FibersTrackerHARDI<T>::FibersTrackerHARDI(ODFImageType * odf, 
					    GradientListType grad,
					    vnl_matrix<T> SHMatrix,
					    FloatImageType::Pointer aniso,
					    FloatImageType::Pointer seeds) {

    m_hardi = odf;
    m_grad = grad;
    m_SHMatrix = SHMatrix;
    SetData(aniso, seeds);

    _algo = EULER;
    _dt = 0.1;
    _valuesType = "GFA";

    _anisoThreshold = 0.05;
    _angleThreshold = 60.0;     // in degrees
    _angleConstraint = true;
    _fiberMinimumLength = 1;   // minimum euclidean length

    _verbose = false;
    _nocheck = true;
    _nFibers = 0;
    _nSeeds = 0;

    m_ProcessObject = 0;
      
    // for proba
    // _exponent = 1;
    //     _stiffness = 0;

  }

  template<typename T>
  FibersTrackerHARDI<T>::FibersTrackerHARDI(const FibersTrackerHARDI<T>& ft) {
    m_hardi = ft.m_hardi;
    m_grad = ft.m_grad;
    m_SHMatrix = ft.m_SHMatrix;
    m_seeds = ft.m_seeds;
    _width = ft._width;
    _height = ft._height;
    _depth = ft._depth;

    _algo = ft._algo;
    _dt = ft._dt;
    _valuesType = ft._valuesType;

    m_anisoImage = ft.m_anisoImage;
    _anisoThreshold = ft._anisoThreshold;
    _angleThreshold = ft._angleThreshold;
    _angleConstraint = ft._angleConstraint;
    _fiberMinimumLength = ft._fiberMinimumLength;
    _verbose = ft._verbose;
    _nocheck = ft._nocheck;

    _nFibers = ft._nFibers;
    _nSeeds = ft._nSeeds;

    // for proba
    //     _exponent = ft._exponent;
    //     _stiffness = ft._stiffness;

    _tract = ft._tract;

    m_ProcessObject = ft.GetProcessObject();
  }


  template<typename T>
  FibersTrackerHARDI<T>& FibersTrackerHARDI<T>::operator=(const 
						   FibersTrackerHARDI<T>& ft) {
    m_hardi = ft.m_hardi;
    m_grad = ft.m_grad;
    m_SHMatrix = ft.m_SHMatrix;
    m_seeds = ft.m_seeds;
    _width = ft._width;
    _height = ft._height;
    _depth = ft._depth;

    _algo = ft._algo;
    _dt = ft._dt;
    _valuesType = ft._valuesType;

    m_anisoImage = ft.m_anisoImage;
    _anisoThreshold = ft._anisoThreshold;
    _angleThreshold = ft._angleThreshold;
    _angleConstraint = ft._angleConstraint;
    _fiberMinimumLength = ft._fiberMinimumLength;
    _verbose = ft._verbose;
    _nocheck = ft._nocheck;

    _nFibers = ft._nFibers;
    _nSeeds = ft._nSeeds;

    // for proba
//     _exponent = ft._exponent;
//     _stiffness = ft._stiffness;

    _tract = ft._tract;

    m_ProcessObject = ft.GetProcessObject();

    return *this;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::SetData(FloatImageType::Pointer aniso, 
				      FloatImageType::Pointer seeds) {
    
    m_anisoImage = aniso;
    m_seeds = seeds;
    
    FloatImageType::SizeType size = aniso->GetLargestPossibleRegion().GetSize();
    
    _width  = size[0];
    _height = size[1];
    _depth  = size[2];
  }

  template<typename T>
  void FibersTrackerHARDI<T>::SetCurrentNumberOfFibers(const unsigned int & n) {
    _nFibers = n;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::SetNumberOfSeeds(const unsigned int & n) {
    _nSeeds = n;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::SetTract(const Tract<T> & t) {
    _tract = t;  
  }

  template<typename T>
  void FibersTrackerHARDI<T>::SetTrackingMethod(const TrackingMethod& tm) {
    _algo = tm;
  }

  template<typename T>
  Tract<T> FibersTrackerHARDI<T>::GetTract() {
    return _tract;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::SetIntegrationStep(const T& dt) {
    _dt = dt;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::SetValuesType(const std::string& val) {
    _valuesType = val;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::VerboseOn() {
    _verbose = true;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::VerboseOff() {
    _verbose = false;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::CheckOn() {
    _nocheck = false;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::CheckOff() {
    _nocheck = true;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::SetAnisoThreshold(const T& a) {
    _anisoThreshold = a;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::SetFiberLengthThreshold(const int& l) {
    _fiberMinimumLength = l;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::SetAngleThreshold(const T& a) {
    _angleThreshold = a;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::EnableAngleConstraint() {
    _angleConstraint = true;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::DisableAngleConstraint() {
    _angleConstraint = false;
  }

  template<typename T>
  void FibersTrackerHARDI<T>::ClearShortFibers() {
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



  template<typename T>
  void FibersTrackerHARDI<T>::SetColor(const T & color) {
    _color = color;
    _setcolor = true;
  } 
	
  template<typename T>
  void FibersTrackerHARDI<T>::TrackDeterministic() {
    this->SetCurrentNumberOfFibers(0);
    std::cout << "Preparing to track " 
	      << this->_nSeeds << " fibers" << std::endl;
	
    switch(this->_algo) {
    case EULER:
      this->_tract.initialize(_nSeeds,1,this->_dt);
      this->_tract.setAttrDesc(0,std::string("GFA"));
      
      _euler_track();			
      break;
      /* case RK2:
	 case RK4:*/
    default:
      std::cerr << "FibersTrackerHARDI<T>::track() "
		<< "- Please specify a valid tracking method";
    }
    
    std::cout << _nFibers << " fibers reconstructed...\n";
  }
	
  template<typename T>
  void FibersTrackerHARDI<T>::_euler_track() {
    T aniso;    
    T angle = 90.0;  
    T x1, x2, y1, y2, z1, z2, dot;

    // First we setup discrete mesh and all neighbors on which
    // we will compute the fODF maxima
    { //find min angle in mesh
      /* approx angle between two discrete samplings on the sphere */    
      //       m_grad[0] -> radius
      //       m_grad[1] -> phi
      //       m_grad[2] -> theta
	
      x1 = std::cos(((m_grad)[2])[1])*std::sin(((m_grad)[2])[2]);
      y1 = std::sin(((m_grad)[2])[1])*std::sin(((m_grad)[2])[2]);
      z1 = std::cos(((m_grad)[2])[2]);
    
      /* finding minimum angle between samplings */
      for(unsigned int i = 0; i < m_grad.size(); i++) {
	if(i != 2) {
	  x2 = std::cos(((m_grad)[i])[1])*std::sin(((m_grad)[i])[2]);
	  y2 = std::sin(((m_grad)[i])[1])*std::sin(((m_grad)[i])[2]);
	  z2 = std::cos(((m_grad)[i])[2]);

	  dot = x1*x2 + y1*y2 + z1*z2;
	  dot = 180*std::acos(dot)/M_PI;
		  
	  if(dot < angle)
	    angle = dot;
	}
      }
    }    

    if(_verbose)
      std::cout << "minimum angle in mesh " << angle << "...\n";
    
    std::vector<int> *Nbors = new std::vector<int>[m_grad.size()];
    { // find neighbors to all mesh points 
      std::vector<int> neighbors_i;
      for(unsigned int i = 0; i < m_grad.size(); i++) {
	x1 = std::cos(((m_grad)[i])[1])*std::sin(((m_grad)[i])[2]);
	y1 = std::sin(((m_grad)[i])[1])*std::sin(((m_grad)[i])[2]);
	z1 = std::cos(((m_grad)[i])[2]);
	
	/* 
	   look at other possible direction sampling neighbors
	   if a sampling directions is within +- 3 degrees from i,
	   we consider it and check if i is bigger
	*/
	for(unsigned int j=0; j<m_grad.size(); j++) {
	  if(j != i ) {
	    x2 = std::cos(((m_grad)[j])[1])*std::sin(((m_grad)[j])[2]);
	    y2 = std::sin(((m_grad)[j])[1])*std::sin(((m_grad)[j])[2]);
	    z2 = std::cos(((m_grad)[j])[2]);
	    
	    
	    dot = x1*x2 + y1*y2 + z1*z2;
	    dot = 180*std::acos(dot)/M_PI;
	    
	    if(dot > 90)
	      dot = 180 - dot;
	    
	    if(fabs(angle - dot) < angle/3) { 
	      /* allow +- 1/3* angle  with 
		 neighborhing expected direction */
	      neighbors_i.push_back(j);
	    }
	  }
	}
	Nbors[i] = neighbors_i;
	neighbors_i.clear();
      }
    }
    
    if(_verbose)
      std::cout << "Done preparing for maxima extraction...\n";

    FloatIteratorType itSeed(m_seeds, m_seeds->GetLargestPossibleRegion());
    FloatIteratorType itGFA(m_anisoImage, 
			    m_anisoImage->GetLargestPossibleRegion());
    ODFIteratorType itODF (m_hardi, m_hardi->GetLargestPossibleRegion());


    unsigned long numSeeds = m_seeds->GetLargestPossibleRegion().GetNumberOfPixels();
    unsigned long step     = numSeeds/1000;
    unsigned long progress = 0;
    

    if( !m_ProcessObject.IsNull() )
      m_ProcessObject->UpdateProgress (0.0);
    
    
    /*! Proceed with actual tracking */
    while (!itSeed.IsAtEnd() ) {

      if( itSeed.Get() > 0 ) {
	aniso = itGFA.Get();
	//std::cout << "aniso : " << aniso << "\n";

	if(aniso >= _anisoThreshold) {
	  unsigned int nfiberstemp = _nFibers; 
	  
	  /* going forward */
	  //std::cout << "going forward\n";
	  _eulerPropagate(itODF, 1,
			   _tract[nfiberstemp],angle,Nbors);
	  
	  /* going backward */
	  _eulerPropagate(itODF,-1, 
			   _tract[nfiberstemp],angle,Nbors);
	  
	  _nFibers++;
	  //std::cout << ">> " << _nFibers << "\t\t\r";
	  //std::cout.flush();
	}       
      }
      
      ++itSeed;
      ++itGFA;
      ++itODF;
      ++progress;

      if( !m_ProcessObject.IsNull() )
      {
	if( step && (progress%step)==0 )
	{
	  m_ProcessObject->UpdateProgress( (double)progress/double(numSeeds) );
	}
      }
    }
    std::cout  << std::endl;
    this->_tract.setNumFibers(_nFibers);

    if( !m_ProcessObject.IsNull() )
      m_ProcessObject->UpdateProgress (1.0);
    
    delete [] Nbors;
  }
	
  template<typename T>
  void FibersTrackerHARDI<T>::_eulerPropagate(ODFIteratorType itODF,
					       const int& fwdbwd, 
					       Fiber<T>& fiber,
					       const T &angle,
					       const std::vector<int> Nbors[]) {

    typename ODFImageType::IndexType voxel = itODF.GetIndex();
    int x = voxel[0], y = voxel[1], z = voxel[2];
    
    Vertex3<T> pos(x,y,z);
    Vector3<T> speed, hardi_dir;
    Node<T> node;
    
    ODFType odf = itODF.Get();
    vnl_matrix<T> sh(1, odf.Size());
    for(unsigned int i = 0; i<odf.Size(); i++) {
      sh (0,i) =  odf[i];
    }


    T aniso = m_anisoImage->GetPixel(voxel);

    // Initialization
    node.set(pos);
    if(_setcolor)
      node.setAttribute(0, _color);
    else 
      node.setAttribute(0, aniso);
    
    if (fwdbwd == 1)
      fiber.addFnode(node);
    else
      fiber.addBnode(node);
    
    T max_threshold = 0.5;

    
    std::vector<direction_value> hardi_dirs = _computeMaxima(sh, max_threshold,
							     angle, Nbors);

    // determine direction to use for tracking
    if(hardi_dirs.size() != 0) {
      Vector3<T> newvec(hardi_dirs[0].x, hardi_dirs[0].y, hardi_dirs[0].z);
      hardi_dir = newvec;

      // If more than 1 maxima, we choose the strongest peak to start the 
      // tracking if the Check option is On
      if(hardi_dirs.size() > 1 && !_nocheck) {
	if(hardi_dirs.size() == 2) {
	  T v1,v2;
	  v1 = hardi_dirs[0].v;
	  v2 = hardi_dirs[1].v;
	  
	  if(v2 > v1) {
	    Vector3<T> v(hardi_dirs[1].x, hardi_dirs[1].y, hardi_dirs[1].z);
	    hardi_dir = v;
	  }
	  else { 
	    Vector3<T> v(hardi_dirs[0].x, hardi_dirs[0].y, hardi_dirs[0].z);
	    hardi_dir = v;
	  }
	}
	else if(hardi_dirs.size() == 3) {
	  T v1,v2,v3;
	  v1 = hardi_dirs[0].v;
	  v2 = hardi_dirs[1].v;
	  v3 = hardi_dirs[2].v;
	  
	  T min = _get_min(v1,v2,v3);
	  
	  if(v3 == min) {
	    if(v1 > v2) {
	      Vector3<T> v(hardi_dirs[0].x, hardi_dirs[0].y, hardi_dirs[0].z);
	      hardi_dir = v;
	    }
	    else {
	      Vector3<T> v(hardi_dirs[1].x, hardi_dirs[1].y, hardi_dirs[1].z);
	      hardi_dir = v;
	    }
	  }
	  else if(v2 == min) {
	    if(v1 > v3) {
	      Vector3<T> v(hardi_dirs[0].x, hardi_dirs[0].y, hardi_dirs[0].z);
	      hardi_dir = v;
	    }
	    else {
	      Vector3<T> v(hardi_dirs[2].x, hardi_dirs[2].y, hardi_dirs[2].z);
	      hardi_dir = v;
	    }
	  }
	  else {
	    if(v2 > v3) {
	      Vector3<T> v(hardi_dirs[1].x, hardi_dirs[1].y, hardi_dirs[1].z);
	      hardi_dir = v;
	    }
	    else {
	      Vector3<T> v(hardi_dirs[2].x, hardi_dirs[2].y, hardi_dirs[2].z);
	      hardi_dir = v;
	    }
	  }
	}
      }
    }


    speed = fwdbwd * this->_dt * hardi_dir;
    
    pos[0] += speed[0];
    pos[1] += speed[1];
    pos[2] += speed[2];

    bool angle_constraint = true;
    while(this->_inVolume(pos) && (aniso >= this->_anisoThreshold)
	  && angle_constraint) {
      /* 
	 could improve this by computing GFA on the interpolated
	 spherical function....  but sometimes the given anisotropy
	 image can be something else than GFA, so better to take closest
	 anisotropy voxel 
      */
      voxel[0] = (int) rint(pos.X());
      voxel[1] = (int) rint(pos.Y());
      voxel[2] = (int) rint(pos.Z());

      // clamping
      if( (unsigned int)voxel[0]>_width-1 )  voxel[0] = _width-1;
      if( (unsigned int)voxel[1]>_height-1 ) voxel[1] = _height-1;
      if( (unsigned int)voxel[2]>_depth-1 )  voxel[2] = _depth-1;

      aniso = m_anisoImage->GetPixel(voxel);
      sh = _interpolate(pos.X(), pos.Y(), pos.Z());

      node.set(pos);
      if(_setcolor)
	node.setAttribute(0, _color);
      else 
	node.setAttribute(0, aniso);
      
      if(fwdbwd == 1)
	fiber.addFnode(node);
      else
	fiber.addBnode(node);
      
      angle_constraint = _eulerIntegrate(pos,speed,sh,fwdbwd,angle,Nbors);
      
    }
    
  }

  template<typename T>
  std::vector< direction_value > FibersTrackerHARDI<T>::_computeMaxima(
					      vnl_matrix<T> odf,
					      T max_thresh,
					      const T & angle,
					      const std::vector<int> Nbors[]) {
    std::vector<direction_value> max_dir;
    std::vector<T> hemisODF; 

    T epsilon = 0.0;  //for equality measurement 
    vnl_matrix<T> ODF = odf*m_SHMatrix;

    for(unsigned int i = 0; i < m_grad.size(); i++) {
      if(ODF(0,i) < 0) {
	//cout << "ODF has negative entries!!!\r";
	//cout.flush();
	hemisODF.push_back(0);
      }
      else
	hemisODF.push_back(ODF(0,i));
    }
	
    std::vector<T> norm_hemisODF = Utils::normalize(hemisODF);
    
    std::vector<int> indices;
    bool candidate = false;        
    for(unsigned int i = 0; i < m_grad.size(); i++) {
      candidate = false;
      
      if(norm_hemisODF[i] > max_thresh) { //potential maximum
	/* 
	   look at other possible direction sampling neighbors
	   if a sampling directions is within +- 3 degrees from i,
	   we consider it and check if i is bigger
	*/
	for(unsigned int j=0; j<Nbors[i].size(); j++) {
	  if(norm_hemisODF[i] - norm_hemisODF[Nbors[i][j]] > epsilon) { 
	    /* 
	       has to be epsilon bigger 
	       This is to get rid of some noisy peaks detected 
	       with ODF value close to the thresh
	    */
	    candidate = true;
	  }
	  else {                /* wrong candidate */
	    candidate = false;
	    break;
	  }
	}
      }
      
      if(candidate) {
	indices.push_back(i);
	direction_value d;
	// d.x = ((m_grad)[i])[0]; 
	// d.y = ((m_grad)[i])[1]; 
	// d.z = ((m_grad)[i])[2]; 
	// d.v = norm_hemisODF[i];

	// saving maxima in cartesian coordinate
	d.x = std::cos(m_grad[i][1])*std::sin(m_grad[i][2]);
	d.y = std::sin(m_grad[i][1])*std::sin(m_grad[i][2]);
	d.z =  std::cos(m_grad[i][2]);
	d.v = norm_hemisODF[i];
	max_dir.push_back(d);
      }
    }
    
    return max_dir;
  }

  

  template<typename T>
  bool FibersTrackerHARDI<T>::_eulerIntegrate(Vertex3<T>& pos, 
					       Vector3<T>& speed, 
					       vnl_matrix<T>& sh,
					       const int& fwdbwd,
					       const T &angle_min,
					       const std::vector<int> Nbors[]) {
    
    Vector3<T> step;
    Vector3<T> newspeed;
    T angle = 0.0;   //angle between speed (v_in) and ODF max closest to it

    T max_threshold = 0.5;

    /* get angles in cartesian coordinates x,y,z */
    std::vector<direction_value> hardi_dirs = _computeMaxima(sh, max_threshold,
							     angle, Nbors);

    /*
      MULTIPLE DIRECTIONS EXTENSION HERE
    */
    // if _nocheck, simply taking first direction hardi_dirs
    // not checking if it is the most important one
    if(hardi_dirs.size() != 0) {
      if(_nocheck) {
	Vector3<T> newspeed2(hardi_dirs[0].x, hardi_dirs[0].y, hardi_dirs[0].z);
	newspeed = newspeed2;
	
	T angle = newspeed.dot(speed);
	
	if(angle >= -1.0 - M_COND_CHECK && angle <= -1.0 + M_COND_CHECK)
	  angle = 180;
	else if(angle <= 1.0 + M_COND_CHECK && angle >= 1.0 - M_COND_CHECK)
	  angle = 0;
	else {
	  angle = 180*std::acos(angle)/M_PI;
	}

	if(angle > 90)
	  angle = 180 - angle;
      }
      else {
	if(hardi_dirs.size() == 1) {
	  Vector3<T> newspeed2(hardi_dirs[0].x, 
			       hardi_dirs[0].y, hardi_dirs[0].z);
	  newspeed = newspeed2;
	}
	else if(hardi_dirs.size() == 2) {
	  T angle1 = 0.0, angle2 = 0.0, angle = 0.0;
	  
	  Vector3<T> newspeed1(hardi_dirs[0].x, 
			       hardi_dirs[0].y, hardi_dirs[0].z);
	  Vector3<T> newspeed2(hardi_dirs[1].x, 
			       hardi_dirs[1].y, hardi_dirs[1].z);
					
	  T newnorm1 = newspeed1.norm();
	  T newnorm2 = newspeed2.norm();
	  T oldnorm = speed.norm();
					
	  double temp = (newspeed1.dot(speed))/(newnorm1*oldnorm);
	  double temp2 = (newspeed2.dot(speed))/(newnorm2*oldnorm);

	  if(temp >= -1.0 - M_COND_CHECK && temp <= -1.0 + M_COND_CHECK)
	    angle1 = 180;
	  else if(temp <= 1.0 + M_COND_CHECK && temp >= 1.0 - M_COND_CHECK)
	    angle1 = 0;
	  else {
	    angle1 = 180*std::acos((double)temp)/M_PI;
	    if( isnan(angle1) ) {
	      std::cout << "Quitting angle is NAN!\n";
	      exit(1);
	    }
	  }

	  if(temp2 >= -1.0 - M_COND_CHECK && temp2 <= -1.0 + M_COND_CHECK)
	    angle2 = 180;
	  else if(temp2 <= 1.0 + M_COND_CHECK && temp2 >= 1.0 - M_COND_CHECK)
	    angle2 = 0;
	  else {
	    angle2 = 180*std::acos((double)temp2)/M_PI;
	    if( isnan(angle2) ) {
	      std::cout << "Quitting angle is NAN!\n";
	      exit(1);
	    }
	  }
	  if(angle1 > 90) {
	    angle1 = 180 - angle1;
	  }
	  if(angle2 > 90)
	    angle2 = 180 - angle2;
	  if(angle1 < angle2) {
	    newspeed = newspeed1;
	    angle = angle1;
	  }
	  else {
	    newspeed = newspeed2;
	    angle = angle2;
	  }
							  
	}
	else {
	  if(hardi_dirs.size() > 3) {
// 	    if(_verbose) {
// 	      std::cout << "WARNING: FibersTrackerHARDI::_eulerIntegrate: "
// 		"more than 3 maxima...\t";
// 	      std::cout << "exiting tract at" << pos << "...\n";
// 	    }
	    return false;
	  }
	  else {						
	    T angle1 = 0.0, angle2 = 0.0, angle3 = 0.0;
	    
	    Vector3<T> newspeed1(hardi_dirs[0].x, 
				 hardi_dirs[0].y, hardi_dirs[0].z);
	    Vector3<T> newspeed2(hardi_dirs[1].x, 
				 hardi_dirs[1].y, hardi_dirs[1].z);
	    Vector3<T> newspeed3(hardi_dirs[2].x, 
				 hardi_dirs[2].y, hardi_dirs[2].z);
						
	    T newnorm1 = newspeed1.norm();
	    T newnorm2 = newspeed2.norm();
	    T newnorm3 = newspeed3.norm();
	    T oldnorm = speed.norm();
				
	    double temp = (newspeed1.dot(speed))/(newnorm1*oldnorm);
	    double temp2 = (newspeed2.dot(speed))/(newnorm2*oldnorm);
	    double temp3 = (newspeed3.dot(speed))/(newnorm3*oldnorm);

	    if(temp >= -1.0 - M_COND_CHECK && temp <= -1.0 + M_COND_CHECK)
	      angle1 = 180;
	    else if(temp <= 1.0 + M_COND_CHECK && temp >= 1.0 - M_COND_CHECK)
	      angle1 = 0;
	    else {
	      angle1 = 180*std::acos((double)temp)/M_PI;
	      if( isnan(angle1) ) {
		std::cout << "Quitting angle is NAN!\n";
		exit(1);
	      }
	    }

	    if(temp2 >= -1.0 - M_COND_CHECK && temp2 <= -1.0 + M_COND_CHECK)
	      angle2 = 180;
	    else if(temp2 <= 1.0 + M_COND_CHECK && temp2 >= 1.0 - M_COND_CHECK)
	      angle2 = 0;
	    else {
	      //std::cout << "computing std::acos...\t";
	      angle2 = 180*std::acos((double)temp2)/M_PI;
	      if( isnan(angle2) ) {
		std::cout << "Quitting angle is NAN!\n";
		exit(1);
	      }
	    }
	    
	    if(temp3 >= -1.0 - M_COND_CHECK && temp3 <= -1.0 + M_COND_CHECK)
	      angle3 = 180;
	    else if(temp3 <= 1.0 + M_COND_CHECK && temp3 >= 1.0 - M_COND_CHECK)
	      angle3 = 0;
	    else {
	      //std::cout << "computing std::acos...\t";
	      angle3 = 180*std::acos((double)temp3)/M_PI;
	      if( isnan(angle3) ) {
		std::cout << "Quitting angle is NAN!\n";
		exit(1);
	      }
	    }

	    if(angle1 > 90)
	      angle1 = 180 - angle1;
	    if(angle2 > 90)
	      angle2 = 180 - angle2;
	    if(angle3 > 90)
	      angle3 = 180 - angle3;
					
	    T min = _get_min(angle1, angle2, angle3);
						
	    /* At this point,
	       only checking for one splitting, the minimum of the two
	       remaining directions 
	    */
	    if(min == angle1) {
	      newspeed = newspeed1;
	      angle = angle1;
	    }
	    if(min == angle2) {
	      newspeed = newspeed2;
	      angle = angle2;
	    }
	    if(min == angle3) {
	      newspeed = newspeed3;
	      angle = angle3;
	    }
	  }
	}
      }
    }

    T dotproduct = newspeed.dot(speed);
		
    // Make sure we keep propagating in the right direction
    if(dotproduct < 0.0)
      newspeed.flip();
    
    // Enforce angle threshold
    if(this->_angleConstraint && 
       angle < this->_angleThreshold) { 
      
      step = this->_dt*newspeed;
      
      if(fabs(step.X()) <= M_COND_CHECK 
	 && fabs(step.Y()) <= M_COND_CHECK 
	 && fabs(step.Z()) <= M_COND_CHECK) {

	return false;
      }
			      
      pos.translate(step.X(), step.Y(), step.Z());
      speed = newspeed;

      return true;
    }
    else {
      /* If we are here, the tracking for this tract should be stopped */
      //       if(_verbose) {
      // 	std::cout << "STOP TRACKING!!!\t";
      // 	std::cout << newnorm << "," << oldnorm << "," 
      // 	     << dotproduct << "," << angle  << "\n";
      //       }
			
      return false;
    }
		
  }

  template<typename T>
  T FibersTrackerHARDI<T>::_get_min(const T &a, const T &b, const T &c) {
    if(a < b){
      if(c < a)
	return c;
      else {
	if(b < c)
	  return a;
	else
	  return c;
      }
    }
    else {  // b < a
      if(c < b)
	return c;
      else {  // b < c
	if(a < c)  
	  return c;
	else
	  return a;
      }
    }
  }


  template<typename T>
  bool FibersTrackerHARDI<T>::_inVolume(const Vertex3<T>& pos) {
    T x = pos.GetX();
    T y = pos.GetY();
    T z = pos.GetZ();
    return (x>=0 && x<_width && y>=0 && y<_height && z>=0 && z<_depth);
  }

  template<typename T>
  vnl_matrix<T> FibersTrackerHARDI<T>::_interpolate(const T& ffx, 
						    const T& ffy, 
						    const T& ffz) {

    typename ODFImageType::IndexType voxel,voxel2;
    ODFType odf1,odf2;


    /*! Trilinear interpolation with Neumann boundary conditions */
    // Enforce boundary conditions and compute weights
    const T fx = ffx<0.0 ? 0.0 : (ffx>_width-1 ? _width-1  : ffx);
    const T fy = ffy<0.0 ? 0.0 : (ffy>_height-1? _height-1 : ffy);
    const T fz = ffz<0.0 ? 0.0 : (ffz>_depth-1 ? _depth-1  : ffz);

    const unsigned int x = (unsigned int) std::floor(fx);
    const unsigned int y = (unsigned int) std::floor(fy);
    const unsigned int z = (unsigned int) std::floor(fz);

    const T dx = fx-x;
    const T dy = fy-y;
    const T dz = fz-z;

    const unsigned int nx = dx>0.0 ? x+1 : x;
    const unsigned int ny = dy>0.0 ? y+1 : y;
    const unsigned int nz = dz>0.0 ? z+1 : z;

    // Compute interpolated value at (fx,fy,fz)
    voxel[0] = x; voxel[1] = y; voxel[2] = z;
    voxel2[0] = nx; voxel2[1] = y; voxel2[2] = z;
    odf1 = m_hardi->GetPixel(voxel);
    odf2 = m_hardi->GetPixel(voxel2);
    vnl_matrix<T> sh1(1, odf1.Size()), sh2(1, odf1.Size());
    for(unsigned int i = 0; i<odf1.Size(); i++) {
      sh1(0,i) = odf1[i];
      sh2(0,i) = odf2[i]; 
    }

    vnl_matrix<T> valx0 = (1-dx)*sh1  + (dx)*sh2;

    voxel[0] = x; voxel[1] = ny; voxel[2] = z;
    voxel2[0] = nx; voxel2[1] = ny; voxel2[2] = z;
    odf1 = m_hardi->GetPixel(voxel);
    odf2 = m_hardi->GetPixel(voxel2);
    for(unsigned int i = 0; i<odf1.Size(); i++) {
      sh1(0,i) = odf1[i];
      sh2(0,i) = odf2[i]; 
    }
    
    vnl_matrix<T> valx1 = (1-dx)*sh1 + (dx)*sh2;
    
    const vnl_matrix<T> valy0 = (1-dy)*valx0 + (dy)*valx1;
    
    voxel[0] = x; voxel[1] = y; voxel[2] = nz;
    voxel2[0] = nx; voxel2[1] = y; voxel2[2] = nz;
    odf1 = m_hardi->GetPixel(voxel);
    odf2 = m_hardi->GetPixel(voxel2);
    for(unsigned int i = 0; i<odf1.Size(); i++) {
      sh1(0,i) = odf1[i];
      sh2(0,i) = odf2[i]; 
    }
    valx0 = (1-dx)*sh1  + (dx)*sh2;
    
    voxel[0] = x; voxel[1] = ny; voxel[2] = nz;
    voxel2[0] = nx; voxel2[1] = ny; voxel2[2] = nz;
    odf1 = m_hardi->GetPixel(voxel);
    odf2 = m_hardi->GetPixel(voxel2);
    for(unsigned int i = 0; i<odf1.Size(); i++) {
      sh1(0,i) = odf1[i];
      sh2(0,i) = odf2[i]; 
    }
    valx1 = (1-dx)*sh1 + (dx)*sh2;

    const vnl_matrix<T> valy1 = (1-dy)*valx0 + (dy)*valx1;
    
    return (1-dz)*valy0 + (dz)*valy1;
  }



  template class FibersTrackerHARDI<float>;
  template class FibersTrackerHARDI<double>;
}




///////////////////////////////////////////////////////////
//
// PROBABILISTIC TRACKING,
// Everything is there and works 
// But one needs to remove MImg, HARDI, dependences
// SphericalFunction and CImg dependences
//
///////////////////////////////////////////////////////////

//   template<typename T>
//   void FibersTrackerHARDI<T>::_proba_track() {
//     double voxel[3], world[3];
		
//     _ntimes_touched = new MImg<T>(this->_anisoImage.width,
// 				  this->_anisoImage.height,
// 				  this->_anisoImage.depth);
//     _ntimes_touched->SetTransform(_hardi.GetTransform());
//     _ntimes_touched->fill(0);

//     T aniso = 0;
//     int old_anglei, crossing_factor, anglei;
//     T* proba;
		
//     int start[3];
//     std::vector<direction> grad = _hardi.getGrad();
							
//     /*! Proceed with actual tracking */
//     cimg_mapXYZ(this->_seeds, x,y,z) {
//       if (this->_seeds(x,y,z) > 0) {
// 	/* Since the seeds may be defined in a different volume, 
// 	   we go through millimetric coordinates */
// 	voxel[0] = x;
// 	voxel[1] = y;
// 	voxel[2] = z;
				
// 	this->_seeds.VoxelToWorld(voxel,world);				
// 	_hardi.WorldToVoxel(world,voxel);
				
// 	const unsigned int xx = (unsigned int) rint(voxel[0]);
// 	const unsigned int yy = (unsigned int) rint(voxel[1]);
// 	const unsigned int zz = (unsigned int) rint(voxel[2]);
				
// 	start[0] = xx; start[1] = yy; start[2] = zz;
				
// 	if(_anisoImage(xx,yy,zz) > _anisoThreshold) {
// // 	  std::cout << "Initiswating tracking at [" << xx << ", " << yy 
// // 		    << ", " << zz << "]\n";
// // 	  std::cout.flush();
					
// 	  for(int imol = 0; imol < _molecules; imol++) {
// // 	    std::cout << ">> " << imol << " / " << _molecules 
// // 		      << " / " << this->_numFibers*_molecules << "\t\t\r"; 
// // 	    std::cout.flush();
					
// 	    bool fiber_flag = false; /* check wether you keep all fibers 
// 					If a fiber contains only voxels 
// 					already visited
// 					we remove it.
// 				     */
	
// 	    /* no history on the angle */
// 	    old_anglei = -1;    
	    
// 	    /* multiplication factor when fiber crossing is reached */
// 	    crossing_factor = 1; 
// 	    Vertex3<T> pos(xx,yy,zz);  /* start # _molecules from xx,yy,zz */
// 	    bool wayout = false;
						
// 	    for(int jump = 0; jump < _njumps; jump++) {
// 	      /* if first time you go through a voxel,
// 		 want to keep the fiber for sure
// 	      */
							
// 	      aniso = (*_ntimes_touched)((int)pos.X(), 
// 					 (int)pos.Y(), (int)pos.Z());
							
// 	      if(aniso == 0)
// 		fiber_flag = true;
	      
// 	      /* increment number of times passed in voxel pos */
// 	      (*_ntimes_touched)((int)pos.X(), (int)pos.Y(), (int)pos.Z()) += 
// 		crossing_factor; 
	      
// 	      /* add xx,yy,zz to fibers if not too long already */
// 	      Node<T> node;
// 	      node.set(pos);
// 	      node.setAttribute(0, aniso);
// 	      this->_tract[_nFibers].addFnode(node);
							
// 	      std::vector< Vertex3<T> > neighbors;
							
// 	      for(unsigned int i = 0; i < grad.size(); i++) {
// 		direction d;
// 		d.x = std::cos(grad[i].y)*std::sin(grad[i].z);
// 		d.y = std::sin(grad[i].y)*std::sin(grad[i].z);
// 		d.z = std::cos(grad[i].z);
// 		Vertex3<T> v(pos.X() + d.x*this->_dt, 
// 			     pos.Y() + d.y*this->_dt, 
// 			     pos.Z() + d.z*this->_dt);
		
// 		neighbors.push_back(v);
// 	      }

// 	      /* get proba distribution according to ODF */
// 	      proba = _get_proba(pos,neighbors,old_anglei,&wayout);
							
// 	      if(wayout == true) {
// 		free(proba);
// 		neighbors.clear();
// 		break;
// 	      }
							
// 	      /* get direction with given proba distribution  */
// 	      anglei = _mkrand(proba, neighbors.size()); 
// 	      /* anglei in [0 ... size] */
							
// 	      /* jump to new spot */
// 	      pos = neighbors[anglei];	
	      
// 	      /* break if mask is reached */
// 	      if(_anisoImage((int)(pos.X()+0.5),
// 			     (int)(pos.Y()+0.5),
// 			     (int)(pos.Z()+0.5)) < this->_anisoThreshold) {
// 		//cout << "reaching the mask at ... stop this molecule...\n";
// 		break;
// 	      }
							
// 	      old_anglei = anglei;
							
// 	    } // end of jump loop
						
// 	    if(fiber_flag) /* keep fiber and increment */
// 	      _nFibers++;
// 	    else          /* delete the fiber because only previsited voxels */ 
// 	      this->_tract[_nFibers].clear();
// 	  }
					
// 	} // loop on the seeds 
// 	else {
// 	  ;// if(_verbose)
// 	   //  cout << aniso << " seed point not in mask...\n";
// 	}
//       }
//     }
		
//     cout << _nFibers << " fibers found\n";
//     cout << (*_ntimes_touched)(start[0],start[1],start[2]) 
// 	 << " times touched in start voxel\n";
		
//     int max = 0;
//     for(unsigned int i = 0; i < _ntimes_touched->width; i++)
//       for(unsigned int j = 0; j < _ntimes_touched->height; j++)
// 	for(unsigned int k = 0; k < _ntimes_touched->depth; k++) {
// 	  if((*_ntimes_touched)(i,j,k) > max)
// 	    max = (int)(*_ntimes_touched)(i,j,k);
// 	}
    
//     //cout << "Max number of times in a voxel: " << max << "\n";
//     (*_ntimes_touched)(start[0],start[1],start[2]) = max+1;
		
//     int count = 0;
//     for(unsigned int i = 0; i < _ntimes_touched->width; i++)
//       for(unsigned int j = 0; j < _ntimes_touched->height; j++)
// 	for(unsigned int k = 0; k < _ntimes_touched->depth; k++) {
// 	  if((*_ntimes_touched)(i,j,k))
// 	    count++;
// 	}
		
//     cout << count << " connected voxels to the start point...\n";
		
//     /* log normalization of the output depending on number of molecules */
//     T log_moles = log((T)_molecules);
//     if(log_moles == 0) log_moles = 1; /* in case _molecules == 1 */
		
//     for(unsigned int i = 0; i < _ntimes_touched->width; i++)
//       for(unsigned int j = 0; j < _ntimes_touched->height; j++)
// 	for(unsigned int k = 0; k < _ntimes_touched->depth; k++) {
// 	  T temp = (*_ntimes_touched)(i,j,k);
// 	  if(temp > 0) {
// 	    (*_ntimes_touched)(i,j,k)  = log(temp) / log_moles;
					
// 	  }
				
// 	}
		
//     std::cout  <<std::endl;
//     this->_tract.setNumFibers(_nFibers);
//   }
	
//   /*! 
//     computes probabilities for jump to neighbors from diffusion _trans_proba
//     using the fiber ODF value
//   */
//   template<typename T>
//   T *FibersTrackerHARDI<T>::_get_proba(Vertex3<T> &pos, 
// 				       std::vector< Vertex3<T> > &neighbors, 
// 				       const int &old_anglei, bool *wayout) {

//     T *proba, angle, proba_sum; 
		
//     Vertex3<T> goal;
//     bool norm = true;

//     proba = (T *) malloc(neighbors.size() * sizeof(T));
		
//     /* get interpolated ODF at position pos */
//     CImg<T> odf1 = _hardi.interpolate(pos.X(), pos.Y(), pos.Z());
//     CImg<T> Btranspose = _hardi.getSHmatrix().get_transpose();
		
//     CImg<T> temp1 =  odf1 * Btranspose;
//     if(norm)
//       temp1.normalize(0,1);
		
		
//     CImg<T> odf2;
//     CImg<T> temp2;
		
//     for(unsigned int i = 0; i < neighbors.size(); i++) {
			
//       if(old_anglei >= 0)
// 	angle = _angles(old_anglei, i);
//       else
// 	angle = 0;
			
//       if(angle / M_PI * 180 < this->_angleThreshold) {				
// 	goal = neighbors[i];
				
// 	odf2 = _hardi.interpolate(goal.X(), goal.Y(), goal.Z());
// 	temp2 = odf2 * Btranspose;
// 	if(norm)
// 	  temp2 = temp2.normalize(0,1);
				
// 	T temp22 = temp2(i,0);
				
// 	if(_stiffness > 0)
// 	  temp22 = temp22 * exp(-0.5 * _stiffness * angle * angle);

// 	proba[i] = pow(temp22 * temp1(i,0), _exponent); 
				
//       }
//       else
// 	proba[i] = 0;
//     }
		
//     proba_sum = 0.0;
//     for(unsigned int i = 0; i < neighbors.size(); i++) {
//       proba_sum += proba[i];
//     }

//     T sum = 0.0;
//     if(proba_sum == 0)
//       *wayout = true;
//     else {
//       *wayout = false;
//       for(unsigned int i = 0; i < neighbors.size(); i++) {
// 	proba[i] = proba[i]/proba_sum;
// 	sum+= proba[i];
//       }
//     }

//     return proba;
//   }
		
//   template<typename T>
//   T FibersTrackerHARDI<T>::_get_angle(const int & anglei, const int & i) {
//     T  x_1, y_1, z_1, x_2, y_2, z_2;
//     T s_prod, norm_1, norm_2, angle;
		
//     std::vector<direction> grad = _hardi.getGrad();

//     /*determine orientation. */
//     x_1 = grad[anglei].x;  y_1 = grad[anglei].y;   z_1 = grad[anglei].z;
//     x_2 = grad[i].x;  y_2 = grad[i].y;   z_2 = grad[i].z;

//     s_prod = x_1*x_2 + y_1*y_2 + z_1*z_2;
//     norm_1 = x_1*x_1 + y_1*y_1 + z_1*z_1;
//     norm_2 = x_2*x_2 + y_2*y_2 + z_2*z_2;

//     angle = acos(s_prod/( sqrt(norm_1*norm_2) ));
		
//     return angle;
//   }

//   template<typename T>
//   int FibersTrackerHARDI<T>::_mkrand(const T *proba, const int & size) {
//     T x,integral;
//     int i,randint;

//     //"Generate random number with proba distribution on the hemisphere..."
		
//     x=(T)drand48(); /*uniformly distributed in [0,1)*/
		
//     /*find y with: integral_0^y (pd(y')) dy' == x */
//     integral=0.;
//     i=-1;
//     while ((x>integral)&& (i<size)) {
//       i += 1;
//       integral += *(proba+i);
//     }

//     randint=i;
//     return randint;
//   }


//   template<typename T>
//   void FibersTrackerHARDI<T>::SaveTractogram(const std::string &tractogram) {
//     _ntimes_touched->Save(tractogram);
//   }

//   template<typename T>
//   void FibersTrackerHARDI<T>::TrackProba() {
		
//     /* determine number of fibers */
//     this->_numFibers=0;
//     cimg_mapXYZ(this->_seeds,x,y,z) {
//       if (this->_seeds(x,y,z) > 0)
// 	this->_numFibers++;
//     }
		
//     /* initialize array of angles between each discrete point on the sphere */
//     std::vector<direction> grad = _hardi.getGrad();
//     CImg<T> angles(grad.size(), grad.size(),1,1,0);
		
//     for(unsigned int i = 0; i < grad.size(); i++) {
//       //cout << "\n";
//       for(unsigned int j = i; j < grad.size(); j++) {
// 	if(i == j)
// 	  angles(i,j) = 0;
// 	else {
// 	  direction d1,d2;
// 	  d2.x = std::cos(grad[i].y)*std::sin(grad[i].z);
// 	  d2.y = std::sin(grad[i].y)*std::sin(grad[i].z);
// 	  d2.z = std::cos(grad[i].z);
// 	  d1.x = std::cos(grad[j].y)*std::sin(grad[j].z);
// 	  d1.y = std::sin(grad[j].y)*std::sin(grad[j].z);
// 	  d1.z = std::cos(grad[j].z);
						
// 	  T s_prod, norm_1, norm_2, angle;
						
// 	  /*determine orientation. */
						
// 	  s_prod = d1.x*d2.x + d1.y*d2.y + d1.z*d2.z;
// 	  norm_1 = d1.x*d1.x + d1.y*d1.y + d1.z*d1.z;
// 	  norm_2 = d2.x*d2.x + d2.y*d2.y + d2.z*d2.z;
						
// 	  angle = acos(s_prod/( sqrt(norm_1*norm_2) ));
// 	  //cout << angle / M_PI * 180 << " ";
// 	  angles(i,j) = angle;
// 	  angles(j,i) = angle;
// 	}
//       }
//     }
//     _angles = angles;

//     /* Initialize random number generator */
//     srand48((long) 29869);
	
//     switch(this->_algo) {
      
//     case PROBA:
//       this->_tract.initialize(this->_numFibers*_molecules,1,this->_dt);
			
//       this->_tract.setAttrDesc(0,std::string("speed"));
			
//       _proba_track();
			
//       break;
//     default:
//       throw CImgException("FibersTrackerHARDI<T>::track() 
// - Please specify a valid tracking method");
//     }
		
//     cout << "Saving Connectivity Index ... \t";
//     _ntimes_touched->Save(_tractogram_f);
//     cout << "Done\n";
//   }


	
//   template<typename T>
//   void FibersTrackerHARDI<T>::SetTractogramName(const std::string & f) {
//     _tractogram_f = f;
//   }


//   template<typename T>
//   void FibersTrackerHARDI<T>::SetExponent(const int & exp) {
//     _exponent = exp;
//   }
	
//   template<typename T>
//   void FibersTrackerHARDI<T>::SetStiffness(const int & stiff) {
//     _stiffness = stiff;
//   }


//   template<typename T>
//   void FibersTrackerHARDI<T>::SetMolecules(const int & molecules) {
//     _molecules = molecules;
//     cout << _molecules << " molecules travelling in white matter...\n";
//   }

//   template<typename T>
//   void FibersTrackerHARDI<T>::SetNumberOfJumps(const int & jumps) {
//     _njumps = jumps;
//     cout << _njumps << " jumps per molecule...\n";
//   }


  /* 
     clear all fibers that have a node with connectivity  
     index less than value
  */
//   template<typename T>
//   void FibersTrackerHARDI<T>::ClearMaskFibers(const T & value) {
//     std::cout << "Clearing fibers with connectivity score less than " 
// 	      << value << "\n";
				
//     unsigned int clearedFibers = 0;
//     unsigned int fi, nfibers = this->_tract.getNumFibers();
		
//     for (fi=0; fi<nfibers; fi++) {
//       cout << (fi+1)/nfibers << "%\t\t\r";
//       std::cout.flush();
			
//       Fiber<T> f = this->_tract[fi];
//       int num = f.getNumFnodes();
//       T score = 0.0;
			
//       for(int i = 0; i < num; i++) {
// 	Vertex3<T> v = f.getFnode(i).getPos();
// 	score += (*_ntimes_touched)((int)v.X(), (int)v.Y(), (int)v.Z());
//       }
//       if(num != 0)
// 	score /= num;
//       else
// 	score = 0;
			
//       if(score < value) {
// 	this->_tract[fi].clear();
// 	clearedFibers++;
//       }				
//     }
		
//     std::cout << clearedFibers << " fibers have been cleared" << std::endl;
//   }

//   template<typename T>
//   void FibersTrackerHARDI<T>::ClearFibersNotReaching(const T & value) {
//     std::cout << "Clearing fibers not reaching connectivity index " 
// 	      << value << "\n";
				
//     unsigned int clearedFibers = 0;
//     unsigned int fi, nfibers = this->_tract.getNumFibers();
		
//     for (fi=0; fi<nfibers; fi++) {
//       cout << (fi+1)/nfibers << "%\t\t\r";
//       std::cout.flush();
			
//       Fiber<T> f = this->_tract[fi];
//       int num = f.getNumFnodes();
//       //cout << num << " nodes in the fiber...\n";
//       Vertex3<T> v;
			
//       if(num > 0) {
// 	v = f.getFnode(num-1).getPos();
				
// 	if((*_ntimes_touched)((int)v.X(), (int)v.Y(), (int)v.Z()) < value) {
// 	  this->_tract[fi].clear();
// 	  clearedFibers++;
// 	}
//       }
//     }
		
//     std::cout << clearedFibers << " fibers have been cleared" << std::endl;
//   }
