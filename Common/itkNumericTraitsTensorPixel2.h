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
  typedef unsigned char ValueType;
  typedef Tensor<unsigned char, 1> PrintType;
  typedef Tensor<unsigned char, 1> AbsType;
  typedef Tensor<unsigned short, 1> AccumulateType;
  typedef Tensor<double, 1> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 1> > {
public:
  typedef signed char ValueType;
  typedef Tensor<signed char, 1> PrintType;
  typedef Tensor<unsigned char, 1> AbsType;
  typedef Tensor<short, 1> AccumulateType;
  typedef Tensor<double, 1> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 1> > {
public:
  typedef char ValueType;
  typedef Tensor<char, 1> PrintType;
  typedef Tensor<unsigned char, 1> AbsType;
  typedef Tensor<short, 1> AccumulateType;
  typedef Tensor<double, 1> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 1> > {
public:
  typedef short ValueType;
  typedef Tensor<short, 1> PrintType;
  typedef Tensor<unsigned short, 1> AbsType;
  typedef Tensor<int, 1> AccumulateType;
  typedef Tensor<double, 1> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 1> > {
public:
  typedef unsigned short ValueType;
  typedef Tensor<unsigned short, 1> PrintType;
  typedef Tensor<unsigned short, 1> AbsType;
  typedef Tensor<unsigned int, 1> AccumulateType;
  typedef Tensor<double, 1> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 1> > {
public:
  typedef int ValueType;
  typedef Tensor<int, 1> PrintType;
  typedef Tensor<unsigned int, 1> AbsType;
  typedef Tensor<long, 1> AccumulateType;
  typedef Tensor<double, 1> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 1> > {
public:
  typedef unsigned int ValueType;
  typedef Tensor<unsigned int, 1> PrintType;
  typedef Tensor<unsigned int, 1> AbsType;
  typedef Tensor<unsigned long, 1> AccumulateType;
  typedef Tensor<double, 1> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 1> > {
public:
  typedef long ValueType;
  typedef Tensor<long, 1> PrintType;
  typedef Tensor<unsigned long, 1> AbsType;
  typedef Tensor<long, 1> AccumulateType;
  typedef Tensor<double, 1> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 1> > {
public:
  typedef unsigned long ValueType;
  typedef Tensor<unsigned long, 1> PrlongType;
  typedef Tensor<unsigned long, 1> AbsType;
  typedef Tensor<unsigned long, 1> AccumulateType;
  typedef Tensor<double, 1> RealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 1> > {
public:
  typedef float ValueType;
  typedef Tensor<float, 1> PrintType;
  typedef Tensor<float, 1> AbsType;
  typedef Tensor<double, 1> AccumulateType;
  typedef Tensor<double, 1> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 1> > {
public:
  typedef double ValueType;
  typedef Tensor<double, 1> PrintType;
  typedef Tensor<double, 1> AbsType;
  typedef Tensor<long double, 1> AccumulateType;
  typedef Tensor<double, 1> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 1> > {
public:
  typedef long double ValueType;
  typedef Tensor<long double, 1> PrintType;
  typedef Tensor<long double, 1> AbsType;
  typedef Tensor<long double, 1> AccumulateType;
  typedef Tensor<long double, 1> RealType;
  typedef long double ScalarRealType;
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 2 > > {
public:
  typedef unsigned char ValueType;
  typedef Tensor<unsigned char, 2> PrintType;
  typedef Tensor<unsigned char, 2> AbsType;
  typedef Tensor<unsigned short, 2> AccumulateType;
  typedef Tensor<double, 2> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 2> > {
public:
  typedef signed char ValueType;
  typedef Tensor<signed char, 2> PrintType;
  typedef Tensor<unsigned char, 2> AbsType;
  typedef Tensor<short, 2> AccumulateType;
  typedef Tensor<double, 2> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 2> > {
public:
  typedef char ValueType;
  typedef Tensor<char, 2> PrintType;
  typedef Tensor<unsigned char, 2> AbsType;
  typedef Tensor<short, 2> AccumulateType;
  typedef Tensor<double, 2> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 2> > {
public:
  typedef short ValueType;
  typedef Tensor<short, 2> PrintType;
  typedef Tensor<unsigned short, 2> AbsType;
  typedef Tensor<int, 2> AccumulateType;
  typedef Tensor<double, 2> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 2> > {
public:
  typedef unsigned short ValueType;
  typedef Tensor<unsigned short, 2> PrintType;
  typedef Tensor<unsigned short, 2> AbsType;
  typedef Tensor<unsigned int, 2> AccumulateType;
  typedef Tensor<double, 2> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 2> > {
public:
  typedef int ValueType;
  typedef Tensor<int, 2> PrintType;
  typedef Tensor<unsigned int, 2> AbsType;
  typedef Tensor<long, 2> AccumulateType;
  typedef Tensor<double, 2> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 2> > {
public:
  typedef unsigned int ValueType;
  typedef Tensor<unsigned int, 2> PrintType;
  typedef Tensor<unsigned int, 2> AbsType;
  typedef Tensor<unsigned long, 2> AccumulateType;
  typedef Tensor<double, 2> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 2> > {
public:
  typedef long ValueType;
  typedef Tensor<long, 2> PrintType;
  typedef Tensor<unsigned long, 2> AbsType;
  typedef Tensor<long, 2> AccumulateType;
  typedef Tensor<double, 2> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 2> > {
public:
  typedef unsigned long ValueType;
  typedef Tensor<unsigned long, 2> PrlongType;
  typedef Tensor<unsigned long, 2> AbsType;
  typedef Tensor<unsigned long, 2> AccumulateType;
  typedef Tensor<double, 2> RealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 2> > {
public:
  typedef float ValueType;
  typedef Tensor<float, 2> PrintType;
  typedef Tensor<float, 2> AbsType;
  typedef Tensor<double, 2> AccumulateType;
  typedef Tensor<double, 2> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 2> > {
public:
  typedef double ValueType;
  typedef Tensor<double, 2> PrintType;
  typedef Tensor<double, 2> AbsType;
  typedef Tensor<long double, 2> AccumulateType;
  typedef Tensor<double, 2> RealType;
  typedef double ScalarRealType;
  static const Tensor< double,2 > ITK_EXPORT Zero;
  static Tensor< double,2 > ZeroValue() { return Zero; }
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 2> > {
public:
  typedef long double ValueType;
  typedef Tensor<long double, 2> PrintType;
  typedef Tensor<long double, 2> AbsType;
  typedef Tensor<long double, 2> AccumulateType;
  typedef Tensor<long double, 2> RealType;
  typedef long double ScalarRealType;
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 3 > > {
public:
  typedef unsigned char ValueType;
  typedef Tensor<unsigned char, 3> PrintType;
  typedef Tensor<unsigned char, 3> AbsType;
  typedef Tensor<unsigned short, 3> AccumulateType;
  typedef Tensor<double, 3> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 3> > {
public:
  typedef signed char ValueType;
  typedef Tensor<signed char, 3> PrintType;
  typedef Tensor<unsigned char, 3> AbsType;
  typedef Tensor<short, 3> AccumulateType;
  typedef Tensor<double, 3> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 3> > {
public:
  typedef char ValueType;
  typedef Tensor<char, 3> PrintType;
  typedef Tensor<unsigned char, 3> AbsType;
  typedef Tensor<short, 3> AccumulateType;
  typedef Tensor<double, 3> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 3> > {
public:
  typedef short ValueType;
  typedef Tensor<short, 3> PrintType;
  typedef Tensor<unsigned short, 3> AbsType;
  typedef Tensor<int, 3> AccumulateType;
  typedef Tensor<double, 3> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 3> > {
public:
  typedef unsigned short ValueType;
  typedef Tensor<unsigned short, 3> PrintType;
  typedef Tensor<unsigned short, 3> AbsType;
  typedef Tensor<unsigned int, 3> AccumulateType;
  typedef Tensor<double, 3> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 3> > {
public:
  typedef int ValueType;
  typedef Tensor<int, 3> PrintType;
  typedef Tensor<unsigned int, 3> AbsType;
  typedef Tensor<long, 3> AccumulateType;
  typedef Tensor<double, 3> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 3> > {
public:
  typedef unsigned int ValueType;
  typedef Tensor<unsigned int, 3> PrintType;
  typedef Tensor<unsigned int, 3> AbsType;
  typedef Tensor<unsigned long, 3> AccumulateType;
  typedef Tensor<double, 3> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 3> > {
public:
  typedef long ValueType;
  typedef Tensor<long, 3> PrintType;
  typedef Tensor<unsigned long, 3> AbsType;
  typedef Tensor<long, 3> AccumulateType;
  typedef Tensor<double, 3> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 3> > {
public:
  typedef unsigned long ValueType;
  typedef Tensor<unsigned long, 3> PrlongType;
  typedef Tensor<unsigned long, 3> AbsType;
  typedef Tensor<unsigned long, 3> AccumulateType;
  typedef Tensor<double, 3> RealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 3> > {
public:
  typedef float ValueType;
  typedef Tensor<float, 3> PrintType;
  typedef Tensor<float, 3> AbsType;
  typedef Tensor<float, 3> AccumulateType;
  typedef Tensor<float, 3> FloatType;
  typedef Tensor<double, 3> RealType;
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
  
  typedef double  ScalarRealType;
  const static bool is_integer=false;
  const static bool is_signed=true;
};
template <> class NumericTraits<Tensor<double, 3> > {
public:
  typedef double ValueType;
  typedef Tensor<double, 3> PrintType;
  typedef Tensor<double, 3> AbsType;
  typedef Tensor<long double, 3> AccumulateType;
  typedef Tensor<double, 3> RealType;
  typedef Tensor<long double, 3> FloatType;
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
  typedef double ScalarRealType;
  const static bool is_integer=false;
  const static bool is_signed=true;
};
template <> class NumericTraits<Tensor<long double, 3> > {
public:
  typedef long double ValueType;
  typedef Tensor<long double, 3> PrintType;
  typedef Tensor<long double, 3> AbsType;
  typedef Tensor<long double, 3> AccumulateType;
  typedef Tensor<long double, 3> RealType;
  typedef long double ScalarRealType;
  static const Tensor< long double,3 > ITK_EXPORT Zero;
  static Tensor< long double,3 > ZeroValue();
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 4 > > {
public:
  typedef unsigned char ValueType;
  typedef Tensor<unsigned char, 4> PrintType;
  typedef Tensor<unsigned char, 4> AbsType;
  typedef Tensor<unsigned short, 4> AccumulateType;
  typedef Tensor<double, 4> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 4> > {
public:
  typedef signed char ValueType;
  typedef Tensor<signed char, 4> PrintType;
  typedef Tensor<unsigned char, 4> AbsType;
  typedef Tensor<short, 4> AccumulateType;
  typedef Tensor<double, 4> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 4> > {
public:
  typedef char ValueType;
  typedef Tensor<char, 4> PrintType;
  typedef Tensor<unsigned char, 4> AbsType;
  typedef Tensor<short, 4> AccumulateType;
  typedef Tensor<double, 4> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 4> > {
public:
  typedef short ValueType;
  typedef Tensor<short, 4> PrintType;
  typedef Tensor<unsigned short, 4> AbsType;
  typedef Tensor<int, 4> AccumulateType;
  typedef Tensor<double, 4> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 4> > {
public:
  typedef unsigned short ValueType;
  typedef Tensor<unsigned short, 4> PrintType;
  typedef Tensor<unsigned short, 4> AbsType;
  typedef Tensor<unsigned int, 4> AccumulateType;
  typedef Tensor<double, 4> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 4> > {
public:
  typedef int ValueType;
  typedef Tensor<int, 4> PrintType;
  typedef Tensor<unsigned int, 4> AbsType;
  typedef Tensor<long, 4> AccumulateType;
  typedef Tensor<double, 4> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 4> > {
public:
  typedef unsigned int ValueType;
  typedef Tensor<unsigned int, 4> PrintType;
  typedef Tensor<unsigned int, 4> AbsType;
  typedef Tensor<unsigned long, 4> AccumulateType;
  typedef Tensor<double, 4> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 4> > {
public:
  typedef long ValueType;
  typedef Tensor<long, 4> PrintType;
  typedef Tensor<unsigned long, 4> AbsType;
  typedef Tensor<long, 4> AccumulateType;
  typedef Tensor<double, 4> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 4> > {
public:
  typedef unsigned long ValueType;
  typedef Tensor<unsigned long, 4> PrlongType;
  typedef Tensor<unsigned long, 4> AbsType;
  typedef Tensor<unsigned long, 4> AccumulateType;
  typedef Tensor<double, 4> RealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 4> > {
public:
  typedef float ValueType;
  typedef Tensor<float, 4> PrintType;
  typedef Tensor<float, 4> AbsType;
  typedef Tensor<double, 4> AccumulateType;
  typedef Tensor<double, 4> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 4> > {
public:
  typedef double ValueType;
  typedef Tensor<double, 4> PrintType;
  typedef Tensor<double, 4> AbsType;
  typedef Tensor<long double, 4> AccumulateType;
  typedef Tensor<double, 4> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 4> > {
public:
  typedef long double ValueType;
  typedef Tensor<long double, 4> PrintType;
  typedef Tensor<long double, 4> AbsType;
  typedef Tensor<long double, 4> AccumulateType;
  typedef Tensor<long double, 4> RealType;
  typedef long double ScalarRealType;
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 5 > > {
public:
  typedef unsigned char ValueType;
  typedef Tensor<unsigned char, 5> PrintType;
  typedef Tensor<unsigned char, 5> AbsType;
  typedef Tensor<unsigned short, 5> AccumulateType;
  typedef Tensor<double, 5> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 5> > {
public:
  typedef signed char ValueType;
  typedef Tensor<signed char, 5> PrintType;
  typedef Tensor<unsigned char, 5> AbsType;
  typedef Tensor<short, 5> AccumulateType;
  typedef Tensor<double, 5> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 5> > {
public:
  typedef char ValueType;
  typedef Tensor<char, 5> PrintType;
  typedef Tensor<unsigned char, 5> AbsType;
  typedef Tensor<short, 5> AccumulateType;
  typedef Tensor<double, 5> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 5> > {
public:
  typedef short ValueType;
  typedef Tensor<short, 5> PrintType;
  typedef Tensor<unsigned short, 5> AbsType;
  typedef Tensor<int, 5> AccumulateType;
  typedef Tensor<double, 5> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 5> > {
public:
  typedef unsigned short ValueType;
  typedef Tensor<unsigned short, 5> PrintType;
  typedef Tensor<unsigned short, 5> AbsType;
  typedef Tensor<unsigned int, 5> AccumulateType;
  typedef Tensor<double, 5> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 5> > {
public:
  typedef int ValueType;
  typedef Tensor<int, 5> PrintType;
  typedef Tensor<unsigned int, 5> AbsType;
  typedef Tensor<long, 5> AccumulateType;
  typedef Tensor<double, 5> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 5> > {
public:
  typedef unsigned int ValueType;
  typedef Tensor<unsigned int, 5> PrintType;
  typedef Tensor<unsigned int, 5> AbsType;
  typedef Tensor<unsigned long, 5> AccumulateType;
  typedef Tensor<double, 5> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 5> > {
public:
  typedef long ValueType;
  typedef Tensor<long, 5> PrintType;
  typedef Tensor<unsigned long, 5> AbsType;
  typedef Tensor<long, 5> AccumulateType;
  typedef Tensor<double, 5> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 5> > {
public:
  typedef unsigned long ValueType;
  typedef Tensor<unsigned long, 5> PrlongType;
  typedef Tensor<unsigned long, 5> AbsType;
  typedef Tensor<unsigned long, 5> AccumulateType;
  typedef Tensor<double, 5> RealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 5> > {
public:
  typedef float ValueType;
  typedef Tensor<float, 5> PrintType;
  typedef Tensor<float, 5> AbsType;
  typedef Tensor<double, 5> AccumulateType;
  typedef Tensor<double, 5> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 5> > {
public:
  typedef double ValueType;
  typedef Tensor<double, 5> PrintType;
  typedef Tensor<double, 5> AbsType;
  typedef Tensor<long double, 5> AccumulateType;
  typedef Tensor<double, 5> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 5> > {
public:
  typedef long double ValueType;
  typedef Tensor<long double, 5> PrintType;
  typedef Tensor<long double, 5> AbsType;
  typedef Tensor<long double, 5> AccumulateType;
  typedef Tensor<long double, 5> RealType;
  typedef long double ScalarRealType;
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 6 > > {
public:
  typedef unsigned char ValueType;
  typedef Tensor<unsigned char, 6> PrintType;
  typedef Tensor<unsigned char, 6> AbsType;
  typedef Tensor<unsigned short, 6> AccumulateType;
  typedef Tensor<double, 6> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 6> > {
public:
  typedef signed char ValueType;
  typedef Tensor<signed char, 6> PrintType;
  typedef Tensor<unsigned char, 6> AbsType;
  typedef Tensor<short, 6> AccumulateType;
  typedef Tensor<double, 6> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 6> > {
public:
  typedef char ValueType;
  typedef Tensor<char, 6> PrintType;
  typedef Tensor<unsigned char, 6> AbsType;
  typedef Tensor<short, 6> AccumulateType;
  typedef Tensor<double, 6> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 6> > {
public:
  typedef short ValueType;
  typedef Tensor<short, 6> PrintType;
  typedef Tensor<unsigned short, 6> AbsType;
  typedef Tensor<int, 6> AccumulateType;
  typedef Tensor<double, 6> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 6> > {
public:
  typedef unsigned short ValueType;
  typedef Tensor<unsigned short, 6> PrintType;
  typedef Tensor<unsigned short, 6> AbsType;
  typedef Tensor<unsigned int, 6> AccumulateType;
  typedef Tensor<double, 6> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 6> > {
public:
  typedef int ValueType;
  typedef Tensor<int, 6> PrintType;
  typedef Tensor<unsigned int, 6> AbsType;
  typedef Tensor<long, 6> AccumulateType;
  typedef Tensor<double, 6> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 6> > {
public:
  typedef unsigned int ValueType;
  typedef Tensor<unsigned int, 6> PrintType;
  typedef Tensor<unsigned int, 6> AbsType;
  typedef Tensor<unsigned long, 6> AccumulateType;
  typedef Tensor<double, 6> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 6> > {
public:
  typedef long ValueType;
  typedef Tensor<long, 6> PrintType;
  typedef Tensor<unsigned long, 6> AbsType;
  typedef Tensor<long, 6> AccumulateType;
  typedef Tensor<double, 6> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 6> > {
public:
  typedef unsigned long ValueType;
  typedef Tensor<unsigned long, 6> PrlongType;
  typedef Tensor<unsigned long, 6> AbsType;
  typedef Tensor<unsigned long, 6> AccumulateType;
  typedef Tensor<double, 6> RealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 6> > {
public:
  typedef float ValueType;
  typedef Tensor<float, 6> PrintType;
  typedef Tensor<float, 6> AbsType;
  typedef Tensor<double, 6> AccumulateType;
  typedef Tensor<double, 6> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 6> > {
public:
  typedef double ValueType;
  typedef Tensor<double, 6> PrintType;
  typedef Tensor<double, 6> AbsType;
  typedef Tensor<long double, 6> AccumulateType;
  typedef Tensor<double, 6> RealType;
  typedef double ScalarRealType;
  static unsigned int GetLength(Tensor<double, 6> &t);
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 6> > {
public:
  typedef long double ValueType;
  typedef Tensor<long double, 6> PrintType;
  typedef Tensor<long double, 6> AbsType;
  typedef Tensor<long double, 6> AccumulateType;
  typedef Tensor<long double, 6> RealType;
  typedef long double ScalarRealType;
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 7 > > {
public:
  typedef unsigned char ValueType;
  typedef Tensor<unsigned char, 7> PrintType;
  typedef Tensor<unsigned char, 7> AbsType;
  typedef Tensor<unsigned short, 7> AccumulateType;
  typedef Tensor<double, 7> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 7> > {
public:
  typedef signed char ValueType;
  typedef Tensor<signed char, 7> PrintType;
  typedef Tensor<unsigned char, 7> AbsType;
  typedef Tensor<short, 7> AccumulateType;
  typedef Tensor<double, 7> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 7> > {
public:
  typedef char ValueType;
  typedef Tensor<char, 7> PrintType;
  typedef Tensor<unsigned char, 7> AbsType;
  typedef Tensor<short, 7> AccumulateType;
  typedef Tensor<double, 7> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 7> > {
public:
  typedef short ValueType;
  typedef Tensor<short, 7> PrintType;
  typedef Tensor<unsigned short, 7> AbsType;
  typedef Tensor<int, 7> AccumulateType;
  typedef Tensor<double, 7> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 7> > {
public:
  typedef unsigned short ValueType;
  typedef Tensor<unsigned short, 7> PrintType;
  typedef Tensor<unsigned short, 7> AbsType;
  typedef Tensor<unsigned int, 7> AccumulateType;
  typedef Tensor<double, 7> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 7> > {
public:
  typedef int ValueType;
  typedef Tensor<int, 7> PrintType;
  typedef Tensor<unsigned int, 7> AbsType;
  typedef Tensor<long, 7> AccumulateType;
  typedef Tensor<double, 7> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 7> > {
public:
  typedef unsigned int ValueType;
  typedef Tensor<unsigned int, 7> PrintType;
  typedef Tensor<unsigned int, 7> AbsType;
  typedef Tensor<unsigned long, 7> AccumulateType;
  typedef Tensor<double, 7> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 7> > {
public:
  typedef long ValueType;
  typedef Tensor<long, 7> PrintType;
  typedef Tensor<unsigned long, 7> AbsType;
  typedef Tensor<long, 7> AccumulateType;
  typedef Tensor<double, 7> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 7> > {
public:
  typedef unsigned long ValueType;
  typedef Tensor<unsigned long, 7> PrlongType;
  typedef Tensor<unsigned long, 7> AbsType;
  typedef Tensor<unsigned long, 7> AccumulateType;
  typedef Tensor<double, 7> RealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 7> > {
public:
  typedef float ValueType;
  typedef Tensor<float, 7> PrintType;
  typedef Tensor<float, 7> AbsType;
  typedef Tensor<double, 7> AccumulateType;
  typedef Tensor<double, 7> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 7> > {
public:
  typedef double ValueType;
  typedef Tensor<double, 7> PrintType;
  typedef Tensor<double, 7> AbsType;
  typedef Tensor<long double, 7> AccumulateType;
  typedef Tensor<double, 7> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 7> > {
public:
  typedef long double ValueType;
  typedef Tensor<long double, 7> PrintType;
  typedef Tensor<long double, 7> AbsType;
  typedef Tensor<long double, 7> AccumulateType;
  typedef Tensor<long double, 7> RealType;
  typedef long double ScalarRealType;
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 8 > > {
public:
  typedef unsigned char ValueType;
  typedef Tensor<unsigned char, 8> PrintType;
  typedef Tensor<unsigned char, 8> AbsType;
  typedef Tensor<unsigned short, 8> AccumulateType;
  typedef Tensor<double, 8> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 8> > {
public:
  typedef signed char ValueType;
  typedef Tensor<signed char, 8> PrintType;
  typedef Tensor<unsigned char, 8> AbsType;
  typedef Tensor<short, 8> AccumulateType;
  typedef Tensor<double, 8> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 8> > {
public:
  typedef char ValueType;
  typedef Tensor<char, 8> PrintType;
  typedef Tensor<unsigned char, 8> AbsType;
  typedef Tensor<short, 8> AccumulateType;
  typedef Tensor<double, 8> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 8> > {
public:
  typedef short ValueType;
  typedef Tensor<short, 8> PrintType;
  typedef Tensor<unsigned short, 8> AbsType;
  typedef Tensor<int, 8> AccumulateType;
  typedef Tensor<double, 8> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 8> > {
public:
  typedef unsigned short ValueType;
  typedef Tensor<unsigned short, 8> PrintType;
  typedef Tensor<unsigned short, 8> AbsType;
  typedef Tensor<unsigned int, 8> AccumulateType;
  typedef Tensor<double, 8> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 8> > {
public:
  typedef int ValueType;
  typedef Tensor<int, 8> PrintType;
  typedef Tensor<unsigned int, 8> AbsType;
  typedef Tensor<long, 8> AccumulateType;
  typedef Tensor<double, 8> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 8> > {
public:
  typedef unsigned int ValueType;
  typedef Tensor<unsigned int, 8> PrintType;
  typedef Tensor<unsigned int, 8> AbsType;
  typedef Tensor<unsigned long, 8> AccumulateType;
  typedef Tensor<double, 8> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 8> > {
public:
  typedef long ValueType;
  typedef Tensor<long, 8> PrintType;
  typedef Tensor<unsigned long, 8> AbsType;
  typedef Tensor<long, 8> AccumulateType;
  typedef Tensor<double, 8> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 8> > {
public:
  typedef unsigned long ValueType;
  typedef Tensor<unsigned long, 8> PrlongType;
  typedef Tensor<unsigned long, 8> AbsType;
  typedef Tensor<unsigned long, 8> AccumulateType;
  typedef Tensor<double, 8> RealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 8> > {
public:
  typedef float ValueType;
  typedef Tensor<float, 8> PrintType;
  typedef Tensor<float, 8> AbsType;
  typedef Tensor<double, 8> AccumulateType;
  typedef Tensor<double, 8> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 8> > {
public:
  typedef double ValueType;
  typedef Tensor<double, 8> PrintType;
  typedef Tensor<double, 8> AbsType;
  typedef Tensor<long double, 8> AccumulateType;
  typedef Tensor<double, 8> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 8> > {
public:
  typedef long double ValueType;
  typedef Tensor<long double, 8> PrintType;
  typedef Tensor<long double, 8> AbsType;
  typedef Tensor<long double, 8> AccumulateType;
  typedef Tensor<long double, 8> RealType;
  typedef long double ScalarRealType;
  const static bool is_integer=false;
};

template <> class NumericTraits<Tensor<unsigned char, 9 > > {
public:
  typedef unsigned char ValueType;
  typedef Tensor<unsigned char, 9> PrintType;
  typedef Tensor<unsigned char, 9> AbsType;
  typedef Tensor<unsigned short, 9> AccumulateType;
  typedef Tensor<double, 9> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<signed char, 9> > {
public:
  typedef signed char ValueType;
  typedef Tensor<signed char, 9> PrintType;
  typedef Tensor<unsigned char, 9> AbsType;
  typedef Tensor<short, 9> AccumulateType;
  typedef Tensor<double, 9> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<char, 9> > {
public:
  typedef char ValueType;
  typedef Tensor<char, 9> PrintType;
  typedef Tensor<unsigned char, 9> AbsType;
  typedef Tensor<short, 9> AccumulateType;
  typedef Tensor<double, 9> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<short, 9> > {
public:
  typedef short ValueType;
  typedef Tensor<short, 9> PrintType;
  typedef Tensor<unsigned short, 9> AbsType;
  typedef Tensor<int, 9> AccumulateType;
  typedef Tensor<double, 9> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned short, 9> > {
public:
  typedef unsigned short ValueType;
  typedef Tensor<unsigned short, 9> PrintType;
  typedef Tensor<unsigned short, 9> AbsType;
  typedef Tensor<unsigned int, 9> AccumulateType;
  typedef Tensor<double, 9> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<int, 9> > {
public:
  typedef int ValueType;
  typedef Tensor<int, 9> PrintType;
  typedef Tensor<unsigned int, 9> AbsType;
  typedef Tensor<long, 9> AccumulateType;
  typedef Tensor<double, 9> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned int, 9> > {
public:
  typedef unsigned int ValueType;
  typedef Tensor<unsigned int, 9> PrintType;
  typedef Tensor<unsigned int, 9> AbsType;
  typedef Tensor<unsigned long, 9> AccumulateType;
  typedef Tensor<double, 9> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<long, 9> > {
public:
  typedef long ValueType;
  typedef Tensor<long, 9> PrintType;
  typedef Tensor<unsigned long, 9> AbsType;
  typedef Tensor<long, 9> AccumulateType;
  typedef Tensor<double, 9> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<unsigned long, 9> > {
public:
  typedef unsigned long ValueType;
  typedef Tensor<unsigned long, 9> PrlongType;
  typedef Tensor<unsigned long, 9> AbsType;
  typedef Tensor<unsigned long, 9> AccumulateType;
  typedef Tensor<double, 9> RealType;
  const static bool is_integer=true;
};
template <> class NumericTraits<Tensor<float, 9> > {
public:
  typedef float ValueType;
  typedef Tensor<float, 9> PrintType;
  typedef Tensor<float, 9> AbsType;
  typedef Tensor<double, 9> AccumulateType;
  typedef Tensor<double, 9> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<double, 9> > {
public:
  typedef double ValueType;
  typedef Tensor<double, 9> PrintType;
  typedef Tensor<double, 9> AbsType;
  typedef Tensor<long double, 9> AccumulateType;
  typedef Tensor<double, 9> RealType;
  typedef double ScalarRealType;
  const static bool is_integer=false;
};
template <> class NumericTraits<Tensor<long double, 9> > {
public:
  typedef long double ValueType;
  typedef Tensor<long double, 9> PrintType;
  typedef Tensor<long double, 9> AbsType;
  typedef Tensor<long double, 9> AccumulateType;
  typedef Tensor<long double, 9> RealType;
  typedef long double ScalarRealType;
  const static bool is_integer=false;
};

 
} // end namespace itk

#endif // __itkNumericTraitsTensorPixel2_h  

