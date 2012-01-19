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
#ifndef _itk_TensorImageToVectorImage_txx_
#define _itk_TensorImageToVectorImage_txx_

#include "itkTensorImageToVectorImage.h"

#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIterator.h>


namespace itk
{


  template <class TInputImage, class TOutputImage>
  void
  TensorImageToVectorImage<TInputImage,TOutputImage>
  ::BeforeThreadedGenerateData()
  {

    if( InputPixelType::NDegreesOfFreedom != OutputPixelType::GetVectorDimension() )
    {
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error: Tensor and vector dimensions do not match.");
    }    
  }



  template <class TInputImage, class TOutputImage>
  void
  TensorImageToVectorImage<TInputImage,TOutputImage>
  ::ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, int threadId)
  {

    typedef ImageRegionIterator<OutputImageType>      IteratorOutputType;
    typedef ImageRegionConstIterator<InputImageType>  IteratorInputType;


    
    IteratorOutputType itOut(this->GetOutput(), outputRegionForThread);
    IteratorInputType  itIn (this->GetInput(),  outputRegionForThread);

    while( !itOut.IsAtEnd() )
    {

      InputPixelType T = itIn.Get();
      OutputPixelType V;

      for( unsigned int i=0; i<OutputPixelType::GetVectorDimension(); i++)
      {
        V[i] = static_cast<typename OutputPixelType::ValueType>( T.GetNthComponentAsVector (i) );
      }

      itOut.Set (V);
      
      ++itOut;
      ++itIn;
    }
    
    
  }
  
  
  


} // end of namespace


#endif
