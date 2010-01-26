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

#include "sphericalHarmonics.hpp"
#include "defs.hpp"

#include <boost/math/special_functions/legendre.hpp>
#include <boost/math/special_functions/spherical_harmonic.hpp>

namespace Utils {

  template<typename T>
  SphericalHarmonics<T>::SphericalHarmonics() { 
    _l = 1; _m = 1; _verbose = false; 
  }
	
  template<typename T>
  SphericalHarmonics<T>::SphericalHarmonics(const int& l, const int& m) { 
    _l = l;  _m = m; _verbose = false;
  }

  template <typename T>
  SphericalHarmonics<T>::~SphericalHarmonics() {}

  template<typename T>
  void SphericalHarmonics<T>::setL(const int& l) { _l = l; }

  template<typename T>
  void SphericalHarmonics<T>::setM(const int& m) { _m = m; }

  template<typename T>
  void SphericalHarmonics<T>::setLM(const int& l, const int& m) { _l = l;  
    _m = m; }

  template<typename T>
  int SphericalHarmonics<T>::getL() { return _l; }

  template<typename T>
  int SphericalHarmonics<T>::getM() { return _m; }
  
  template<typename T>
  void SphericalHarmonics<T>::setVerboseOn() {
    _verbose = true;
  }
  
  template <typename T> T SphericalHarmonics<T>::_factorial(const int& n) {
    T res = 1;
    for(int i=2; i<=n; i++) 
      res *= i;
		
    return res;
  }

  // Physics convention for theta phi
  template<typename T> std::complex<T> 
  SphericalHarmonics<T>::eval(const T& theta, const T& phi) {
    int absm = std::abs(_m);
    T sign;
    
    if (absm%2==1) 
      sign=-1.0;
    else 
      sign=1.0;
    
    std::complex<T> retval(0.0,(T)(absm*phi));
    
    retval = std::exp(retval);
    
    T factor = sign*sqrt(((T)(2*_l+1) / (4.0*M_PI)) * (_factorial(_l - _m) 
						       / _factorial(_l + _m))) *
      boost::math::spherical_harmonic_r (_l,absm,theta,0.0);

    retval = factor*retval;
    
    if (_m<0) { retval = std::conj(retval); retval = sign*retval; }
    return retval;
  }
	
  // Physics convention for theta phi
  // same as above but we use the normal gsl_sf_legendre_Plm
  //
  // I think the normalized version above, multiplies the legendre output
  // by the normalization factor sqrt(((T)(2*_l+1) / (4.0*M_PI))
  // Hence, in the end, we do not get the expected spherical harmonics
  //
  // ex: Y_0^0 should be 1/sqrt(4PI) = 0.2821 but we get with eval above
  // 0.0795775 which is (0.2821)^2
  template<typename T> std::complex<T> 
  SphericalHarmonics<T>::eval_notNorm(const T& theta, const T& phi) {
    int absm = std::abs(_m);
    T sign;
	  
    if (absm%2==1) 
      sign=-1.0;
    else 
      sign=1.0;
	  
    std::complex<T> retval(0.0,(T)(absm*phi));
	  
    retval = std::exp(retval);
	  
	  
    T factor = sqrt(((T)(2*_l+1) / (4.0*M_PI)) * 
		    (_factorial(_l - absm) / _factorial(_l + absm))) *
      boost::math::legendre_p (_l,absm,cos(theta));

    retval = factor*retval;
	  
    if (_m<0) { 
      retval = std::conj(retval); 
      retval = sign*retval; 
    }
	  
    if(_verbose) {
      std::cout << "retval: " << retval << "\n";
    }
    return retval;
  }
  
  template class SphericalHarmonics<float>;
  template class SphericalHarmonics<double>;
}
