/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: https://scm.gforge.inria.fr/svn/ttk/trunk/Algorithms/itkVectorImageToTensor.h $
  Language:  C++
  Date:      $Date: 2011-08-02 12:43:00 +0100 (Tue, 02 Aug 2011) $
  Version:   $Revision: 124 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_VectorImageToTensorimage_txx_
#define _itk_VectorImageToTensorimage_txx_

#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIterator.h>

#include "itkVectorImageToTensorImage.h"


namespace itk
{


template <class TInputImage, class TOutputImage>
void
VectorImageToTensorImage<TInputImage,TOutputImage>::
BeforeThreadedGenerateData()
{
    // Preliminary test
    if( InputPixelType::GetVectorDimension() != OutputPixelType::NDegreesOfFreedom )
        throw itk::ExceptionObject (__FILE__,__LINE__,"Error: Tensor and vector dimensions do not match.");

    // Images
    const TInputImage * input  = this->GetInput();
    TOutputImage      * output = this->GetOutput();

    // Copy image geometry from input to output image
    output->SetRegions(   input->GetLargestPossibleRegion() );
    output->SetOrigin(    input->GetOrigin() );
    output->SetSpacing(   input->GetSpacing() );
    output->SetDirection( input->GetDirection() );
}



template <class TInputImage, class TOutputImage>
void
VectorImageToTensorImage<TInputImage,TOutputImage>::
ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, int threadId)
{

    // Define iterators
    typedef ImageRegionConstIterator<InputImageType>  IteratorInputType;
    typedef ImageRegionIterator<OutputImageType>      IteratorOutputType;
    IteratorInputType  itIn (this->GetInput(),  outputRegionForThread);
    IteratorOutputType itOut(this->GetOutput(), outputRegionForThread);

    // Iterate on image voxels
    while( !itOut.IsAtEnd() )
    {

        InputPixelType  V = itIn.Get();
        OutputPixelType T;

        for( unsigned int i=0; i<OutputPixelType::NDegreesOfFreedom; i++)
            T.SetNthComponent( i, static_cast<typename OutputPixelType::ValueType>( V[i] ) );

        itOut.Set(T);

        ++itOut;
        ++itIn;
    }
    
}


} // end of namespace


#endif
