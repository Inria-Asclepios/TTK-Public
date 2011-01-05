/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: svn+ssh://ntoussaint@scm.gforge.inria.fr/svn/ttk/trunk/Common/itkAffineTensorTransform.h $
  Language:  C++
  Date:      $Date: 2010-06-07 12:39:13 +0100 (Mon, 07 Jun 2010) $
  Version:   $Revision: 68 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkAddGaussianNoiseImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

int main(int argc, char* argv[])
{

  typedef itk::Image<double, 3> ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageFileWriter<ImageType> WriterType;
  typedef itk::AddGaussianNoiseImageFilter<ImageType,ImageType> FilterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const char* filein = argv[1];
  const char* fileout = argv[2];
  const float variance = std::atof (argv[3]);
  
  std::cout<<"reading input image : "<<filein<<std::endl;
  reader->SetFileName (filein);
  
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput (reader->GetOutput());
  filter->SetVariance (variance);
  
  std::cout<<"writing output image : "<<fileout<<std::endl;
  writer->SetInput (filter->GetOutput());
  writer->SetFileName (fileout);

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
