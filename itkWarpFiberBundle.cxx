#include <iostream>
#include "GetPot.h"

#include <itkVector.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkInterpolateImageFilter.h>
#include <itkVectorLinearInterpolateImageFunction.h>
#include <itkTransformFileReader.h>
#include <itkMatrixOffsetTransformBase.h>
#include <itkTransformFactory.h>

#include <vtkPolyData.h>
#include <vtkAppendPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkUnsignedCharArray.h>

void PrintHelp (const char* exec)
{
  std::cout << "Usage:\n";
  std::cout << exec << "\n";
  std::cout << "<-i input>\n";
  std::cout << "<-v vector field> or <-m matrix>\n";
  std::cout << "<-o output>" << std::endl;
}




int main (int narg, char* arg[])
{

  GetPot   cl(narg, arg); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") )
  {
    PrintHelp(cl[0]);
    return -1;
  }


  const char* file_in     = cl.follow ("",2,"-i","-I");
  const char* file_out    = cl.follow ("",2,"-o","-O");
  const char* file_vector = cl.follow ("",2,"-v","-V");
  const char* file_matrix = cl.follow ("",2,"-m","-M");

  
  typedef itk::Vector<double, 3>                VectorType;
  typedef itk::Image<VectorType, 3>             VectorImageType;
  typedef itk::ImageFileReader<VectorImageType> ReaderType;
  typedef itk::InterpolateImageFilter<VectorImageType, VectorImageType>      InterpolateFilterType;
  typedef itk::VectorLinearInterpolateImageFunction<VectorImageType, double> InterpolateFunctionType;
  

  vtkPolyDataReader* reader = vtkPolyDataReader::New();
  reader->SetFileName (file_in);

  vtkPolyData* bundle = reader->GetOutput();
  bundle->Update();

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

  LinearTransformType::Pointer transform = 0;
  if( strcmp (file_matrix, "")!=0 )
  {
    itk::TransformFactory< LinearTransformType >::RegisterTransform ();
  
    typedef itk::TransformFileReader TransformReaderType;
    TransformReaderType::Pointer reader = TransformReaderType::New();
    reader->SetFileName ( file_matrix );
    try
    {
      reader->Update();
    }
    catch (itk::ExceptionObject &e)
    {
    std::cerr << e;
    return -1;
    }
    transform = dynamic_cast<LinearTransformType*>( reader->GetTransformList()->front().GetPointer() );
  }
  
  

  vtkPolyData* new_bundle = vtkPolyData::New();
  new_bundle->DeepCopy (bundle);
  

  //vtkPoints* points = bundle->GetPoints();
  vtkPoints* new_points = new_bundle->GetPoints();
  vtkUnsignedCharArray* new_colors = vtkUnsignedCharArray::New();
  new_colors->SetNumberOfComponents( 4 );
  new_colors->SetNumberOfTuples(new_points->GetNumberOfPoints());

  vtkUnsignedCharArray* colors = vtkUnsignedCharArray::New();
  colors->SetNumberOfComponents( 4 );
  colors->SetNumberOfTuples(new_points->GetNumberOfPoints());

  double n_color[4] = {0,0,255,255};
  double color[4] = {255,0,0,255};
  for( int i=0; i<new_points->GetNumberOfPoints(); i++ )
  {
    double pt[3];
    new_points->GetPoint ( i, pt);
    
    VectorImageType::PointType pt_i;
    pt_i[0] = pt[0];
    pt_i[1] = pt[1];
    pt_i[2] = pt[2];


    VectorImageType::PointType pt_n;
    

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
    
    new_points->SetPoint (i, pt);
    new_colors->SetTuple (i, n_color);
    colors->SetTuple (i, color);
  }
  

  bundle->GetPointData()->SetScalars (colors);
  new_bundle->GetPointData()->SetScalars ( new_colors );

  vtkAppendPolyData* appender = vtkAppendPolyData::New();
  appender->AddInput ( bundle );
  appender->AddInput ( new_bundle );
  appender->Update();


  vtkPolyDataWriter* writer2 = vtkPolyDataWriter::New();
  writer2->SetFileName ( "bundle_fusion.vtk" );
  writer2->SetInput ( appender->GetOutput() );
  writer2->Write();


  bundle->SetPoints (new_points);
  vtkPolyDataWriter* writer = vtkPolyDataWriter::New();
  writer->SetFileName ( file_out );
  writer->SetInput ( bundle );
  writer->Write();
  

  new_bundle->Delete();
  new_colors->Delete();
  colors->Delete();
  reader->Delete();
  writer->Delete();
  appender->Delete();
  writer2->Delete();
  
  return 0;
}
