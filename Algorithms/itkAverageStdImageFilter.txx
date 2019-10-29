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
#ifndef _itk_AverageStdImageFilter_txx_
#define _itk_AverageStdImageFilter_txx_

#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIterator.h>

namespace itk
{

  template<class TInputImage, class TOutputImage>
  void
  AverageStdImageFilter<TInputImage,TOutputImage>
  ::BeforeThreadedGenerateData()
  {

     if( this->GetNumberOfIndexedInputs()==0 )
	 {
		throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Input is not set.");
	 }
	 
	 m_VarianceImage->Initialize();
	 
	 // allocate the cov matrix image
	 m_VarianceImage->SetRegions( this->GetInput(0)->GetLargestPossibleRegion() );
	 m_VarianceImage->SetOrigin( this->GetInput(0)->GetOrigin() );
	 m_VarianceImage->SetSpacing( this->GetInput(0)->GetSpacing() );
	 m_VarianceImage->SetDirection( this->GetInput(0)->GetDirection() );	 
	 
	 m_VarianceImage->Allocate();
	 
	 OutputPixelType zero(0.0);
	 m_VarianceImage->FillBuffer(zero);
  }
  
  
  template<class TInputImage, class TOutputImage>
  void
  AverageStdImageFilter<TInputImage,TOutputImage>
  ::DynamicThreadedGenerateData(const OutputImageRegionType &outputRegionForThread)
  {
  
    typedef ImageRegionIterator<OutputImageType>      IteratorOutputType;
	typedef ImageRegionIterator<OutputImageType>      IteratorVarianceType;
    typedef ImageRegionConstIterator<InputImageType>  IteratorInputType;
	    
    unsigned int n = this->GetNumberOfIndexedInputs();
    
	IteratorOutputType    itOut(this->GetOutput(), outputRegionForThread);
	IteratorVarianceType  itVariance(this->GetVarianceImage(), outputRegionForThread);

    // create a list of iterators for each input
    std::vector<IteratorInputType> ListOfInputIterators;
    for(unsigned int i=0 ;i<n; i++)
    {
	   IteratorInputType it(this->GetInput(i),outputRegionForThread);
	   ListOfInputIterators.push_back(it);
    }
	    
    while(!itOut.IsAtEnd())
    {

      if( this->GetAbortGenerateData() )
	  {
        throw itk::ProcessAborted(__FILE__,__LINE__);
	  }
	  
	  
	  OutputPixelType mean( static_cast<ScalarType>(0.0) );
	  
	  
	  
	  // first step: average tensors
	  unsigned int numTensors = 0;
	  for( unsigned int i=0; i<n; i++)
	  {
		    InputPixelType pix = ListOfInputIterators[i].Value();
			mean += static_cast<OutputPixelType>(pix);
			++numTensors;
	  }
	  
	  
	  
	  if( numTensors >0 )
	  {
		mean /= static_cast<OutputPixelType>(numTensors);		
	  }

	  itOut.Set(mean);
	  
	  
	  // second step: the covariance matrix
	  OutputPixelType var (static_cast<OutputPixelType>(0.0));
	  numTensors = 0;
	  for( unsigned int i=0; i<n; i++)
	  {
		var += (static_cast<OutputPixelType>(ListOfInputIterators[i].Get()) - mean)*
		(static_cast<OutputPixelType>(ListOfInputIterators[i].Get()) - mean);
		++numTensors;
	  }
	  
	  if( numTensors>1)
	  {
		var /= static_cast<ScalarType>( numTensors-1 );
	  }
	  
	  itVariance.Set(var);
	
      ++itOut;
	  ++itVariance;
      for(unsigned int i=0; i<n; i++)
	  {
        ++(ListOfInputIterators[i]);
	  }

	}

  
  }


}


#endif
