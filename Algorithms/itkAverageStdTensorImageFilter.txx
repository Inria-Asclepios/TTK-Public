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
#ifndef _itk_AverageStdTensorImageFilter_txx_
#define _itk_AverageStdTensorImageFilter_txx_

#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIterator.h>

namespace itk
{

  template<class TInputImage, class TOutputImage>
  void
  AverageStdTensorImageFilter<TInputImage,TOutputImage>
  ::BeforeThreadedGenerateData()
  {

     if( this->GetNumberOfIndexedInputs()==0 )
	 {
		throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Input is not set.");
	 }
	 
	 m_CovarianceMatrixImage->Initialize();
	 
	 // allocate the cov matrix image
	 m_CovarianceMatrixImage->SetRegions( this->GetInput(0)->GetLargestPossibleRegion() );
	 m_CovarianceMatrixImage->SetOrigin( this->GetInput(0)->GetOrigin() );
	 m_CovarianceMatrixImage->SetSpacing( this->GetInput(0)->GetSpacing() );
	 
	 m_CovarianceMatrixImage->Allocate();
	 
	 CovMatrixType zero(0.0);
	 m_CovarianceMatrixImage->FillBuffer(zero);
  }
  
  
  template<class TInputImage, class TOutputImage>
  void
  AverageStdTensorImageFilter<TInputImage,TOutputImage>
  ::ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, ThreadIdType threadId)
  {
  
    typedef ImageRegionIterator<OutputImageType>      IteratorOutputType;
	typedef ImageRegionIterator<CovMatrixImageType>   IteratorCovMatrixType;
    typedef ImageRegionConstIterator<InputImageType>  IteratorInputType;
	    
    unsigned int n = this->GetNumberOfIndexedInputs();
    
	IteratorOutputType    itOut(this->GetOutput(), outputRegionForThread);
	IteratorCovMatrixType itCovMatrix(this->GetCovarianceMatrixImage(), outputRegionForThread);

    // create a list of iterators for each input
    std::vector<IteratorInputType> ListOfInputIterators;
    for(unsigned int i=0 ;i<n; i++)
    {
	   IteratorInputType it(this->GetInput(i),outputRegionForThread);
	   ListOfInputIterators.push_back(it);
    }
	
	
	if( threadId==0 )
      this->UpdateProgress (0.0);
    
    while(!itOut.IsAtEnd())
    {

      if( this->GetAbortGenerateData() )
	  {
        throw itk::ProcessAborted(__FILE__,__LINE__);
	  }
	  
	  
	  InputPixelType mean( static_cast<ScalarType>(0.0) );
	  
	  
	  
	  // first step: average tensors
	  unsigned int numTensors = 0;
	  for( unsigned int i=0; i<n; i++)
	  {
		if( !ListOfInputIterators[i].Get().IsZero() )
		{
		try
		{
		    InputPixelType pix = ListOfInputIterators[i].Value();
			if( threadId==0)
			{
			//std::cout << pix << std::endl;
			}
			mean += pix.Log();
			++numTensors;
			}
			catch(itk::ExceptionObject &e)
			{
				std::cerr << e;
			}
		}
	  }
	  
	  
	  
	  if( numTensors >0 )
	  {
		mean /= static_cast<ScalarType>(numTensors);
		mean = mean.Exp();
	  }

	  itOut.Set(mean);
	  
	  
	  // second step: the covariance matrix
	  CovMatrixType covMat (static_cast<ScalarType>(0.0));
	  VectorType vecMean = this->Tensor2Vec( mean.Log() );
	  numTensors = 0;
	  for( unsigned int i=0; i<n; i++)
	  {
		if( !ListOfInputIterators[i].Get().IsZero() )
		{
		try
		{
			covMat += CovMatrixType( this->Tensor2Vec(ListOfInputIterators[i].Get().Log()) - vecMean ); 
			++numTensors;
			}
			catch(itk::ExceptionObject &e)
			{
				std::cerr << e;
			}
		}
	  }
	  
	  if( numTensors>1)
	  {
		covMat /= static_cast<ScalarType>( numTensors-1 );
	  }
	  
	  itCovMatrix.Set(covMat);
	
      ++itOut;
	  ++itCovMatrix;
      for(unsigned int i=0; i<n; i++)
	  {
        ++(ListOfInputIterators[i]);
	  }

	}

  
  }
  

  template<class TInputImage, class TOutputImage>
  typename AverageStdTensorImageFilter<TInputImage,TOutputImage>::VectorType
  AverageStdTensorImageFilter<TInputImage,TOutputImage>
  ::Tensor2Vec(const InputPixelType &tensor) const
  {
    VectorType vec;
	
	for( unsigned int i=0; i<Self::TensorDegreesOfFreedom; i++)
	{
		vec[i] = tensor.GetNthComponentAsVector(i);
	}
    return vec;
  }


}


#endif
