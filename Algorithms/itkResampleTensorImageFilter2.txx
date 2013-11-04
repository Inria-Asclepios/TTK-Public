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
#ifndef _itk_ResampleTensorImageFilter_txx_
#define _itk_ResampleTensorImageFilter_txx_
#include "itkResampleTensorImageFilter.h"

#include <itkContinuousIndex.h>
#include <itkZeroFluxNeumannBoundaryCondition.h>
#include <itkConstNeighborhoodIterator.h>
#include <itkImageRegionIterator.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkImageRegionConstIterator.h>


namespace itk
{


  template< class TInputImage, class TOutputImage >
  void
  ResampleTensorImageFilter< TInputImage, TOutputImage >
  ::GenerateInputRequestedRegion() throw (InvalidRequestedRegionError)
  {

    // call the superclass's implementation of this method
    Superclass::GenerateInputRequestedRegion();
    
    if ( !this->GetInput() )
      return;
    
    // get pointers to the input and output
    typename InputImageType::Pointer  inputPtr  = 
      const_cast< TInputImage *>( this->GetInput() );
    
    // Request the entire input image
    typename InputImageType::RegionType inputRegion;
    inputRegion = inputPtr->GetLargestPossibleRegion();
    inputPtr->SetLargestPossibleRegion(inputRegion);
    inputPtr->SetRequestedRegion(inputRegion);
    
    return;
    
  }

  template < class TInputImage, class TOutputImage >
  void 
  ResampleTensorImageFilter< TInputImage, TOutputImage >
  ::GenerateOutputInformation()
  {
    // call the superclass' implementation of this method
    Superclass::GenerateOutputInformation();
    
    // get pointers to the input and output
    typename OutputImageType::Pointer outputPtr = this->GetOutput();
    if ( !outputPtr )
      return;
    
    // Set the size of the output region
    typename TOutputImage::RegionType outputLargestPossibleRegion;
    IndexType index = {{0,0,0}};
    outputLargestPossibleRegion.SetSize( m_OutputSize );
    outputLargestPossibleRegion.SetIndex( index );
    outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );
    
    // Set spacing and origin
    outputPtr->SetSpacing( m_OutputSpacing );
    outputPtr->SetOrigin( m_OutputOrigin );
    
    return;
  }
  
  

  
  template< class TInputImage, class TOutputImage >
  void
  ResampleTensorImageFilter<TInputImage, TOutputImage >
  ::ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, ThreadIdType threadId)
  {
    
    typedef ZeroFluxNeumannBoundaryCondition<TOutputImage>                   BoundaryConditionType;
    typedef ConstNeighborhoodIterator<InputImageType,BoundaryConditionType>  NIType;
    typedef ImageRegionConstIterator<InputImageType>                         InputIteratorType;
    typedef ImageRegionIterator<OutputImageType>                             IteratorType;
    
    InputIteratorType itIn(this->GetInput(), this->GetInput()->GetLargestPossibleRegion());
    IteratorType itOut(this->GetOutput(), outputRegionForThread);
    
    typename InputImageType::SizeType dims = this->GetInput()->GetLargestPossibleRegion().GetSize();
    
    while(!itOut.IsAtEnd())
    {
      IndexType index = itOut.GetIndex();
      //std::cout << "Output index: " << index << std::endl;
      
      PointType realCoord;
      this->GetOutput()->TransformIndexToPhysicalPoint(index,realCoord);
      //std::cout << "Real coord is: " << realCoord << std::endl;
      
	
      ContinuousIndexType inputIndex;
      bool isInside = this->GetInput()->TransformPhysicalPointToContinuousIndex(realCoord, inputIndex);

      //std::cout << "Continuous index in input:  " << inputIndex <<  std::endl;
      //getchar();
      
      
      OutputPixelType out( static_cast<ScalarType>( 0.0 ) );
      
      
      if(isInside)
      {
        int x = int(inputIndex[0]);
        int y = int(inputIndex[1]);
        int z = int(inputIndex[2]);
        
        double c[8];
        std::vector<InputPixelType> T;
        int ind=0;
        double sum = 0.0;
	
        // The go-through should be replaced with a Neighborhood Iterator
        // TO CHANGE FOR A BETTER COMPATIBILTY WITH ND IMAGES
        for(int nx=0;nx<=1;nx++)
        {
          for(int ny=0;ny<=1;ny++)
          {
            for(int nz=0;nz<=1;nz++)
            {
              
              if(x+nx<(int)dims[0] && y+ny<(int)dims[1] && z+nz<(int)dims[2])
              {
		
                IndexType inIndex = {{x+nx,y+ny,z+nz}};
                itIn.SetIndex(inIndex);
                InputPixelType Ti = itIn.Get();

                if ( !Ti.IsZero() )
                {                  
                  T.push_back(Ti);
                  int cx = x;
                  int cy = y;
                  int cz = z;
                  if(nx==0)
                    cx += 1;
                  if(ny==0)
                    cy += 1;
                  if(nz==0)
                    cz += 1;
                  
                  c[ind] = fabs( double( (cx-inputIndex[0])*(cy-inputIndex[1])*(cz-inputIndex[2]) ));
                  sum += c[ind];
                  ++ind;
                }
                
                
              }			
            }
          }		
        }
	
        // Riemannian mean	    
        unsigned int n = T.size();
	
        if(n!=0 && sum!=0.0) 
        {
          OutputPixelType Mean( static_cast<ScalarType>( 0.0 ) );
          
          for(unsigned int i=0;i<n;i++)
            Mean += T[i] * static_cast<ScalarType>( c[i] );

          Mean /= static_cast<ScalarType> (sum);
          
          
          
          out = Mean;
                    
        }
	
      }
      
      itOut.Set(out);
      ++itOut;
    }
    
    
    
  }
  
} // end of namespace itk

#endif
  
