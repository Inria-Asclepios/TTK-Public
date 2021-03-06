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
#ifndef __itkWarpTensorImageFilter_txx
#define __itkWarpTensorImageFilter_txx
#include "itkWarpTensorImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkAffineTensorTransform.h"
#include "itkProgressReporter.h"

namespace itk
{

/**
 * Default constructor.
 */
template <class TInputImage,class TOutputImage,class TDisplacementField>
WarpTensorImageFilter<TInputImage,TOutputImage,TDisplacementField>
::WarpTensorImageFilter()
{
  // Setup the number of required inputs
  this->SetNumberOfRequiredInputs( 2 );  
  
  // Setup default values
  m_OutputSpacing.Fill( 1.0 );
  m_OutputOrigin.Fill( 0.0 );
  m_OutputDirection.SetIdentity();
  
  m_EdgePaddingValue = NumericTraits<PixelType>::Zero;

  // Setup default interpolator
  typename DefaultInterpolatorType::Pointer interp =
    DefaultInterpolatorType::New();

  m_Interpolator = 
    static_cast<InterpolatorType*>( interp.GetPointer() );
    
  m_Jacobian = 0;
  
  m_ReorientationStrategy = FS;
  
}

/**
 * Standard PrintSelf method.
 */
template <class TInputImage,class TOutputImage,class TDisplacementField>
void
WarpTensorImageFilter<TInputImage,TOutputImage,TDisplacementField>
::PrintSelf(std::ostream& os, Indent indent) const
{

  Superclass::PrintSelf(os, indent);

  os << indent << "OutputSpacing: " << m_OutputSpacing << std::endl;;
  os << indent << "OutputOrigin: " << m_OutputOrigin << std::endl;
  os << indent << "OutputDirection: " << m_OutputDirection << std::endl;
  os << indent << "EdgePaddingValue: "
       << static_cast<typename NumericTraits<PixelType>::PrintType>(m_EdgePaddingValue)
     << std::endl;
  os << indent << "Interpolator: " << m_Interpolator.GetPointer() << std::endl;
  
}


/**
 * Set the output image spacing.
 *
 */
template <class TInputImage,class TOutputImage,class TDisplacementField>
void
WarpTensorImageFilter<TInputImage,TOutputImage,TDisplacementField>
::SetOutputSpacing(
  const double* spacing)
{
  SpacingType s(spacing);
  this->SetOutputSpacing( s );
}


/**
 * Set the output image origin.
 *
 */
template <class TInputImage,class TOutputImage,class TDisplacementField>
void
WarpTensorImageFilter<TInputImage,TOutputImage,TDisplacementField>
::SetOutputOrigin(
  const double* origin)
{
  PointType p(origin);
  this->SetOutputOrigin(p);
}



/**
 * Set deformation field as Inputs[1] for this ProcessObject.
 *
 */
template <class TInputImage,class TOutputImage,class TDisplacementField>
void
WarpTensorImageFilter<TInputImage,TOutputImage,TDisplacementField>
::SetDisplacementField(
  const DisplacementFieldType * field )
{
  // const cast is needed because the pipeline is not const-correct.
  DisplacementFieldType * input =  
       const_cast< DisplacementFieldType * >( field );
  this->ProcessObject::SetNthInput( 1, input );
}


/**
 * Return a pointer to the deformation field.
 */
template <class TInputImage,class TOutputImage,class TDisplacementField>
typename WarpTensorImageFilter<TInputImage,TOutputImage,TDisplacementField>
::DisplacementFieldType *
WarpTensorImageFilter<TInputImage,TOutputImage,TDisplacementField>
::GetDisplacementField(void)
{
  return static_cast<DisplacementFieldType *>
    ( this->ProcessObject::GetInput( 1 ));
}


/**
 * Setup state of filter before multi-threading.
 * InterpolatorType::SetInputImage is not thread-safe and hence
 * has to be setup before ThreadedGenerateData
 */
template <class TInputImage,class TOutputImage,class TDisplacementField>
void
WarpTensorImageFilter<TInputImage,TOutputImage,TDisplacementField>
::BeforeThreadedGenerateData()
{

  if( !m_Interpolator )
    {
    itkExceptionMacro(<< "Interpolator not set");
    }

  // Connect input image to interpolator
  m_Interpolator->SetInputImage( this->GetInput() );
  
  // compute the Jacobian:
  typename JacobianFilterType::Pointer jacobianFilter = JacobianFilterType::New();
  jacobianFilter->SetInput( this->GetDisplacementField() );
  jacobianFilter->SetUseImageSpacing( true );
  
  try
  {
	jacobianFilter->Update();
  }
  catch(itk::ExceptionObject &e)
  {
	std::cerr << e;
	throw itk::ExceptionObject(__FILE__,__LINE__,"Error in WarpTensorImageFilter::BeforeThreadedGenerateData()");
  }
  
  m_Jacobian = jacobianFilter->GetOutput();

}

/**
 * Setup state of filter after multi-threading.
 */
template <class TInputImage,class TOutputImage,class TDisplacementField>
void
WarpTensorImageFilter<TInputImage,TOutputImage,TDisplacementField>
::AfterThreadedGenerateData()
{
  // Disconnect input image from interpolator
  m_Interpolator->SetInputImage( NULL );

}


/**
 * Compute the output for the region specified by outputRegionForThread.
 */
template <class TInputImage,class TOutputImage,class TDisplacementField>
void
WarpTensorImageFilter<TInputImage,TOutputImage,TDisplacementField>
::DynamicThreadedGenerateData(
  const OutputImageRegionType& outputRegionForThread)
{

  InputImageConstPointer inputPtr = this->GetInput();
  OutputImagePointer outputPtr = this->GetOutput();
  DisplacementFieldPointer fieldPtr = this->GetDisplacementField();
  JacobianPointer jacobianPtr = this->GetJacobian();

  // iterator for the output image
  ImageRegionIteratorWithIndex<OutputImageType> outputIt(
    outputPtr, outputRegionForThread );

  // iterator for the deformation field
  ImageRegionIterator<DisplacementFieldType> fieldIt(
    fieldPtr, outputRegionForThread );
    
  // iterator for the output image
  ImageRegionIteratorWithIndex<JacobianImageType> jacobianIt(
    jacobianPtr, outputRegionForThread );
  
  IndexType index;
  PointType point;
  DisplacementType displacement;

  while( !outputIt.IsAtEnd() )
    {
    // get the output image index
    index = outputIt.GetIndex();
    outputPtr->TransformIndexToPhysicalPoint( index, point );

    // get the required displacement
    displacement = fieldIt.Get();

    // compute the required input image point
    for(unsigned int j = 0; j < ImageDimension; j++ )
      {
      point[j] += displacement[j];
      }

    // get the interpolated value
    if( m_Interpolator->IsInsideBuffer( point ) )
      {
      PixelType value = static_cast<PixelType>( 
        m_Interpolator->Evaluate( point ) );
        
        // rotate the value according to the Jacobian
        JacobianType jacobian = jacobianIt.Get();
        
        typedef itk::AffineTensorTransform< double, ImageDimension >  TransformType;
		typename TransformType::Pointer transform = TransformType::New();
		transform->SetMatrix( jacobian );
	switch (m_ReorientationStrategy)
	{
	  case FS:
	    value = transform->TransformTensorWithFS( value );
       	    break;
	  case PPD:
      	    value = transform->TransformTensorWithPPD( value );
  	    break;
	  default:
            value = transform->TransformTensorWithFS( value );
  	    break;
	}
 	outputIt.Set( value );
      }
    else
      {
      outputIt.Set( m_EdgePaddingValue );
      }
    ++outputIt;
    ++fieldIt; 
    ++jacobianIt;
    }

}


template <class TInputImage,class TOutputImage,class TDisplacementField>
void
WarpTensorImageFilter<TInputImage,TOutputImage,TDisplacementField>
::GenerateInputRequestedRegion()
{

  // call the superclass's implementation
  Superclass::GenerateInputRequestedRegion();

  // request the largest possible region for the input image
  InputImagePointer inputPtr = 
    const_cast< InputImageType * >( this->GetInput() );

  if( inputPtr )
    {
    inputPtr->SetRequestedRegionToLargestPossibleRegion();
    }

  // just propagate up the output requested region for the 
  // deformation field.
  DisplacementFieldPointer fieldPtr = this->GetDisplacementField();
  OutputImagePointer outputPtr = this->GetOutput();
  if( fieldPtr )
    {
    fieldPtr->SetRequestedRegion( outputPtr->GetRequestedRegion() );
    }

}


template <class TInputImage,class TOutputImage,class TDisplacementField>
void
WarpTensorImageFilter<TInputImage,TOutputImage,TDisplacementField>
::GenerateOutputInformation()
{
  // call the superclass's implementation of this method
  Superclass::GenerateOutputInformation();

  OutputImagePointer outputPtr = this->GetOutput();

  outputPtr->SetSpacing( m_OutputSpacing );
  outputPtr->SetOrigin( m_OutputOrigin );
  outputPtr->SetDirection( m_OutputDirection );

  DisplacementFieldPointer fieldPtr = this->GetDisplacementField();
  if( fieldPtr )
    {
    outputPtr->SetLargestPossibleRegion( fieldPtr->
                                         GetLargestPossibleRegion() );
    }

}


} // end namespace itk

#endif
 
