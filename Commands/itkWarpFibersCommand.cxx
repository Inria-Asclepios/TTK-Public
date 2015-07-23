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
#include "itkWarpFibersCommand.h"

#include <itkVector.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkInterpolateImageFilter.h>
#include <itkVectorLinearInterpolateImageFunction.h>
#include <itkTransformFileReader.h>
#include <itkMatrixOffsetTransformBase.h>
#include <itkAffineTransform.h>
#include <itkTransformFactory.h>

#include <vtkPolyData.h>
#include <vtkAppendPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkUnsignedCharArray.h>

#include <iostream>
#include "GetPot.h"


namespace itk
{

  WarpFibersCommand::WarpFibersCommand()
  {
    m_ShortDescription = "Warp fibers w.r.t. a linear matrix or a deformation field";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i input>\n";
    m_LongDescription += "<-v vector field> or <-m matrix>\n";
    m_LongDescription += "<-o output>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  
  WarpFibersCommand::~WarpFibersCommand()
  {}
  

  int WarpFibersCommand::Execute (int narg, const char* arg[])
  {

    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }

    
    const char* file_in     = cl.follow ("",2,"-i","-I");
    const char* file_out    = cl.follow ("",2,"-o","-O");
    const char* file_vector = cl.follow ("",2,"-v","-V");
    const char* file_matrix = cl.follow ("",2,"-m","-M");
    
    
    typedef itk::Vector<double, 3>                VectorType;
    typedef itk::Image<VectorType, 3>             VectorImageType;
    typedef itk::ImageFileReader<VectorImageType> ReaderType;
    typedef itk::VectorLinearInterpolateImageFunction<VectorImageType, double> InterpolateFunctionType;
    
    
    vtkPolyDataReader* reader = vtkPolyDataReader::New();
    reader->SetFileName (file_in);
    
    vtkPolyData* bundle = reader->GetOutput();
    reader->Update();
    
    if ( strcmp (file_vector, "")!=0 && strcmp (file_matrix, "")!=0 )
    {
      std::cout << "Error: please choose between vector OR matrix" << std::endl;
      return -1;
    }
    
    
    VectorImageType::Pointer vector = 0;
    if( strcmp (file_vector, "")!=0 )
    {
      ReaderType::Pointer reader_v = ReaderType::New();
      reader_v->SetFileName (file_vector);
      try
      {
	reader_v->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr <<e;
	return -1;
      }
      vector = reader_v->GetOutput();
    }  
    
  
    
    InterpolateFunctionType::Pointer interpolator = InterpolateFunctionType::New();
    if( !vector.IsNull() )
    {
      interpolator->SetInputImage ( vector );
    }
    
    
    typedef itk::MatrixOffsetTransformBase<double, 3, 3> LinearTransformType;
    typedef itk::AffineTransform<double, 3>              AffineTransformType;
    
    LinearTransformType::Pointer transform = 0;
    if( strcmp (file_matrix, "")!=0 )
    {
      itk::TransformFactory< LinearTransformType >::RegisterTransform ();
      itk::TransformFactory< AffineTransformType >::RegisterTransform ();
      
      typedef itk::TransformFileReader TransformReaderType;
      TransformReaderType::Pointer reader_t = TransformReaderType::New();
      reader_t->SetFileName ( file_matrix );
      try
      {
	reader_t->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      transform = dynamic_cast<LinearTransformType*>( reader_t->GetTransformList()->front().GetPointer() );
    }
    
    
    vtkPoints* points = bundle->GetPoints();
    
    /*
      vtkPolyData* new_bundle = vtkPolyData::New();
      new_bundle->DeepCopy (bundle);
      vtkPoints* new_points = new_bundle->GetPoints();
      vtkUnsignedCharArray* new_colors = vtkUnsignedCharArray::New();
      new_colors->SetNumberOfComponents( 4 );
      new_colors->SetNumberOfTuples(new_points->GetNumberOfPoints());
      
      vtkUnsignedCharArray* colors = vtkUnsignedCharArray::New();
      colors->SetNumberOfComponents( 4 );
      colors->SetNumberOfTuples(new_points->GetNumberOfPoints());
      
      double n_color[4] = {0,0,255,255};
      double color[4] = {255,0,0,255};
    */
    for( int i=0; i</*new_*/points->GetNumberOfPoints(); i++ )
    {
      double pt[3];
      points->GetPoint ( i, pt);
      
      VectorImageType::PointType pt_i;
      pt_i[0] = pt[0];
      pt_i[1] = pt[1];
      pt_i[2] = pt[2];
      
      
      VectorImageType::PointType pt_n = 0.0;
      
      if( !vector.IsNull() )
      {
	VectorType vec = interpolator->Evaluate ( pt_i );
	pt_n = pt_i + vec;
      }
      else if (!transform.IsNull() )
      {
	pt_n = transform->TransformPoint (pt);
      }
      pt[0] = pt_n[0];
      pt[1] = pt_n[1];
      pt[2] = pt_n[2];
      
      points->SetPoint (i, pt);
      //new_colors->SetTuple (i, n_color);
      //colors->SetTuple (i, color);
    }
    

    //bundle->GetPointData()->SetScalars (colors);
    //new_bundle->GetPointData()->SetScalars ( new_colors );
    
    /*
      vtkAppendPolyData* appender = vtkAppendPolyData::New();
      appender->AddInput ( bundle );
      appender->AddInput ( new_bundle );
      appender->Update();
    */
    /*
      vtkPolyDataWriter* writer2 = vtkPolyDataWriter::New();
      writer2->SetFileName ( "bundle_fusion.vtk" );
      writer2->SetInput ( appender->GetOutput() );
      writer2->SetFileTypeToBinary();
      writer2->Write();
    */
    
    //bundle->SetPoints (new_points);
    vtkPolyDataWriter* writer = vtkPolyDataWriter::New();
    writer->SetFileName ( file_out );
    writer->SetInputData ( bundle );
    writer->SetFileTypeToBinary();
    writer->Write();
    
    /*
      new_bundle->Delete();
      new_colors->Delete();
      colors->Delete();
      appender->Delete();
      writer2->Delete();
    */
    
    reader->Delete();
    writer->Delete();
    
    return 0;
  }
  
}
