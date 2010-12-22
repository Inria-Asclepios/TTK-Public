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
#include <iostream>
#include <fstream>

#include <itkVector.h>
#include <itkImage.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkImageFileWriter.h>

void PrintHelp (const char* exec)
{
  std::cout << "Usage:\n";
  std::cout << exec << " <input> <output>" << std::endl;
}


int main (int narg, char* arg[])
{

  if( narg <3 )
  {
    PrintHelp (arg[0]);
    return -1;
  }


  typedef itk::Vector<double, 3>    VectorType;
  typedef itk::Image<VectorType, 3> VectorImageType;
  

  char* file_input  = arg[1];
  char* file_output = arg[2];


  std::ifstream buffer_in (file_input);
  if( buffer_in.fail() )
  {
    std::cerr << "Error: cannot open file for reading." << std::endl;
    return -1;
  }


  char data[256];
  buffer_in >> data; // Offset
  buffer_in >> data; // =
  double origin[3];
  for( int i=0; i<3; i++)
    buffer_in >> origin[i];

  
  buffer_in >> data; // ElementSpacing
  buffer_in >> data; // =
  double spacing[3];
  for( int i=0; i<3; i++)
    buffer_in >> spacing[i];

  
  buffer_in >> data; // DimSize
  buffer_in >> data; // =
  int dims[3];
  for( int i=0; i<3; i++)
    buffer_in >> dims[i];
 
  VectorImageType::SizeType size = {{dims[0], dims[1], dims[2]}};
  VectorImageType::RegionType region;
  region.SetSize (size);
  VectorImageType::IndexType index = {{0, 0, 0}};
  region.SetIndex ( index );

  VectorImageType::PointType p_origin;
  for( int i=0; i<3; i++)
    p_origin[i] = origin[i];

  VectorImageType::SpacingType p_spacing;
  for( int i=0; i<3; i++)
    p_spacing[i] = spacing[i];
  
  
  VectorImageType::Pointer image = VectorImageType::New();
  image->SetOrigin( p_origin );
  image->SetSpacing( p_spacing );
  image->SetRegions ( region );
  image->Allocate();

  itk::ImageRegionIteratorWithIndex<VectorImageType> it (image, image->GetLargestPossibleRegion() );
  
  
  int num_voxels = dims[0]*dims[1]*dims[2];
  int vox_count = 0;
  
  while( !buffer_in.eof() )
  {

    if( it.IsAtEnd() )
    {
      break;
    }
    
    VectorImageType::PointType pt_i;
    for( int i=0; i<3; i++)
      buffer_in >> pt_i[i];


    index = it.GetIndex();
    VectorImageType::PointType pt;
    image->TransformIndexToPhysicalPoint (index, pt);

        
    VectorType vec = pt_i - pt;
    it.Set ( vec );
    
    ++it;
    ++vox_count;
  }
  

  std::cout << "Total read: " << vox_count << " over " << num_voxels << std::endl;


  typedef itk::ImageFileWriter<VectorImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName (file_output);
  writer->SetInput (image);
  writer->Update();
  
  
  
}
