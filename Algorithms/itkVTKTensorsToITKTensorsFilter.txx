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
#ifndef _itkVTKTensorsToITKTensorsFilter_txx
#define _itkVTKTensorsToITKTensorsFilter_txx

#include "itkVTKTensorsToITKTensorsFilter.h"

#include "vtkPointData.h"
#include "vtkStructuredPoints.h"

#include "itkImage.h"
#include "itkImageSource.h"
#include "itkImageRegionIterator.h"

namespace itk
{



/**
 * Constructor
 */
template < class TTensor, int TDimension >
VTKTensorsToITKTensorsFilter< TTensor, TDimension >
::VTKTensorsToITKTensorsFilter()
{
	
	m_Output = OutputImageType::New();
	m_VTKTensors = NULL;
	
}




/**
 * Destructor
 */
template < class TTensor, int TDimension >
VTKTensorsToITKTensorsFilter< TTensor,  TDimension >
::~VTKTensorsToITKTensorsFilter()
{
//	m_Output->Delete();
	m_VTKTensors = NULL;
}



/**
 * Set a vtkStructuredPoints as input 
 */
template < class TTensor, int TDimension >
void
VTKTensorsToITKTensorsFilter< TTensor, TDimension >
::SetVTKTensors( vtkStructuredPoints* p )
{
  this->m_VTKTensors = p;
  this->Modified();
}

  

/**
 * Get an itk::Image as output
 */
template < class TTensor, int TDimension >
typename itk::Image< TTensor, TDimension >::Pointer
VTKTensorsToITKTensorsFilter< TTensor, TDimension >::GetOutput() const
{
  return m_Output;
}
  
  





/**
 * Do the conversion
 */
template < class TTensor, int TDimension >
void
VTKTensorsToITKTensorsFilter< TTensor, TDimension >
::GenerateData()
{
	// VTK only supports 3D image with 3x3 tensors
	if( TDimension != 3 )
	{
		throw itk::ExceptionObject(__FILE__,__LINE__,"Error: VTK only supports 3x3 tensors.");
	}
    
	// Allocate the output image
	m_Output->Initialize();
    
	typename OutputImageType::SizeType size;
	double spacing[3];
	double origin[3];
	for(unsigned int i=0; i<3; i++)
	{
		size[i] = m_VTKTensors->GetDimensions()[i];
		spacing[i] = m_VTKTensors->GetSpacing()[i];
		origin[i] = m_VTKTensors->GetOrigin()[i];
	}

	typename OutputImageType::RegionType region;
	typename OutputImageType::IndexType index = {{0,0,0}};
	region.SetSize(size);
	region.SetIndex(index);

	m_Output->SetRegions(region);
	m_Output->SetSpacing(spacing);
	m_Output->SetOrigin(origin);

	try
	{
		m_Output->Allocate();
	}
	catch(itk::ExceptionObject &e)
	{
		std::cerr << e << std::endl;
		throw itk::ExceptionObject(__FILE__,__LINE__,"Error during memory allocation");
	}

	vtkDoubleArray* data = (vtkDoubleArray*)(m_VTKTensors->GetPointData()->GetTensors());

	typedef ImageRegionIterator<OutputImageType> IteratorType;
	IteratorType it(m_Output, m_Output->GetLargestPossibleRegion());

	unsigned long numPixels = m_Output->GetLargestPossibleRegion().GetNumberOfPixels();
	unsigned long step = numPixels/100;
	unsigned int ind = 0;

	this->UpdateProgress ( 0.0 );

	while( !it.IsAtEnd() )
	{
      
		double coefs[9];
		data->GetTuple(ind,coefs);
      
		typedef typename TensorType::ValueType TensorValueType;
		TensorValueType newCoefs[6];
		newCoefs[0] = static_cast<TensorValueType>(coefs[0]);
		newCoefs[1] = static_cast<TensorValueType>(coefs[3]);
		newCoefs[2] = static_cast<TensorValueType>(coefs[4]);
		newCoefs[3] = static_cast<TensorValueType>(coefs[6]);
		newCoefs[4] = static_cast<TensorValueType>(coefs[7]);
		newCoefs[5] = static_cast<TensorValueType>(coefs[8]);
      
		TensorType tensor(newCoefs);
		it.Set (tensor);
      
		++ind;
		++it;

		if( (ind%step)==0 )
		{
			this->UpdateProgress ( double(ind)/double(numPixels) );
		}
	}

	this->UpdateProgress ( 1.0 );
}





} // end namespace itk

#endif

