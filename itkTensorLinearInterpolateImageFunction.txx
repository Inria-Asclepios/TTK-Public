#ifndef __itkTensorLinearInterpolateImageFunction_txx
#define __itkTensorLinearInterpolateImageFunction_txx

#include "itkTensorLinearInterpolateImageFunction.h"

#include "vnl/vnl_math.h"

namespace itk
{
  
  /**
   * Define the number of neighbors
   */
  template<class TInputImage, class TCoordRep>
  const unsigned long
  TensorLinearInterpolateImageFunction< TInputImage, TCoordRep >
  ::m_Neighbors = 1 << TInputImage::ImageDimension;
  

  /**
   * Constructor
   */
  template<class TInputImage, class TCoordRep>
  TensorLinearInterpolateImageFunction< TInputImage, TCoordRep >
  ::TensorLinearInterpolateImageFunction()
  {
    
  }
  
  
  /**
   * PrintSelf
   */
  template<class TInputImage, class TCoordRep>
  void
  TensorLinearInterpolateImageFunction< TInputImage, TCoordRep >
  ::PrintSelf(std::ostream& os, Indent indent) const
  {
    this->Superclass::PrintSelf(os,indent);
  }
  
  
  /**
   * Evaluate at image index position
   */
  template<class TInputImage, class TCoordRep>
  typename TensorLinearInterpolateImageFunction< TInputImage, TCoordRep >
  ::OutputType
  TensorLinearInterpolateImageFunction< TInputImage, TCoordRep >
  ::EvaluateAtContinuousIndex( const ContinuousIndexType& index) const
  {
    unsigned int dim;  // index over dimension
    
    /**
     * Compute base index = closet index below point
     * Compute distance from point to base index
     */
    signed long baseIndex[ImageDimension];
    double distance[ImageDimension];
    
    for( dim = 0; dim < ImageDimension; dim++ )
    {
#if ITK_VERSION_MAJOR>3 || (ITK_VERSION_MAJOR==3 && ITK_VERSION_MINOR>=16)
      baseIndex[dim] = Math::Floor( index[dim] );
#else
      baseIndex[dim] = (long)( index[dim] );
#endif
      distance[dim] = index[dim] - static_cast< double >( baseIndex[dim] );
    }
    
    /**
     * Interpolated value is the weighted sum of each of the surrounding
     * neighbors. The weight for each neighbor is the fraction overlap
     * of the neighbor pixel with respect to a pixel centered on point.
     */
    RealType   value = NumericTraits<RealType>::Zero;
    ScalarType totalOverlap = NumericTraits<ScalarType>::Zero;

    for( unsigned int counter = 0; counter < m_Neighbors; counter++ )
    {
      
      double overlap = 1.0;          // fraction overlap
      unsigned int upper = counter;  // each bit indicates upper/lower neighbour
      IndexType neighIndex;
      
      // get neighbor index and overlap fraction
      for( dim = 0; dim < ImageDimension; dim++ )
      {
        
	if ( upper & 1 )
        {
	  neighIndex[dim] = baseIndex[dim] + 1;
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
	  // Take care of the case where the pixel is just
	  // in the outer upper boundary of the image grid.
	  if( neighIndex[dim] > this->m_EndIndex[dim] )
          {
	    neighIndex[dim] = this->m_EndIndex[dim];
          }
#endif
	  overlap *= distance[dim];
        }
	else
        {
	  neighIndex[dim] = baseIndex[dim];
#ifdef ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
	  // Take care of the case where the pixel is just
	  // in the outer lower boundary of the image grid.
	  if( neighIndex[dim] < this->m_StartIndex[dim] )
          {
	    neighIndex[dim] = this->m_StartIndex[dim];
          }
#endif
	  overlap *= 1.0 - distance[dim];
        }
	
	upper >>= 1;
	
      }
      
      // get neighbor value only if overlap is not zero
      // Moreover, one does not interpolate with null tensors.
      if( overlap )
      {
        RealType T = static_cast<RealType>( this->GetInputImage()->GetPixel( neighIndex ) );
        if( !T.IsZero() )
        {
          value += T * overlap;
          totalOverlap += overlap;
        }
      }

      if( totalOverlap == 1.0 )
       {
	// finished
	break;
       }
      
    }

    if( totalOverlap!=NumericTraits<ScalarType>::Zero )
    {
      if( !value.IsZero() )
      {
        return static_cast<OutputType>( value )/totalOverlap;
      }
    }
    
    return ( static_cast<OutputType>( value ) );
}

} // end namespace itk

#endif
