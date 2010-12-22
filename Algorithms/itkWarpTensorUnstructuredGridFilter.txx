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
#ifndef __itkWarpTensorUnstructuredGridFilter_txx
#define __itkWarpTensorUnstructuredGridFilter_txx
#include "itkWarpTensorUnstructuredGridFilter.h"

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
template <class TDisplacementField>
WarpTensorUnstructuredGridFilter<TDisplacementField>
::WarpTensorUnstructuredGridFilter()
{
  // Setup the number of required inputs
  this->SetNumberOfRequiredInputs( 0 );

  m_EdgePaddingValue = NumericTraits<PixelType>::Zero;
  
  m_Jacobian = 0;
  
  m_ReorientationStrategy = FS;

  m_DisplacementField = 0;
  m_InverseDisplacementField = 0;

  m_InverseDisplacementInterpolator = DisplacementInterpolatorType::New();
  
}

/**
 * Standard PrintSelf method.
 */
template <class TDisplacementField>
void
WarpTensorUnstructuredGridFilter<TDisplacementField>
::PrintSelf(std::ostream& os, Indent indent) const
{

  Superclass::PrintSelf(os, indent);

  os << indent << "EdgePaddingValue: "
       << static_cast<typename NumericTraits<PixelType>::PrintType>(m_EdgePaddingValue)
     << std::endl;
  
}

template <class TDisplacementField>
void
WarpTensorUnstructuredGridFilter<TDisplacementField>
::GenerateInputRequestedRegion()
{

  // call the superclass's implementation
  Superclass::GenerateInputRequestedRegion();

  // just propagate up the output requested region for the 
  // deformation field.
  DisplacementFieldPointer fieldPtr = this->GetDisplacementField();
  if( fieldPtr )
  {
    fieldPtr->SetRequestedRegion( fieldPtr->GetLargestPossibleRegion() );
  }
  
}


template <class TDisplacementField>
void
WarpTensorUnstructuredGridFilter<TDisplacementField>
::GenerateOutputInformation()
{
  // call the superclass's implementation of this method
  Superclass::GenerateOutputInformation();


  if( !this->GetDisplacementField() )
    {
    itkExceptionMacro(<< "deformation field not set");
    }
  if( !this->GetInverseDisplacementField() )
    {
    itkExceptionMacro(<< "inverse deformation field not set");
    }

  
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
    throw itk::ExceptionObject(__FILE__,__LINE__,"Error in WarpTensorUnstructuredGridFilter::BeforeThreadedGenerateData()");
  }
  
  m_Jacobian = jacobianFilter->GetOutput();

  m_InverseDisplacementInterpolator->SetInputImage (this->GetInverseDisplacementField());

  this->GenerateData();
}



/**
 * Compute the output for the region specified by outputRegionForThread.
 */
template <class TDisplacementField>
void
WarpTensorUnstructuredGridFilter<TDisplacementField>
::GenerateData()
{

  DisplacementFieldPointer fieldPtr = this->GetDisplacementField();
  DisplacementFieldPointer inversefieldPtr = this->GetInverseDisplacementField();
  JacobianPointer jacobianPtr = this->GetJacobian();

  RegionType region = fieldPtr->GetLargestPossibleRegion();
  
  // support progress methods/callbacks
  ProgressReporter progress(this, 0, m_Points.size());
  
  
  IndexType index;
  unsigned int counter = 0;
  
  m_OutputPoints.clear();
  m_OutputTensors.clear();
  
  for (unsigned int i=0; i<m_Points.size(); i++)
  {

    if ( ! m_Jacobian->TransformPhysicalPointToIndex (m_Points[i], index))
    {
      counter++;
      progress.CompletedPixel();
      continue;
    }
    
    DisplacementType d = m_InverseDisplacementInterpolator->Evaluate (m_Points[i]);
    
    PointType P = m_Points[i] + d;    

    m_OutputPoints.push_back (P);

    if (i < m_Tensors.size())
    {
      
      TensorType T = m_Tensors[i];
      
      if (! m_Jacobian->TransformPhysicalPointToIndex (P, index))
      {
	counter++;
	progress.CompletedPixel();
	continue;
      }
      
      JacobianType jacobian = m_Jacobian->GetPixel (index);
      
      typedef itk::AffineTensorTransform< double, VectorDimension >  TransformType;
      typename TransformType::Pointer transform = TransformType::New();
      transform->SetMatrix( jacobian );
      switch (m_ReorientationStrategy)
      {
	  case FS:
	    T = transform->TransformTensorWithFS( T );
	    break;
	  case PPD:
	    T = transform->TransformTensorWithPPD( T );
	    break;
	  default:
	    T = transform->TransformTensorWithFS( T );
	    break;
      }
      
      m_OutputTensors.push_back (T);
    }
    
    progress.CompletedPixel();
  }

}


} // end namespace itk

#endif
 
