#ifndef _itk_FiberImageToVtkPolyData_txx_
#define _itk_FiberImageToVtkPolyData_txx_
#include "itkFiberImageToVtkPolyData.h"

#include <itkImageRegionIterator.h>

#include <itkContinuousIndex.h>
#include <vtkPoints.h>
#include <vtkUnsignedCharArray.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <itkImage.h>

namespace itk
{


  template<class TInputImage, class TensorImageType>
  void
  FiberImageToVtkPolyData<TInputImage,TensorImageType>::
  Update()
  {

    if( this->GetInput().IsNull() || !this->GetTensorImage() || !this->GetLogTensorImage() )
    {
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error: Input or Tensors are not set.");
    }

    typedef typename TensorImageType::PixelType TensorType;
    
    typename InterpolatorType::Pointer interpolator = InterpolatorType::New();
    /*
    typename LogFilterType::Pointer loger = LogFilterType::New();
    loger->SetInput( m_TensorImage );
    try
    {
      loger->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error in FiberImageToVtkPolyData::Update()");
    }
    interpolator->SetInputImage( loger->GetOutput() );
    */
    
    interpolator->SetInputImage( this->GetLogTensorImage() );
    
    
    //m_Output->Reset();
    m_Output->Initialize();
    m_Output->Allocate();

    typedef itk::ContinuousIndex<ScalarType, TensorImageType::ImageDimension> ContinuousIndexType;
    
    typedef ImageRegionConstIterator<InputImageType> InputIteratorType;
    InputIteratorType itIn (this->GetInput(), this->GetInput()->GetLargestPossibleRegion());

    
    vtkPoints*            myPoints = vtkPoints::New();
    vtkUnsignedCharArray* myColors = vtkUnsignedCharArray::New();
    vtkUnsignedCharArray* myCellColors = vtkUnsignedCharArray::New();
    vtkFloatArray*        myFAArray = vtkFloatArray::New();
    myColors->SetNumberOfComponents (3);
    myCellColors->SetNumberOfComponents (3);
    myFAArray->SetName ("FA");
    myFAArray->SetNumberOfComponents (1);


    unsigned long numPixels = this->GetInput()->GetLargestPossibleRegion().GetNumberOfPixels();
    unsigned long step      = numPixels/10;
    unsigned long progress  = 0;

    
    this->UpdateProgress (0.0);

    
    while( !itIn.IsAtEnd() )
    {

      FiberType     Fiber      = itIn.Get();
      PointListType listPoints = Fiber.GetPointList();
      int npts = (int)(listPoints.size());

      
      
      if(npts>1)
      {
        vtkIdType* ids = new vtkIdType[npts];

        // special case of the first point:
        VectorType diff = listPoints[1]-listPoints[0];
        diff /= diff.GetNorm();
	
        //double alpha = 1.0;
        double fa = 0.0;

        if( !m_TensorImage.IsNull() )
        {
          ContinuousIndexType index;
          PointType           itkPoint;
          
          for( unsigned int j=0; j<TensorImageType::ImageDimension; j++)
          {
            itkPoint[j] = listPoints[0][j];
          }

          if( this->GetAffineTransform() )
          {
            m_TensorImage->TransformPhysicalPointToContinuousIndex( this->GetAffineTransform()->TransformPoint (itkPoint), index );
          }
          else
          {
            m_TensorImage->TransformPhysicalPointToContinuousIndex( itkPoint, index );
          }
          
          
          if( interpolator->IsInsideBuffer( index ) )
          {
            //TensorType t = interpolator->EvaluateAtContinuousIndex( index ).Exp();
            TensorType t = interpolator->EvaluateAtContinuousIndex( index );
	    if( !t.IsZero() )
	    {
	      t  = t.Exp();
	      fa = t.GetFA();
	    }
            //alpha = t.GetFA();
          }
        }
        
        
	
        for( unsigned int i=0; i<3; i++)
        {
          //double c = 2.0*fabs (diff[i]*alpha)*255.0;
          double c = fabs (diff[i])*255.0;
          myColors->InsertNextValue( (unsigned char)( c>255.0?255.0:c ) );
        }
        myFAArray->InsertNextValue (fa);
        //myColors->InsertNextValue( (unsigned char)(alpha*255.0) );
        //myColors->InsertNextValue( (unsigned char)(255.0) );
        
        PointType pt = listPoints[0];
        ids[0] = myPoints->InsertNextPoint (pt[0],pt[1],pt[2]);
        
        if( npts>1)
        {
          for( int i=1; i<npts-1; i++)
          {
            
            PointType point = listPoints[i];
            
            //alpha = 1.0;
            fa = 0.0;

            if( !m_TensorImage.IsNull() )
            {
              ContinuousIndexType index;
              PointType           itkPoint;
              
              for( unsigned int j=0; j<3; j++)
              {
                itkPoint[j] = point[j];
              }

              if( this->GetAffineTransform() )
              {
                m_TensorImage->TransformPhysicalPointToContinuousIndex( this->GetAffineTransform()->TransformPoint (itkPoint), index );
              }
              else
              {
                m_TensorImage->TransformPhysicalPointToContinuousIndex( itkPoint, index );
              }
                            
              if( interpolator->IsInsideBuffer( index ) )
              {
                //TensorType t = interpolator->EvaluateAtContinuousIndex( index ).Exp();
                TensorType t = interpolator->EvaluateAtContinuousIndex( index );
		if( !t.IsZero() )
		{
		  t  = t.Exp();
		  fa = t.GetFA();
		}
                //alpha = t.GetFA();
              }
            }
            
            diff = listPoints[i+1]-listPoints[i-1];
            diff /= diff.GetNorm();
            for( unsigned int j=0; j<3; j++)
            {
              //double c = 2.0*fabs (diff[j]*alpha)*255.0;
              double c = fabs (diff[j])*255.0;
              myColors->InsertNextValue( (unsigned char)( c>255.0?255.0:c ) );
            }
            myFAArray->InsertNextValue (fa);
            //myColors->InsertNextValue( (unsigned char)(alpha*255.0) );
            //myColors->InsertNextValue( (unsigned char)(255.0) );
            
            pt = listPoints[i];
            ids[i] = myPoints->InsertNextPoint (pt[0],pt[1],pt[2]);
          }
          
          // special case of the last point
          
          fa = 0.0;
          //double alpha = 1.0;

          if( !m_TensorImage.IsNull() )
          {
            
            ContinuousIndexType index;
            PointType           itkPoint;
            
            for( unsigned int j=0; j<3; j++)
            {
              itkPoint[j] = listPoints[npts-1][j];
            }

            if( this->GetAffineTransform() )
            {
              m_TensorImage->TransformPhysicalPointToContinuousIndex( this->GetAffineTransform()->TransformPoint (itkPoint), index );
            }
            else
            {
              m_TensorImage->TransformPhysicalPointToContinuousIndex( itkPoint, index );
            }
            
            if( interpolator->IsInsideBuffer( index ) )
            {
              //TensorType t = interpolator->EvaluateAtContinuousIndex( index ).Exp();
              TensorType t = interpolator->EvaluateAtContinuousIndex( index );
	      if( !t.IsZero() )
	      {
		t  = t.Exp();
		fa = t.GetFA();
	      }
              //alpha = t.GetFA();
            }
          }
          
          diff = listPoints[npts-2]-listPoints[npts-1];
          diff /= diff.GetNorm();
          for( unsigned int i=0; i<3; i++)
          {
            //double c = 2.0*fabs (alpha*diff[i])*255;
            double c = fabs (diff[i])*255;
            myColors->InsertNextValue( (unsigned char)(c>255.0?255.0:c) );
          }
          myFAArray->InsertNextValue (fa);
          //myColors->InsertNextValue( (unsigned char)(255.0*alpha) );
          //myColors->InsertNextValue( (unsigned char)(255.0) );
          
          pt = listPoints[npts-1];
          ids[npts-1] = myPoints->InsertNextPoint (pt[0],pt[1],pt[2]);

        }
        
        m_Output->InsertNextCell (VTK_POLY_LINE, npts, ids);

        // cell color
        PointType first = listPoints[0];
        PointType last  = listPoints[npts-1];
        diff = last-first;
        diff.Normalize();
        for( unsigned int i=0; i<3; i++)
        {
          double c = fabs ( diff[i] )*255;
          unsigned char color = (unsigned char)(c>255.0?255.0:c);
          myCellColors->InsertNextValue ( color );
        }
        
        
        delete [] ids;
      }
      

      if( step>0)
      {        
        if( (progress%step)==0 )
        {
          this->UpdateProgress ( double(progress)/double(numPixels) );
        }
      }
      
      ++itIn;
      ++progress;
    }

    m_Output->SetPoints (myPoints);
    m_Output->GetPointData()->SetScalars (myColors);
    m_Output->GetPointData()->AddArray (myFAArray);
    m_Output->GetCellData()->SetScalars (myCellColors);
    myPoints->Delete();
    myColors->Delete();
    myCellColors->Delete();
    myFAArray->Delete();

    this->UpdateProgress (1.0);
    
  }
  
  
  
} // end of namespace

#endif
