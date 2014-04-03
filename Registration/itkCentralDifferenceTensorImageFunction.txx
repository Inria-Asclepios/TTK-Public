#ifndef __itkCentralDifferenceTensorImageFunction_txx
#define __itkCentralDifferenceTensorImageFunction_txx

#include "itkCentralDifferenceTensorImageFunction.h"

namespace itk
{


/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
CentralDifferenceTensorImageFunction<TInputImage,TCoordRep>
::CentralDifferenceTensorImageFunction()
{
#if defined(ITK_IMAGE_BEHAVES_AS_ORIENTED_IMAGE)
  this->m_UseImageDirection = true;
#else
  this->m_UseImageDirection = false;
#endif
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
void
CentralDifferenceTensorImageFunction<TInputImage,TCoordRep>
::PrintSelf(std::ostream& os, Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "UseImageDirection = " << this->m_UseImageDirection << std::endl;
}


/**
 *
 */
template <class TInputImage, class TCoordRep>
typename CentralDifferenceTensorImageFunction<TInputImage,TCoordRep>::OutputType
CentralDifferenceTensorImageFunction<TInputImage,TCoordRep>
::EvaluateAtIndex( const IndexType& index ) const
{

  OutputType derivative;
  derivative.Fill( 0.0 );

  IndexType neighIndex = index;
  InputPixelType pix;

  const typename InputImageType::SizeType& size =
    this->GetInputImage()->GetBufferedRegion().GetSize();
  const typename InputImageType::IndexType& start =
    this->GetInputImage()->GetBufferedRegion().GetIndex();

  for ( unsigned int dim = 0; dim < TInputImage::ImageDimension; dim++ )
    {
    // bounds checking
    if( index[dim] < static_cast<long>(start[dim]) + 1 ||
        index[dim] > (start[dim] + static_cast<long>(size[dim]) - 2 ) )
      {
      for (unsigned int vdim=0; vdim<DegreeOfFreedom; ++vdim)
        {
        derivative(vdim,dim) = 0.0;
        }
      continue;
      }

    // compute derivative
    neighIndex[dim] += 1;
    pix = this->GetInputImage()->GetPixel( neighIndex );
    for (unsigned int vdim=0; vdim<DegreeOfFreedom; ++vdim)
      {
      derivative(vdim,dim) = pix[vdim];
      }

    neighIndex[dim] -= 2;
    pix = this->GetInputImage()->GetPixel( neighIndex );
    for (unsigned int vdim=0; vdim<DegreeOfFreedom; ++vdim)
      {
      derivative(vdim,dim) -= pix[vdim];
      }

    for (unsigned int vdim=0; vdim<DegreeOfFreedom; ++vdim)
      {
      derivative(vdim,dim) *= 0.5 / this->GetInputImage()->GetSpacing()[dim];
      }
    neighIndex[dim] += 1;
    }

#ifdef ITK_USE_ORIENTED_IMAGE_DIRECTION
  if( this->m_UseImageDirection )
  {
    OutputType orientedDerivative;

    typename InputImageType::DirectionType direction = inputImage->GetDirection();

    for (unsigned int vdim=0; vdim<DegreeOfFreedom; vdim++)
    {
      typedef Vector<double, TInputImage::ImageDimension> VectorType;
      VectorType v;
      for (unsigned int dim=0; dim<TInputImage::ImageDimension; dim++ )
      {
	v[dim] = derivative (vdim, dim);
      }

      //v = direction * v;
      inputImage->TransformLocalVectorToPhysicalVector( v, v );

      for (unsigned int dim=0; dim<TInputImage::ImageDimension; dim++ )
      {
	orientedDerivative (vdim, dim) = v[dim];
      }
    }
    return orientedDerivative;
  }
#endif
  return ( derivative );

}


} // end namespace itk

#endif
