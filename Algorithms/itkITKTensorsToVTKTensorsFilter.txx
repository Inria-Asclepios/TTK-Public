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
#ifndef _itkITKTensorsToVTKTensorsFilter_txx
#define _itkITKTensorsToVTKTensorsFilter_txx

#include "itkITKTensorsToVTKTensorsFilter.h"

#include "vtkPointData.h"
#include "vtkStructuredPoints.h"
#include <vtkMatrix4x4.h>
#include <vtkFloatArray.h>

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
        m_DirectionMatrix = vtkMatrix4x4::New();
        m_DirectionMatrix->Identity();
    }




    /**
 * Destructor
 */
    template < class TTensorImage >
            ITKTensorsToVTKTensorsFilter< TTensorImage >
            ::~ITKTensorsToVTKTensorsFilter()
    {
	m_VTKTensors->Delete();
        m_DirectionMatrix->Delete();
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
            typename ITKTensorsToVTKTensorsFilter<TTensorImage>::TensorImageType*
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

        vtkDataArray* data = 0;
        if (typeid(ScalarType)==typeid(double))
            data = vtkDoubleArray::New();
        else if (typeid(ScalarType)==typeid(float))
            data = vtkFloatArray::New();
        else
            itkExceptionMacro (<<"data is not float or double, cannot convert");

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

        typename TTensorImage::DirectionType directions = input->GetDirection();
        typename TTensorImage::PointType i_origin = input->GetOrigin();
        m_DirectionMatrix->Identity();
        for (int i=0; i<3; i++)
          for (int j=0; j<3; j++)
            m_DirectionMatrix->SetElement (i, j, directions (i,j));
        double v_origin[4], v_origin2[4];
        for (int i=0; i<3; i++)
          v_origin[i] = i_origin[i];
        v_origin[3] = 1.0;
        m_DirectionMatrix->MultiplyPoint (v_origin, v_origin2);
        for (int i=0; i<3; i++)
          m_DirectionMatrix->SetElement (i, 3, v_origin[i]-v_origin2[i]);
	
	this->UpdateProgress ( 1.0 );

	m_VTKTensors->Initialize();
	m_VTKTensors->SetDimensions(dims);
	m_VTKTensors->SetSpacing(spacing);
	m_VTKTensors->SetOrigin(origin);
	m_VTKTensors->GetPointData()->SetTensors(data);
	data->Delete();
	this->Update();
    }


    template < class TTensorImage >
            vtkMatrix4x4*
            ITKTensorsToVTKTensorsFilter< TTensorImage >
            ::GetDirectionMatrix() const
    {
        return m_DirectionMatrix;
    }


} // end namespace itk

#endif

