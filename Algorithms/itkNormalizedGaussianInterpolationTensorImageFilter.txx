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
#ifndef _itk_NormalizedGaussianInterpolationTensorImageFilter_txx_
#define _itk_NormalizedGaussianInterpolationTensorImageFilter_txx_
#include "itkNormalizedGaussianInterpolationTensorImageFilter.h"

#include <itkImageRegionConstIteratorWithIndex.h>
#include <itkImageRegionIterator.h>
#include <itkLinearInterpolateImageFunction.h>

namespace itk
{


  template< class TInputImage, class TOutputImage >
  void
  NormalizedGaussianInterpolationTensorImageFilter< TInputImage, TOutputImage >
  ::SetAuxiliaryMap(AuxiliaryImagePointerType map)
  {
    m_AuxiliaryMap = map;
  }


  template< class TInputImage, class TOutputImage >
  void
  NormalizedGaussianInterpolationTensorImageFilter< TInputImage, TOutputImage >
  ::ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, ThreadIdType threadId)
  {

    // alpha controls the weight to give to the rotation invariant constrain
    
    typedef ImageRegionIterator<OutputImageType>               OutputImageIteratorType;
    typedef ImageRegionConstIteratorWithIndex<InputImageType>  InputImageIteratorType;
    typedef typename InputImageType::IndexType                 IndexType;

    typedef ImageRegionConstIteratorWithIndex<AuxiliaryImageType>  AuxiliaryImageIteratorType;

    typedef LinearInterpolateImageFunction<AuxiliaryImageType, double> InterpolatorType;
    typename InterpolatorType::Pointer interpolator = InterpolatorType::New();
    
    InputImageIteratorType itIn(this->GetInput(),outputRegionForThread);
    OutputImageIteratorType itOut(this->GetOutput(),outputRegionForThread);
    AuxiliaryImageIteratorType itSec(this->GetAuxiliaryMap(), this->GetAuxiliaryMap()->GetLargestPossibleRegion());

    if (m_AuxiliaryMap)
      interpolator->SetInputImage (this->GetAuxiliaryMap());
    
    // get the length of the list == number of seed points
    unsigned int numPoints  = m_Tensors.size();
    double SIGMA2 = m_Sigma*m_Sigma;    

    double sum = 0.0;
    IndexType index;
    
    while(!itOut.IsAtEnd())
      {

        OutputPixelType out( static_cast<ScalarType>(0.0) );
        // threshold the input image that must be scalar, in order
        // to not interpolate in the full brain
 	if( itIn.Value() > static_cast<InputPixelType>(0.0)  )
// 	if( 0  )
	  {
           
            // w will contain the weights of the different seed tensors
            // that have an influence on the current voxel
	    double *w = new double[numPoints];
            // sum is the sum of the weights
            sum = 0.0;
            // current index
            index = itIn.GetIndex();
	    
	    // transform the index (coordinates in the image) into
	    // world coordinates
	    PointType pt;

	    ContinuousIndex<double, ImageDimension> auxindex;
	    ContinuousIndex<double, ImageDimension> auxindexn;
	    
	    this->GetInput()->TransformIndexToPhysicalPoint(index,pt);
	    if (m_AuxiliaryMap)
	      this->GetAuxiliaryMap()->TransformPhysicalPointToContinuousIndex(pt,auxindex);

	    PointType ptn;
	    double dist = 0.0;
	    double auxdist = 0.0;
	    
	    for(unsigned int n=0; n<numPoints; n++)
	    {
		ptn = m_Points[n];
		dist = 0.0;

                // Euclidean distance to the considered point
                for(unsigned int m=0; m<ImageDimension; m++)
		  dist += (ptn[m]-pt[m])*(ptn[m]-pt[m]);

		if (m_AuxiliaryMap)
		{
		  this->GetAuxiliaryMap()->TransformPhysicalPointToContinuousIndex(ptn,auxindexn);		
		  auxdist = (interpolator->EvaluateAtContinuousIndex (auxindex) - interpolator->EvaluateAtContinuousIndex (auxindexn));
		  auxdist *= auxdist;

		  dist = (1.0 - m_AuxiliaryAlpha)* dist + m_AuxiliaryAlpha * auxdist;
		}
		
		// coeff gaussien: as we normalize, one does not need
                // the 1/sqrt(2*pi)^(N/2) thing. 
		double G = exp(-0.5*dist/SIGMA2);
		w[n] = G;
		sum += G;
	      }

            // Tensor weighted mean: we simply compute the mean in the
            // logarithmic domain
            OutputPixelType Mean ( static_cast<ScalarType>(0.0) );
            
            for(unsigned int n=0; n<numPoints; n++)
            {
	      Mean += m_Tensors[n] * static_cast<ScalarType>( w[n] );
            }
            
 	    Mean /= static_cast<ScalarType>(sum);
            out = Mean.Exp();
            delete [] w;
                                    
	  }

        
	itOut.Set(out);
	
	++itIn;
	++itOut;
      }
    
  }

} // end of namespace itk

#endif
