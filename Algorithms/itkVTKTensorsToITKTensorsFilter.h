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
#ifndef __itkVTKTensorsToITKTensorsFilter_h
#define __itkVTKTensorsToITKTensorsFilter_h

#include "itkImage.h"
#include "itkImageSource.h"

class vtkStructuredPoints;

namespace itk
{
  
/** \class VTKTensorsToITKTensorsFilter
 * \brief Convert a VTK tensor structured points into an ITK tensor
 * image
 *
 * \ingroup   ImageSources     
 */
template < class TTensor, int TDimension >
class ITK_EXPORT VTKTensorsToITKTensorsFilter : public ImageSource< itk::Image<TTensor, TDimension> >
{
public:
	/** Some typedefs. */
	typedef TTensor                                      TensorType;
 	typedef typename  itk::Image< TTensor, TDimension >  OutputImageType;
 	typedef typename  OutputImageType::Pointer           OutputImagePointer;
	typedef typename  OutputImageType::PixelType         OutputImagePixelType;

	/** Standard class typedefs. */
	typedef VTKTensorsToITKTensorsFilter       Self;
	typedef ImageSource<OutputImageType>       Superclass;
	typedef SmartPointer<Self>                 Pointer;
	typedef SmartPointer<const Self>           ConstPointer;

	/** Method for creation through the object factory. */
	itkNewMacro(Self);
  
	/** Run-time type information (and related methods). */
	itkTypeMacro(VTKTensorsToITKTensorsFilter, ImageSource);

	/** Return the ITK tensor image */
	OutputImagePointer GetOutput() const;

	/** Set the input VTK tensor image to convert */
	void SetVTKTensors( vtkStructuredPoints* );

protected:
	VTKTensorsToITKTensorsFilter(); 
	virtual ~VTKTensorsToITKTensorsFilter(); 

	/** Method that actually read the VTK data and convert it into an
	 *  ITK data */
	virtual void GenerateData();

private:
	VTKTensorsToITKTensorsFilter(const Self&);   //purposely not implemented
	void operator=(const Self&);                 //purposely not implemented

	/** Output ITK image */
	OutputImagePointer   m_Output;
	
	/** VTK input */
	vtkStructuredPoints* m_VTKTensors;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVTKTensorsToITKTensorsFilter.txx"
#endif

#endif



