/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkITKTensorsToVTKTensorsFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/09/14 09:19:39 $
  Version:   $Revision: 1.3 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkITKTensorsToVTKTensorsFilter_txx
#define _itkITKTensorsToVTKTensorsFilter_txx

#include "itkITKTensorsToVTKTensorsFilter.h"

#include "vtkPointData.h"
#include "vtkStructuredPoints.h"

#include "itkImage.h"
#include "itkImageToImageFilter.h"
#include "itkImageRegionIterator.h"

namespace itk
{



/**
 * Constructor
 */
template < class TTensorImage >
ITKTensorsToVTKTensorsFilter< TTensorImage >
::ITKTensorsToVTKTensorsFilter()
{
	m_VTKTensors = vtkStructuredPoints::New();
}




/**
 * Destructor
 */
template < class TTensorImage >
ITKTensorsToVTKTensorsFilter< TTensorImage >
::~ITKTensorsToVTKTensorsFilter()
{
	m_VTKTensors->Delete();
}



/**
 * Set a vtkStructuredPoints as input 
 */
template < class TTensorImage >
void
ITKTensorsToVTKTensorsFilter< TTensorImage >
::CopyVTKTensors( vtkStructuredPoints* p )
{
	p->DeepCopy(m_VTKTensors);
}



/**
 * Get an itk::Image as output
 */
template < class TTensorImage >
typename ITKTensorsToVTKTensorsFilter<TTensorImage>::TensorImagePointer
ITKTensorsToVTKTensorsFilter< TTensorImage >
::GetOutput()
{
  return const_cast<TTensorImage*>( this->GetInput() );
}





/**
 * Do the conversion
 */
template < class TTensorImage >
void
ITKTensorsToVTKTensorsFilter< TTensorImage >
::GenerateData()
{

	TensorImageConstPointer input = this->GetInput(0);
	if ( ! input )
	{
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error: No input ITK tensor image has been set.");
	}

	// VTK only supports 3D image with 3x3 tensors
	typename TensorImageType::IndexType idx = {{0,0,0}};
	TensorType t = input->GetPixel(idx);
	if( input->GetImageDimension() != 3 || t.GetTensorDimension() != 3 )
	{
		throw itk::ExceptionObject (__FILE__,__LINE__,"Error: VTK only supports 3D images and 3x3 tensors.");
	}
    
	typename TensorImageType::SizeType size = input->GetLargestPossibleRegion().GetSize();
	int    numVoxels = 1;
	int    dims[3];
	double origin[3];
	double spacing[3];
    
	for( unsigned int i = 0; i < 3; i++ )
	{
		numVoxels *= size[i];
		dims[i]    = size[i];
		origin[i]  = input->GetOrigin()[i];
		spacing[i] = input->GetSpacing()[i];
	}
    
	vtkDoubleArray* data = vtkDoubleArray::New();
	data->SetNumberOfComponents(9);
	data->SetNumberOfTuples(numVoxels);

	typedef ImageRegionConstIterator<TensorImageType> IteratorType;
	IteratorType it(input, input->GetLargestPossibleRegion());

	unsigned long step = numVoxels/100;
	unsigned int ind   = 0;

	this->UpdateProgress ( 0.0 );
    
	while( !it.IsAtEnd() )
	{
		TensorType tensor = it.Get();
		double buffer[9];
		buffer[0] = tensor.GetNthComponent(0);
		buffer[1] = tensor.GetNthComponent(1);
		buffer[2] = tensor.GetNthComponent(3);
		buffer[3] = tensor.GetNthComponent(1);
		buffer[4] = tensor.GetNthComponent(2);
		buffer[5] = tensor.GetNthComponent(4);
		buffer[6] = tensor.GetNthComponent(3);
		buffer[7] = tensor.GetNthComponent(4);
		buffer[8] = tensor.GetNthComponent(5);
      
		data->SetTuple(ind,buffer);
		++ind;
		++it;

		if( (ind%step) == 0 )
		{
			this->UpdateProgress ( double(ind)/double(numVoxels) );
		}
	}
	
	this->UpdateProgress ( 1.0 );
  

	/*
	if ( m_VTKTensors )
		m_VTKTensors->Delete();
	*/

	//m_VTKTensors = vtkStructuredPoints::New();
	m_VTKTensors->Initialize();
	m_VTKTensors->SetDimensions(dims);
	m_VTKTensors->SetSpacing(spacing);
	m_VTKTensors->SetOrigin(origin);
	m_VTKTensors->GetPointData()->SetTensors(data);
	data->Delete();
	m_VTKTensors->Update();
}





} // end namespace itk

#endif

