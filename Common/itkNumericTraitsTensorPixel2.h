/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNumericTraitsTensorPixel2_h
#define __itkNumericTraitsTensorPixel2_h

#include "itkNumericTraits.h"
#include "itkTensor.h"

// This file is meant to define numeric traits for tensor pixels types in itk

namespace itk
{
template <> class NumericTraits<Tensor<unsigned char, 1 > > {
public:
  using ValueType      = unsigned char;
  using PrintType      = Tensor<unsigned char, 1>;
  using AbsType        = Tensor<unsigned char, 1>;
  using AccumulateType = Tensor<unsigned short, 1>;
  using RealType       = Tensor<double, 1>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 1> > {
public:
  using ValueType      = signed char;
  using PrintType      = Tensor<signed char, 1>;
  using AbsType        = Tensor<unsigned char, 1>;
  using AccumulateType = Tensor<short, 1>;
  using RealType       = Tensor<double, 1>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 1> > {
public:
  using ValueType      = char;
  using PrintType      = Tensor<char, 1>;
  using AbsType        = Tensor<unsigned char, 1>;
  using AccumulateType = Tensor<short, 1>;
  using RealType       = Tensor<double, 1>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 1> > {
public:
  using ValueType      = short;
  using PrintType      = Tensor<short, 1>;
  using AbsType        = Tensor<unsigned short, 1>;
  using AccumulateType = Tensor<int, 1>;
  using RealType       = Tensor<double, 1>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 1> > {
public:
  using ValueType      = unsigned short;
  using PrintType      = Tensor<unsigned short, 1>;
  using AbsType        = Tensor<unsigned short, 1>;
  using AccumulateType = Tensor<unsigned int, 1>;
  using RealType       = Tensor<double, 1>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 1> > {
public:
  using ValueType      = int;
  using PrintType      = Tensor<int, 1>;
  using AbsType        = Tensor<unsigned int, 1>;
  using AccumulateType = Tensor<long, 1>;
  using RealType       = Tensor<double, 1>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 1> > {
public:
  using ValueType      = unsigned int;
  using PrintType      = Tensor<unsigned int, 1>;
  using AbsType        = Tensor<unsigned int, 1>;
  using AccumulateType = Tensor<unsigned long, 1>;
  using RealType       = Tensor<double, 1>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 1> > {
public:
  using ValueType      = long;
  using PrintType      = Tensor<long, 1>;
  using AbsType        = Tensor<unsigned long, 1>;
  using AccumulateType = Tensor<long, 1>;
  using RealType       = Tensor<double, 1>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 1> > {
public:
  using ValueType      = unsigned long;
  using PrintType      = Tensor<unsigned long, 1>;
  using AbsType        = Tensor<unsigned long, 1>;
  using AccumulateType = Tensor<unsigned long, 1>;
  using RealType       = Tensor<double, 1>;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 1> > {
public:
  using ValueType      = float;
  using PrintType      = Tensor<float, 1>;
  using AbsType        = Tensor<float, 1>;
  using AccumulateType = Tensor<double, 1>;
  using RealType       = Tensor<double, 1>;
  using ScalarRealType = double;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 1> > {
public:
  using ValueType      = double;
  using PrintType      = Tensor<double, 1>;
  using AbsType        = Tensor<double, 1>;
  using AccumulateType = Tensor<long double, 1>;
  using RealType       = Tensor<double, 1>;
  using ScalarRealType = double;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 1> > {
public:
  using ValueType      = long double;
  using PrintType      = Tensor<long double, 1>;
  using AbsType        = Tensor<long double, 1>;
  using AccumulateType = Tensor<long double, 1>;
  using RealType       = Tensor<long double, 1>;
  using ScalarRealType =long double;
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 2 > > {
public:
  using ValueType      = unsigned char;
  using PrintType      = Tensor<unsigned char, 2>;
  using AbsType        = Tensor<unsigned char, 2>;
  using AccumulateType = Tensor<unsigned short, 2>;
  using RealType       = Tensor<double, 2>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 2> > {
public:
  using ValueType      = signed char;
  using PrintType      = Tensor<signed char, 2>;
  using AbsType        = Tensor<unsigned char, 2>;
  using AccumulateType = Tensor<short, 2>;
  using RealType       = Tensor<double, 2>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 2> > {
public:
  using ValueType      = char;
  using PrintType      = Tensor<char, 2>;
  using AbsType        = Tensor<unsigned char, 2>;
  using AccumulateType = Tensor<short, 2>;
  using RealType       = Tensor<double, 2>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 2> > {
public:
 using ValueType      = short;
 using PrintType      = Tensor<short, 2>;
 using AbsType        = Tensor<unsigned short, 2>;
 using AccumulateType = Tensor<int, 2>;
 using RealType       = Tensor<double, 2>;
 using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 2> > {
public:
  using ValueType      = unsigned short;
  using PrintType      = Tensor<unsigned short, 2>;
  using AbsType        = Tensor<unsigned short, 2>;
  using AccumulateType = Tensor<unsigned int, 2>;
  using RealType       = Tensor<double, 2>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 2> > {
public:
  using ValueType      = int;
  using PrintType      = Tensor<int, 2>;
  using AbsType        = Tensor<unsigned int, 2>;
  using AccumulateType = Tensor<long, 2>;
  using RealType       = Tensor<double, 2>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 2> > {
public:
  using ValueType      = unsigned int;
  using PrintType      = Tensor<unsigned int, 2>;
  using AbsType        = Tensor<unsigned int, 2>;
  using AccumulateType = Tensor<unsigned long, 2>;
  using RealType       = Tensor<double, 2>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 2> > {
public:
  using ValueType      = long;
  using PrintType      = Tensor<long, 2>;
  using AbsType        = Tensor<unsigned long, 2>;
  using AccumulateType = Tensor<long, 2>;
  using RealType       = Tensor<double, 2>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 2> > {
public:
  using ValueType      = unsigned long;
  using PrlongType     = Tensor<unsigned long, 2>;
  using AbsType        = Tensor<unsigned long, 2>;
  using AccumulateType = Tensor<unsigned long, 2>;
  using RealType       = Tensor<double, 2>;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 2> > {
public:
  using ValueType      = float;
  using PrintType      = Tensor<float, 2>;
  using AbsType        = Tensor<float, 2>;
  using AccumulateType = Tensor<double, 2>;
  using RealType       = Tensor<double, 2>;
  using ScalarRealType = double;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 2> > {
public:
  using ValueType      = double;
  using PrintType      = Tensor<double, 2>;
  using AbsType        = Tensor<double, 2>;
  using AccumulateType = Tensor<long double, 2>;
  using RealType       = Tensor<double, 2>;
  using ScalarRealType = double;
  static const Tensor< double,2 > ITK_EXPORT Zero;
  static Tensor< double,2 > ZeroValue() { return Zero; }
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 2> > {
public:
  using ValueType      = long double;
  using PrintType      = Tensor<long double, 2>;
  using AbsType        = Tensor<long double, 2>;
  using AccumulateType = Tensor<long double, 2>;
  using RealType       = Tensor<long double, 2>;
  using ScalarRealType = long double;
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 3 > > {
public:
  using ValueType      = unsigned char;
  using PrintType      = Tensor<unsigned char, 3>;
  using AbsType        = Tensor<unsigned char, 3>;
  using AccumulateType = Tensor<unsigned short, 3>;
  using RealType       = Tensor<double, 3>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 3> > {
public:
  using ValueType      = signed char;
  using PrintType      = Tensor<signed char, 3>;
  using AbsType        = Tensor<unsigned char, 3>;
  using AccumulateType = Tensor<short, 3>;
  using RealType       = Tensor<double, 3>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 3> > {
public:
  using ValueType      = char;
  using PrintType      = Tensor<char, 3>;
  using AbsType        = Tensor<unsigned char, 3>;
  using AccumulateType = Tensor<short, 3>;
  using RealType       = Tensor<double, 3>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 3> > {
public:
  using ValueType      = short;
  using PrintType      = Tensor<short, 3>;
  using AbsType        = Tensor<unsigned short, 3>;
  using AccumulateType = Tensor<int, 3>;
  using RealType       = Tensor<double, 3>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 3> > {
public:
  using ValueType      = unsigned short;
  using PrintType      = Tensor<unsigned short, 3>;
  using AbsType        = Tensor<unsigned short, 3>;
  using AccumulateType = Tensor<unsigned int, 3>;
  using RealType       = Tensor<double, 3>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 3> > {
public:
  using ValueType      = int;
  using PrintType      = Tensor<int, 3>;
  using AbsType        = Tensor<unsigned int, 3>;
  using AccumulateType = Tensor<long, 3>;
  using RealType       = Tensor<double, 3>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 3> > {
public:
  using ValueType      = unsigned int;
  using PrintType      = Tensor<unsigned int, 3>;
  using AbsType        = Tensor<unsigned int, 3>;
  using AccumulateType = Tensor<unsigned long, 3>;
  using RealType       = Tensor<double, 3>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 3> > {
public:
  using ValueType      = long;
  using PrintType      = Tensor<long, 3>;
  using AbsType        = Tensor<unsigned long, 3>;
  using AccumulateType = Tensor<long, 3>;
  using RealType       = Tensor<double, 3>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 3> > {
public:
  using ValueType      = unsigned long;
  using PrlongType     = Tensor<unsigned long, 3>;
  using AbsType        = Tensor<unsigned long, 3>;
  using AccumulateType = Tensor<unsigned long, 3>;
  using RealType       = Tensor<double, 3>;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 3> > {
public:
  using ValueType      = float;
  using PrintType      = Tensor<float, 3>;
  using AbsType        = Tensor<float, 3>;
  using AccumulateType = Tensor<float, 3>;
  using FloatType      = Tensor<float, 3>;
  using RealType       = Tensor<double, 3>;
  static const Tensor< float,3 > ITK_EXPORT Zero;
  static const Tensor< float,3 > ITK_EXPORT One;
  static Tensor< float,3 > ZeroValue();
  static Tensor< double,3 > min();
  static Tensor< double,3 > max();
  static Tensor< float,3 > NonpositiveMin();
  static bool IsPositive(Tensor< float,3 > val);
  static bool IsNonpositive(Tensor< float,3 > val);
  static bool IsNegative(Tensor< float,3 > val);
  static bool IsNonnegative(Tensor< float,3 > val);
  static unsigned int GetLength(Tensor<float, 3> &t);
  
  using ScalarRealType = double ;
  const static bool is_integer=false;
  const static bool is_signed=true;
};
template <> class NumericTraits<Tensor<double, 3> > {
public:
  using ValueType      = double;
  using PrintType      = Tensor<double, 3>;
  using AbsType        = Tensor<double, 3>;
  using AccumulateType = Tensor<long double, 3>;
  using RealType       = Tensor<double, 3>;
  using FloatType			= Tensor<long double, 3>;
  static const Tensor< double,3 > ITK_EXPORT Zero;
  static const Tensor< double,3 > ITK_EXPORT One;
  static Tensor< double,3 > ZeroValue();
  static Tensor< double,3 > min();
  static Tensor< double,3 > max();
  static Tensor< double,3 > NonpositiveMin();
  static bool IsPositive(Tensor< double,3 > val);
  static bool IsNonpositive(Tensor< double,3 > val);
  static bool IsNegative(Tensor< double,3 > val);
  static bool IsNonnegative(Tensor< double,3 > val);
  static unsigned int GetLength(Tensor<double, 3> &t);
  using ScalarRealType = double;
  const static bool is_integer=false;
  const static bool is_signed=true;
};
template <> class NumericTraits<Tensor<long double, 3> > {
public:
  using ValueType      = long double;
  using PrintType      = Tensor<long double, 3>;
  using AbsType        = Tensor<long double, 3>;
  using AccumulateType = Tensor<long double, 3>;
  using RealType       = Tensor<long double, 3>;
  using ScalarRealType =long double;
  static const Tensor< long double,3 > ITK_EXPORT Zero;
  static Tensor< long double,3 > ZeroValue();
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 4 > > {
public:
  using ValueType      = unsigned char;
  using PrintType      = Tensor<unsigned char, 4>;
  using AbsType        = Tensor<unsigned char, 4>;
  using AccumulateType = Tensor<unsigned short, 4>;
  using RealType       = Tensor<double, 4>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 4> > {
public:
  using ValueType      = signed char;
  using PrintType      = Tensor<signed char, 4>;
  using AbsType        = Tensor<unsigned char, 4>;
  using AccumulateType = Tensor<short, 4>;
  using RealType       = Tensor<double, 4>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 4> > {
public:
  using ValueType      = char;
  using PrintType      = Tensor<char, 4>;
  using AbsType        = Tensor<unsigned char, 4>;
  using AccumulateType = Tensor<short, 4>;
  using RealType       = Tensor<double, 4>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 4> > {
public:
  using ValueType      = short;
  using PrintType      = Tensor<short, 4>;
  using AbsType        = Tensor<unsigned short, 4>;
  using AccumulateType = Tensor<int, 4>;
  using RealType       = Tensor<double, 4>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 4> > {
public:
  using ValueType      = unsigned short;
  using PrintType      = Tensor<unsigned short, 4>;
  using AbsType        = Tensor<unsigned short, 4>;
  using AccumulateType = Tensor<unsigned int, 4>;
  using RealType       = Tensor<double, 4>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 4> > {
public:
  using ValueType      = int;
  using PrintType      = Tensor<int, 4>;
  using AbsType        = Tensor<unsigned int, 4>;
  using AccumulateType = Tensor<long, 4>;
  using RealType       = Tensor<double, 4>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 4> > {
public:
  using ValueType      = unsigned int;
  using PrintType      = Tensor<unsigned int, 4>;
  using AbsType        = Tensor<unsigned int, 4>;
  using AccumulateType = Tensor<unsigned long, 4>;
  using RealType       = Tensor<double, 4>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 4> > {
public:
  using ValueType      = long;
  using PrintType      = Tensor<long, 4>;
  using AbsType        = Tensor<unsigned long, 4>;
  using AccumulateType = Tensor<long, 4>;
  using RealType       = Tensor<double, 4>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 4> > {
public:
  using ValueType      = unsigned long;
  using PrlongType     = Tensor<unsigned long, 4>;
  using AbsType        = Tensor<unsigned long, 4>;
  using AccumulateType = Tensor<unsigned long, 4>;
  using RealType       = Tensor<double, 4>;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 4> > {
public:
  using ValueType      = float;
  using PrintType      = Tensor<float, 4>;
  using AbsType        = Tensor<float, 4>;
  using AccumulateType = Tensor<double, 4>;
  using RealType       = Tensor<double, 4>;
  using ScalarRealType = double;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 4> > {
public:
  using ValueType      = double;
  using PrintType      = Tensor<double, 4>;
  using AbsType        = Tensor<double, 4>;
  using AccumulateType = Tensor<long double, 4>;
  using RealType       = Tensor<double, 4>;
  using ScalarRealType = double;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 4> > {
public:
  using ValueType      = long double;
  using PrintType      = Tensor<long double, 4>;
  using AbsType        = Tensor<long double, 4>;
  using AccumulateType = Tensor<long double, 4>;
  using RealType       = Tensor<long double, 4>;
  using ScalarRealType =long double;
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 5 > > {
public:
  using ValueType      = unsigned char;
  using PrintType      = Tensor<unsigned char, 5>;
  using AbsType        = Tensor<unsigned char, 5>;
  using AccumulateType = Tensor<unsigned short, 5>;
  using RealType       = Tensor<double, 5>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 5> > {
public:
  using ValueType      = signed char;
  using PrintType      = Tensor<signed char, 5>;
  using AbsType        = Tensor<unsigned char, 5>;
  using AccumulateType = Tensor<short, 5>;
  using RealType       = Tensor<double, 5>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 5> > {
public:
  using ValueType      = char;
  using PrintType      = Tensor<char, 5>;
  using AbsType        = Tensor<unsigned char, 5>;
  using AccumulateType = Tensor<short, 5>;
  using RealType       = Tensor<double, 5>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 5> > {
public:
  using ValueType      = short;
  using PrintType      = Tensor<short, 5>;
  using AbsType        = Tensor<unsigned short, 5>;
  using AccumulateType = Tensor<int, 5>;
  using RealType       = Tensor<double, 5>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 5> > {
public:
  using ValueType      = unsigned short;
  using PrintType      = Tensor<unsigned short, 5>;
  using AbsType        = Tensor<unsigned short, 5>;
  using AccumulateType = Tensor<unsigned int, 5>;
  using RealType       = Tensor<double, 5>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 5> > {
public:
  using ValueType      = int;
  using PrintType      = Tensor<int, 5>;
  using AbsType        = Tensor<unsigned int, 5>;
  using AccumulateType = Tensor<long, 5>;
  using RealType       = Tensor<double, 5>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 5> > {
public:
  using ValueType      = unsigned int;
  using PrintType      = Tensor<unsigned int, 5>;
  using AbsType        = Tensor<unsigned int, 5>;
  using AccumulateType = Tensor<unsigned long, 5>;
  using RealType       = Tensor<double, 5>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 5> > {
public:
  using ValueType      = long;
  using PrintType      = Tensor<long, 5>;
  using AbsType        = Tensor<unsigned long, 5>;
  using AccumulateType = Tensor<long, 5>;
  using RealType       = Tensor<double, 5>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 5> > {
public:
  using ValueType      = unsigned long;
  using PrlongType     = Tensor<unsigned long, 5>;
  using AbsType        = Tensor<unsigned long, 5>;
  using AccumulateType = Tensor<unsigned long, 5>;
  using RealType       = Tensor<double, 5>;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 5> > {
public:
  using ValueType      = float;
  using PrintType      = Tensor<float, 5>;
  using AbsType        = Tensor<float, 5>;
  using AccumulateType = Tensor<double, 5>;
  using RealType       = Tensor<double, 5>;
  using ScalarRealType = double;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 5> > {
public:
  using ValueType      = double;
  using PrintType      = Tensor<double, 5>;
  using AbsType        = Tensor<double, 5>;
  using AccumulateType = Tensor<long double, 5>;
  using RealType       = Tensor<double, 5>;
  using ScalarRealType = double;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 5> > {
public:
  using ValueType      = long double;
  using PrintType      = Tensor<long double, 5>;
  using AbsType        = Tensor<long double, 5>;
  using AccumulateType = Tensor<long double, 5>;
  using RealType       = Tensor<long double, 5>;
  using ScalarRealType =long double;
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 6 > > {
public:
  using ValueType      = unsigned char;
  using PrintType      = Tensor<unsigned char, 6>;
  using AbsType        = Tensor<unsigned char, 6>;
  using AccumulateType = Tensor<unsigned short, 6>;
  using RealType       = Tensor<double, 6>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 6> > {
public:
  using ValueType      = signed char;
  using PrintType      = Tensor<signed char, 6>;
  using AbsType        = Tensor<unsigned char, 6>;
  using AccumulateType = Tensor<short, 6>;
  using RealType       = Tensor<double, 6>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 6> > {
public:
  using ValueType      = char;
  using PrintType      = Tensor<char, 6>;
  using AbsType        = Tensor<unsigned char, 6>;
  using AccumulateType = Tensor<short, 6>;
  using RealType       = Tensor<double, 6>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 6> > {
public:
  using ValueType      = short;
  using PrintType      = Tensor<short, 6>;
  using AbsType        = Tensor<unsigned short, 6>;
  using AccumulateType = Tensor<int, 6>;
  using RealType       = Tensor<double, 6>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 6> > {
public:
  using ValueType      = unsigned short;
  using PrintType      = Tensor<unsigned short, 6>;
  using AbsType        = Tensor<unsigned short, 6>;
  using AccumulateType = Tensor<unsigned int, 6>;
  using RealType       = Tensor<double, 6>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 6> > {
public:
  using ValueType      = int;
  using PrintType      = Tensor<int, 6>;
  using AbsType        = Tensor<unsigned int, 6>;
  using AccumulateType = Tensor<long, 6>;
  using RealType       = Tensor<double, 6>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 6> > {
public:
  using ValueType      = unsigned int;
  using PrintType      = Tensor<unsigned int, 6>;
  using AbsType        = Tensor<unsigned int, 6>;
  using AccumulateType = Tensor<unsigned long, 6>;
  using RealType       = Tensor<double, 6>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 6> > {
public:
  using ValueType      = long;
  using PrintType      = Tensor<long, 6>;
  using AbsType        = Tensor<unsigned long, 6>;
  using AccumulateType = Tensor<long, 6>;
  using RealType       = Tensor<double, 6>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 6> > {
public:
  using ValueType      = unsigned long;
  using PrlongType     = Tensor<unsigned long, 6>;
  using AbsType        = Tensor<unsigned long, 6>;
  using AccumulateType = Tensor<unsigned long, 6>;
  using RealType       = Tensor<double, 6>;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 6> > {
public:
  using ValueType      = float;
  using PrintType      = Tensor<float, 6>;
  using AbsType        = Tensor<float, 6>;
  using AccumulateType = Tensor<double, 6>;
  using RealType       = Tensor<double, 6>;
  using ScalarRealType = double;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 6> > {
public:
  using ValueType      = double;
  using PrintType      = Tensor<double, 6>;
  using AbsType        = Tensor<double, 6>;
  using AccumulateType = Tensor<long double, 6>;
  using RealType       = Tensor<double, 6>;
  using ScalarRealType = double;
  static unsigned int GetLength(Tensor<double, 6> &t);
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 6> > {
public:
  using ValueType      = long double;
  using PrintType      = Tensor<long double, 6>;
  using AbsType        = Tensor<long double, 6>;
  using AccumulateType = Tensor<long double, 6>;
  using RealType       = Tensor<long double, 6>;
  using ScalarRealType =long double;
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 7 > > {
public:
  using ValueType      = unsigned char;
  using PrintType      = Tensor<unsigned char, 7>;
  using AbsType        = Tensor<unsigned char, 7>;
  using AccumulateType = Tensor<unsigned short, 7>;
  using RealType       = Tensor<double, 7>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 7> > {
public:
  using ValueType      = signed char;
  using PrintType      = Tensor<signed char, 7>;
  using AbsType        = Tensor<unsigned char, 7>;
  using AccumulateType = Tensor<short, 7>;
  using RealType       = Tensor<double, 7>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 7> > {
public:
  using ValueType      = char;
  using PrintType      = Tensor<char, 7>;
  using AbsType        = Tensor<unsigned char, 7>;
  using AccumulateType = Tensor<short, 7>;
  using RealType       = Tensor<double, 7>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 7> > {
public:
  using ValueType      = short;
  using PrintType      = Tensor<short, 7>;
  using AbsType        = Tensor<unsigned short, 7>;
  using AccumulateType = Tensor<int, 7>;
  using RealType       = Tensor<double, 7>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 7> > {
public:
  using ValueType      = unsigned short;
  using PrintType      = Tensor<unsigned short, 7>;
  using AbsType        = Tensor<unsigned short, 7>;
  using AccumulateType = Tensor<unsigned int, 7>;
  using RealType       = Tensor<double, 7>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 7> > {
public:
  using ValueType      = int;
  using PrintType      = Tensor<int, 7>;
  using AbsType        = Tensor<unsigned int, 7>;
  using AccumulateType = Tensor<long, 7>;
  using RealType       = Tensor<double, 7>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 7> > {
public:
  using ValueType      = unsigned int;
  using PrintType      = Tensor<unsigned int, 7>;
  using AbsType        = Tensor<unsigned int, 7>;
  using AccumulateType = Tensor<unsigned long, 7>;
  using RealType       = Tensor<double, 7>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 7> > {
public:
  using ValueType      = long;
  using PrintType      = Tensor<long, 7>;
  using AbsType        = Tensor<unsigned long, 7>;
  using AccumulateType = Tensor<long, 7>;
  using RealType       = Tensor<double, 7>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 7> > {
public:
  using ValueType      = unsigned long;
  using PrlongType     = Tensor<unsigned long, 7>;
  using AbsType        = Tensor<unsigned long, 7>;
  using AccumulateType = Tensor<unsigned long, 7>;
  using RealType       = Tensor<double, 7>;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 7> > {
public:
  using ValueType      = float;
  using PrintType      = Tensor<float, 7>;
  using AbsType        = Tensor<float, 7>;
  using AccumulateType = Tensor<double, 7>;
  using RealType       = Tensor<double, 7>;
  using ScalarRealType = double;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 7> > {
public:
  using ValueType      = double;
  using PrintType      = Tensor<double, 7>;
  using AbsType        = Tensor<double, 7>;
  using AccumulateType = Tensor<long double, 7>;
  using RealType       = Tensor<double, 7>;
  using ScalarRealType = double;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 7> > {
public:
  using ValueType      = long double;
  using PrintType      = Tensor<long double, 7>;
  using AbsType        = Tensor<long double, 7>;
  using AccumulateType = Tensor<long double, 7>;
  using RealType       = Tensor<long double, 7>;
  using ScalarRealType =long double;
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 8 > > {
public:
  using ValueType      = unsigned char;
  using PrintType      = Tensor<unsigned char, 8>;
  using AbsType        = Tensor<unsigned char, 8>;
  using AccumulateType = Tensor<unsigned short, 8>;
  using RealType       = Tensor<double, 8>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 8> > {
public:
  using ValueType      = signed char;
  using PrintType      = Tensor<signed char, 8>;
  using AbsType        = Tensor<unsigned char, 8>;
  using AccumulateType = Tensor<short, 8>;
  using RealType       = Tensor<double, 8>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 8> > {
public:
  using ValueType      = char;
  using PrintType      = Tensor<char, 8>;
  using AbsType        = Tensor<unsigned char, 8>;
  using AccumulateType = Tensor<short, 8>;
  using RealType       = Tensor<double, 8>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 8> > {
public:
  using ValueType      = short;
  using PrintType      = Tensor<short, 8>;
  using AbsType        = Tensor<unsigned short, 8>;
  using AccumulateType = Tensor<int, 8>;
  using RealType       = Tensor<double, 8>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 8> > {
public:
  using ValueType      = unsigned short;
  using PrintType      = Tensor<unsigned short, 8>;
  using AbsType        = Tensor<unsigned short, 8>;
  using AccumulateType = Tensor<unsigned int, 8>;
  using RealType       = Tensor<double, 8>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 8> > {
public:
  using ValueType      = int;
  using PrintType      = Tensor<int, 8>;
  using AbsType        = Tensor<unsigned int, 8>;
  using AccumulateType = Tensor<long, 8>;
  using RealType       = Tensor<double, 8>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 8> > {
public:
  using ValueType      = unsigned int;
  using PrintType      = Tensor<unsigned int, 8>;
  using AbsType        = Tensor<unsigned int, 8>;
  using AccumulateType = Tensor<unsigned long, 8>;
  using RealType       = Tensor<double, 8>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 8> > {
public:
  using ValueType      = long;
  using PrintType      = Tensor<long, 8>;
  using AbsType        = Tensor<unsigned long, 8>;
  using AccumulateType = Tensor<long, 8>;
  using RealType       = Tensor<double, 8>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 8> > {
public:
  using ValueType      = unsigned long;
  using PrlongType     = Tensor<unsigned long, 8>;
  using AbsType        = Tensor<unsigned long, 8>;
  using AccumulateType = Tensor<unsigned long, 8>;
  using RealType       = Tensor<double, 8>;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 8> > {
public:
  using ValueType      = float;
  using PrintType      = Tensor<float, 8>;
  using AbsType        = Tensor<float, 8>;
  using AccumulateType = Tensor<double, 8>;
  using RealType       = Tensor<double, 8>;
  using ScalarRealType = double;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 8> > {
public:
  using ValueType      = double;
  using PrintType      = Tensor<double, 8>;
  using AbsType        = Tensor<double, 8>;
  using AccumulateType = Tensor<long double, 8>;
  using RealType       = Tensor<double, 8>;
  using ScalarRealType = double;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 8> > {
public:
  using ValueType      = long double;
  using PrintType      = Tensor<long double, 8>;
  using AbsType        = Tensor<long double, 8>;
  using AccumulateType = Tensor<long double, 8>;
  using RealType       = Tensor<long double, 8>;
  using ScalarRealType =long double;
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 9 > > {
public:
  using ValueType      = unsigned char;
  using PrintType      = Tensor<unsigned char, 9>;
  using AbsType        = Tensor<unsigned char, 9>;
  using AccumulateType = Tensor<unsigned short, 9>;
  using RealType       = Tensor<double, 9>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 9> > {
public:
  using ValueType      = signed char;
  using PrintType      = Tensor<signed char, 9>;
  using AbsType        = Tensor<unsigned char, 9>;
  using AccumulateType = Tensor<short, 9>;
  using RealType       = Tensor<double, 9>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 9> > {
public:
  using ValueType      = char;
  using PrintType      = Tensor<char, 9>;
  using AbsType        = Tensor<unsigned char, 9>;
  using AccumulateType = Tensor<short, 9>;
  using RealType       = Tensor<double, 9>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 9> > {
public:
  using ValueType      = short;
  using PrintType      = Tensor<short, 9>;
  using AbsType        = Tensor<unsigned short, 9>;
  using AccumulateType = Tensor<int, 9>;
  using RealType       = Tensor<double, 9>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 9> > {
public:
  using ValueType      = unsigned short;
  using PrintType      = Tensor<unsigned short, 9>;
  using AbsType        = Tensor<unsigned short, 9>;
  using AccumulateType = Tensor<unsigned int, 9>;
  using RealType       = Tensor<double, 9>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 9> > {
public:
  using ValueType      = int;
  using PrintType      = Tensor<int, 9>;
  using AbsType        = Tensor<unsigned int, 9>;
  using AccumulateType = Tensor<long, 9>;
  using RealType       = Tensor<double, 9>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 9> > {
public:
  using ValueType      = unsigned int;
  using PrintType      = Tensor<unsigned int, 9>;
  using AbsType        = Tensor<unsigned int, 9>;
  using AccumulateType = Tensor<unsigned long, 9>;
  using RealType       = Tensor<double, 9>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 9> > {
public:
  using ValueType      = long;
  using PrintType      = Tensor<long, 9>;
  using AbsType        = Tensor<unsigned long, 9>;
  using AccumulateType = Tensor<long, 9>;
  using RealType       = Tensor<double, 9>;
  using ScalarRealType = double;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 9> > {
public:
  using ValueType      = unsigned long;
  using PrlongType			= Tensor<unsigned long, 9>;
  using AbsType        = Tensor<unsigned long, 9>;
  using AccumulateType = Tensor<unsigned long, 9>;
  using RealType       = Tensor<double, 9>;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 9> > {
public:
  using ValueType      = float;
  using PrintType      = Tensor<float, 9>;
  using AbsType        = Tensor<float, 9>;
  using AccumulateType = Tensor<double, 9>;
  using RealType       = Tensor<double, 9>;
  using ScalarRealType = double;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 9> > {
public:
  using ValueType      = double;
  using PrintType      = Tensor<double, 9>;
  using AbsType        = Tensor<double, 9>;
  using AccumulateType = Tensor<long double, 9>;
  using RealType       = Tensor<double, 9>;
  using ScalarRealType = double;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 9> > {
public:
  using ValueType      = long double;
  using PrintType      = Tensor<long double, 9>;
  using AbsType        = Tensor<long double, 9>;
  using AccumulateType = Tensor<long double, 9>;
  using RealType       = Tensor<long double, 9>;
  using ScalarRealType =long double;
  const static bool is_integer=false;
};

 
} // end namespace itk

#endif // __itkNumericTraitsTensorPixel2_h  

