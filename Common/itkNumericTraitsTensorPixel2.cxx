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
#include "itkNumericTraitsTensorPixel2.h"

namespace itk
{
  
  const Tensor<double , 2>  
  NumericTraits<Tensor<double, 2> >::Zero = Tensor<double , 2>( NumericTraits<double >::Zero );
  
  const Tensor<float , 3>  
  NumericTraits<Tensor<float, 3> >::Zero = Tensor<float , 3>( NumericTraits<float >::Zero );
  
    const Tensor<double , 3>
    NumericTraits<Tensor<double, 3> >::Zero = Tensor<double , 3>( NumericTraits<double >::Zero );

    const Tensor<long double , 3>
    NumericTraits<Tensor<long double, 3> >::Zero = Tensor<long double , 3>( NumericTraits<long double >::Zero );
  
  const Tensor<double , 3>  
  NumericTraits<Tensor<double, 3> >::One = Tensor<double , 3>( NumericTraits<double>::One );

  const Tensor<float , 3>  
  NumericTraits<Tensor<float, 3> >::One = Tensor<float , 3>( NumericTraits<float>::One );

  Tensor< float,3 > NumericTraits<Tensor<float, 3> >::ZeroValue() { return Zero; }

  Tensor< double,3 > NumericTraits<Tensor<float, 3> >::min() { return vcl_numeric_limits<double>::min(); }

  Tensor< double,3 > NumericTraits<Tensor<float, 3> >::max() { return vcl_numeric_limits<double>::max(); }
    
    unsigned int NumericTraits<Tensor<float, 3> >::GetLength(Tensor<float, 3> &t) { return 6; }

  Tensor< float,3 > NumericTraits<Tensor<float, 3> >::NonpositiveMin() { 
    return Tensor< float,3 >(-NumericTraits<float>::NonpositiveMin()); }

  bool NumericTraits<Tensor<float, 3> >::IsPositive(Tensor< float,3 > val) { return val.IsPositive(); }

  bool NumericTraits<Tensor<float, 3> >::IsNonpositive(Tensor< float,3 > val) { return !val.IsPositive(); }

  bool NumericTraits<Tensor<float, 3> >::IsNegative(Tensor< float,3 > val) { return val.IsNegative(); }

  bool NumericTraits<Tensor<float, 3> >::IsNonnegative(Tensor< float,3 > val) {return !val.IsNegative(); }

  Tensor< double,3 > NumericTraits<Tensor<double, 3> >::ZeroValue() { return Zero; }

  Tensor< long double,3 > NumericTraits<Tensor<long double, 3> >::ZeroValue() { return Zero; }

  Tensor< double,3 > NumericTraits<Tensor<double, 3> >::min() { return vcl_numeric_limits<double>::min(); }

  Tensor< double,3 > NumericTraits<Tensor<double, 3> >::max() { return vcl_numeric_limits<double>::max(); }

    unsigned int NumericTraits<Tensor<double, 3> >::GetLength(Tensor<double, 3> &t) { return 6; }

    Tensor< double,3 > NumericTraits<Tensor<double, 3> >::NonpositiveMin() {
    return Tensor< double,3 >(-NumericTraits<double>::NonpositiveMin()); }

  bool NumericTraits<Tensor<double, 3> >::IsPositive(Tensor< double,3 > val) { return val.IsPositive(); }

  bool NumericTraits<Tensor<double, 3> >::IsNonpositive(Tensor< double,3 > val) { return !val.IsPositive(); }
  
  bool NumericTraits<Tensor<double, 3> >::IsNegative(Tensor< double,3 > val) { return val.IsNegative(); }
  
  bool NumericTraits<Tensor<double, 3> >::IsNonnegative(Tensor< double,3 > val) {return !val.IsNegative(); }
    
    unsigned int NumericTraits<Tensor<double, 6> >::GetLength(Tensor<double, 6> &t) { return 21; }
  
}
