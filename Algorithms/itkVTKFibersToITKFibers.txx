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
#ifndef _itk_VTKFibersToITKFibers_txx_
#define _itk_VTKFibersToITKFibers_txx_

#include "itkVTKFibersToITKFibers.h"

#include <vtkPointData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>

namespace itk
{

  template<class TensorImageType>
  void
  VTKFibersToITKFibers<TensorImageType>::
  Update()
  {

    if( !m_Input || m_TensorImage.IsNull() )
    {
      throw ExceptionObject (__FILE__,__LINE__,"Error: Trying to convert VTK fibers but none are provided.");      
    }
    
    
    // clear the GroupSpatialObject
    m_Output->Clear();
    
    
    typedef itk::ContinuousIndex<double, TensorImageType::ImageDimension> ContinuousIndexType;


    // allocate an interpolator
    typename InterpolatorType::Pointer myInterpolator = InterpolatorType::New();
    myInterpolator->SetInputImage ( m_TensorImage );
    

    // get the vtk Points
    vtkPoints* vtkPoints = m_Input->GetPoints();
    vtkUnsignedCharArray* vtkColors = vtkUnsignedCharArray::SafeDownCast ( m_Input->GetPointData()->GetScalars() );
    

    // get the vtk Lines == fibers
    int nfib = m_Input->GetNumberOfLines();
    vtkCellArray* vtkLines = m_Input->GetLines();
    vtkLines->InitTraversal();

    for(int i = 0; i < nfib; ++i)
    {
      
      typename DTITubeSpatialObjectType::Pointer dtiTube = DTITubeSpatialObjectType::New();
      dtiTube->SetId (1);
      
      vtkIdType npts = 0;
      vtkIdType const *pts = 0;
      vtkLines->GetNextCell(npts, pts);
      
      std::vector<DTIPointType> pointsToAdd;
    
      for(int j = 0; j < npts; ++j)
      {
        double* coordinates = vtkPoints->GetPoint( pts[j] );
        
        PointType PT;
        for( unsigned int n=0; n<TensorImageType::GetImageDimension(); n++)
        {
          PT[n] = coordinates[n];
        }

        ContinuousIndexType inputIndex;
        bool isInside = m_TensorImage->TransformPhysicalPointToContinuousIndex (PT, inputIndex);
        
        if( isInside )
        {
          TensorType T(0.0);
          try
          {
            if( myInterpolator->IsInsideBuffer (inputIndex) )
            {
              T = myInterpolator->EvaluateAtContinuousIndex(inputIndex);
              if( !T.IsZero() )
              {
                T = T.Exp()/m_BVal;
                //T /= m_BVal;
              }
            }
          }
          catch (itk::ExceptionObject &e)
          {
            std::cerr << e;
            throw ExceptionObject (__FILE__,__LINE__,"Error while interpolating tensors.");
          }
          
          // convert double to float and tensor order to DTIPoint order
          float SOtensor[6];
          SOtensor[0]=T[0];
          SOtensor[1]=T[1];
          SOtensor[2]=T[3];
          SOtensor[3]=T[2];
          SOtensor[4]=T[4];
          SOtensor[5]=T[5];

          DTIPointType pt;

	  
          pt.SetPositionInObjectSpace(coordinates[0],coordinates[1],coordinates[2]);
          pt.AddField("fa",T.GetFA());
          pt.AddField("ga",T.GetGA());
          pt.AddField("i",0);
          pt.AddField("l1",T.GetEigenvalue (2));
          pt.AddField("l2",T.GetEigenvalue (1));
          pt.AddField("l3",T.GetEigenvalue (0));

          VectorType vmin = T.GetEigenvector (0);
          VectorType vmed = T.GetEigenvector (1);
          VectorType vmax = T.GetEigenvector (2);
          
          pt.AddField("xevmin",vmin[0]);
          pt.AddField("yevmin",vmin[1]);
          pt.AddField("zevmin",vmin[2]);
          pt.AddField("xevmed",vmed[0]);
          pt.AddField("yevmed",vmed[1]);
          pt.AddField("zevmed",vmed[2]);
          pt.AddField("xevmax",vmax[0]);
          pt.AddField("yevmax",vmax[1]);
          pt.AddField("zevmax",vmax[2]);
          pt.AddField("md",T.GetTrace());
          pt.AddField("r",0.2);

          if( m_ColorMode==COLOR_BY_POINTDATA && vtkColors )
          {
            double* color = vtkColors->GetTuple3( pts[j] );
            pt.AddField("red",color[0]/255.0);
            pt.AddField("green",color[1]/255.0);
            pt.AddField("blue",color[2]/255.0);
          }          
          else
          {
            pt.AddField("red", m_UserColor[0]);
            pt.AddField("green", m_UserColor[1]);
            pt.AddField("blue", m_UserColor[2]);
          }

          
          pt.SetTensorMatrix(SOtensor);
          
          pointsToAdd.push_back(pt);
          
        }
        else
        {
          std::cerr << "Warning: A point is outside the volume." << std::endl;
        }
      
      }

      
      dtiTube->SetPoints(pointsToAdd);      
      m_Output->AddChild( dtiTube );


    }

  }
  
  
} // end of namespace

#endif
