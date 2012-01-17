/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: https://scm.gforge.inria.fr/svn/ttk/trunk/Algorithms/itkSheetImageToVtkPolyData.h $
  Language:  C++
  Date:      $Date: 2010-12-10 20:55:58 +0000 (Fri, 10 Dec 2010) $
  Version:   $Revision: 122 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_SheetImageToVtkPolyData_h_
#define _itk_SheetImageToVtkPolyData_h_

#include <itkProcessObject.h>
#include <vtkPolyData.h>
#include <itkTensor.h>
#include <itkImage.h>
#include <itkAffineTransform.h>

namespace itk
{
  /*! \class SheetImageToVtkPolyData
    \ingroup TensorToolKit
    Transforms a fiber image into a vtkPolyData.
   */

  template <class TInputImage>
    class ITK_EXPORT SheetImageToVtkPolyData: public ProcessObject
  {

  public:

    typedef SheetImageToVtkPolyData  Self;
    typedef Object                   Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkNewMacro  (Self);
    itkTypeMacro (SheetImageToVtkPolyData, Object);

    typedef TInputImage                        InputImageType;
    typedef typename InputImageType::PixelType SheetType;
    typedef typename SheetType::PointType      PointType;
    typedef typename SheetType::TensorType     TensorType;
    typedef typename PointType::VectorType     VectorType;
    typedef typename SheetType::SheetPoint     SheetPointType;
    typedef typename SheetType::SheetPointListType  SheetPointListType;
    typedef typename SheetType::ScalarType     ScalarType;
    typedef vtkPolyData                        OutputType;


    itkSetObjectMacro (Input, InputImageType);
    itkGetObjectMacro (Input, InputImageType);

    OutputType *GetOutput (void) const;
    
    void Update (void);

  protected:
    SheetImageToVtkPolyData();
    ~SheetImageToVtkPolyData();

    
  private:
    SheetImageToVtkPolyData (const Self&);
    void operator=(const Self&);

    typename InputImageType::Pointer m_Input;
    OutputType*                      m_Output;
  };
    

} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSheetImageToVtkPolyData.txx"
#endif

#endif
