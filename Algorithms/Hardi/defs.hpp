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

#ifndef DEFS_HPP
#define DEFS_HPP

#include <vector>

#define M_EPS 1e-9
#define M_COND_CHECK 1e-6

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif
#define ABS(x) (x < 0 ? (-x):x)

namespace Utils {

  struct direction_value { double x,y,z,v; };
  struct direction { double x,y,z; };
  struct coords { unsigned int x,y,z; };
  struct dir_spherical { double theta, phi; };

  template<typename T>
  std::vector<T> normalize(std::vector<T> & v) {
    std::vector<T> norm;

    T max = 0;
    T min = 1000;

    for(unsigned int i = 0; i < v.size(); i++) {
      if(v[i] > max)
	max = v[i];
      if(v[i] < min)
	min = v[i];
    }
		
    for(unsigned int i = 0; i < v.size(); i++) {
      if((max-min) != 0)
	norm.push_back((v[i]-min)/(max-min));
      else 
	norm.push_back(0);
    }
 
    return norm;
  }
  
  template<typename T>
  std::vector<T> normalize_max(std::vector<T> & v) {
    std::vector<T> norm;
    T max = 0;

    for(unsigned int i = 0; i < v.size(); i++) {
      if(v[i] > max)
	max = v[i];
    }
		
    for(unsigned int i = 0; i < v.size(); i++) {
      if((max) != 0)
	norm.push_back(v[i]/max);
    }

    return norm;
  }

}
#endif //!DEFS_HPP
