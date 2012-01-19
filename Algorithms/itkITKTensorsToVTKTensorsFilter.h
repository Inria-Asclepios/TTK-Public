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
#ifndef __itkITKTensorsToVTKTensorsFilter_h
#define __itkITKTensorsToVTKTensorsFilter_h

#include "itkImage.h"
#include "itkImageToImageFilter.h"

class vtkStructuredPoints;

namespace itk
{
  
/** \class ITKTensorsToVTKTensorsFilter
 * \brief  Convert an ITK tensor image into a VTK structured points
 *
 * \ingroup   ImageFilter
 */
template < class TTensorImage >
class ITK_EXPORT ITKTensorsToVTKTensorsFilter : public ImageToImageFilter< TTensorImage, TTensorImage >
{
public:
	/** Some typedefs. */
	typedef TTensorImage                             TensorImageType;
	typedef typename  TensorImageType::PixelType     TensorType;
 	typedef typename  TensorImageType::Pointer       TensorImagePointer;
	typedef typename  TensorImageType::ConstPointer  TensorImageConstPointer;

	/** Standard class typedefs. */
	typedef ITKTensorsToVTKTensorsFilter                         Self;
	typedef ImageToImageFilter<TensorImageType,TensorImageType>  Superclass;
	typedef SmartPointer<Self>                                   Pointer;
	typedef SmartPointer<const Self>                             ConstPointer;

	/** Method for creation through the object factory. */
	itkNewMacro(Self);
  
	/** Run-time type information (and related methods). */
	itkTypeMacro(ITKTensorsToVTKTensorsFilter, ImageToImageFilter);

	/** Return the input (no ITK filtering is performed on the input
	 * image) */
	TensorImagePointer GetOutput();

	/** Get the VTK tensor image to obtained from the input ITK tensor
	 * image */
	void CopyVTKTensors( vtkStructuredPoints* );

	vtkStructuredPoints* GetVTKTensors(void) const
	{ return m_VTKTensors; }

protected:
	ITKTensorsToVTKTensorsFilter(); 
	virtual ~ITKTensorsToVTKTensorsFilter(); 

	/** Method that actually read the ITK data and convert it into a
	 *  VTK structured point */
	virtual void GenerateData();

private:
	ITKTensorsToVTKTensorsFilter(const Self&);   //purposely not implemented
	void operator=(const Self&);                 //purposely not implemented

	/** VTK output */
	vtkStructuredPoints* m_VTKTensors;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkITKTensorsToVTKTensorsFilter.txx"
#endif

#endif



