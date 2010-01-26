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

#ifndef SPHERICALHARMONICS_HPP
#define SPHERICALHARMONICS_HPP

#include <complex>
#include <iostream>

namespace Utils {

  template<typename T> class SphericalHarmonics {

  public:
    SphericalHarmonics();
    SphericalHarmonics(const int& l, const int& m);
    virtual ~SphericalHarmonics();

    void setL(const int& l);
    void setM(const int& m);
    void setLM(const int& l, const int& m);
    int getL();
    int getM();

    std::complex<T> eval(const T& theta, const T& phi);
    std::complex<T> eval_notNorm(const T& theta, const T& phi);
    
    void setVerboseOn();
    
  private :
    int _l, _m;
    bool _verbose;
    T _factorial(const int& n);
  };
}
#endif
