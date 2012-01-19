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
#ifndef _itkLogTensorImageFilter_txx
#define _itkLogTensorImageFilter_txx

#include "itkLogTensorImageFilter.h"

#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>


namespace itk
{

  template <class TInputImage, class TOutputImage>
  void
  LogTensorImageFilter<TInputImage, TOutputImage>
  ::ThreadedGenerateData(const OutputRegionType& outputRegion, int threadId)
  {

    typedef ImageRegionIterator<OutputImageType>       OutputIteratorType;
    typedef ImageRegionConstIterator<InputImageType>  InputIteratorType;
                                      
    InputIteratorType itIn(this->GetInput(), outputRegion);
    OutputIteratorType itOut(this->GetOutput(), outputRegion);
    
    while(!itOut.IsAtEnd() )
    {
      
      InputPixelType T = itIn.Get();
      if( !T.IsZero() )
      {
	try
	{
	  T = T.Log();
	}
	catch( itk::ExceptionObject &)
	{
	  throw itk::ExceptionObject(__FILE__,__LINE__,"Error in LogTensorImageFilter::ThreadedGenerateData()");
	}
      }      
      itOut.Set (T);
      
      ++itOut;
      ++itIn;
    }
    
  }


} // end of namespace itk

#endif
