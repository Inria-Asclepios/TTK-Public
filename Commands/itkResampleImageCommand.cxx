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
#include "itkResampleImageCommand.h"

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkResampleImageFilter.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkImage.h>
#include <itkAffineTransform.h>
#include <itkTransformFileReader.h>
#include <itkTransformFactory.h>
#include <itkMatrixOffsetTransformBase.h>

#include "ttkConfigure.h"

#ifdef TTK_USE_MIPS
#include "mipsInrimageImageIOFactory.h"
#endif

#include <iostream>
#include "GetPot.h"

namespace itk
{

  ResampleImageCommand::ResampleImageCommand()
  {
    m_ShortDescription = "Resample an image given a linear transform and a reference image";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "-i  [Image File]\n";
    m_LongDescription += "-r  [Reference image file]\n";
    m_LongDescription += "-m  [matrix file]\n";
    m_LongDescription += "-sx [double]\n";
    m_LongDescription += "-sy [double]\n";
    m_LongDescription += "-sz [double]\n";
    m_LongDescription += "-o  [Output File Name]\n\n";
    m_LongDescription += m_ShortDescription;
  }
  
  ResampleImageCommand::~ResampleImageCommand()
  {}

  int ResampleImageCommand::Execute(int narg, const char* arg[])
  {

    itk::Object::GlobalWarningDisplayOff();

#ifdef TTK_USE_MIPS	
    itk::InrimageImageIOFactory::RegisterOneFactory();
#endif
    
    
    GetPot cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
    
    const char* input = cl.follow("NoFile",2,"-I","-i");
    const char* output = cl.follow("NoFile",2,"-O","-o");
    const char* mat    = cl.follow( "NoFile", 2, "-m", "-M");
    const char* ref    = cl.follow( "NoFile", 2, "-r", "-R");
    const int sx = cl.follow(-1,2,"-SX","-sx");
    const int sy = cl.follow(-1,2,"-SY","-sy");
    const int sz = cl.follow(-1,2,"-SZ","-sz");
    
    typedef double ScalarType;
    typedef itk::MatrixOffsetTransformBase< ScalarType, 3 ,3 >  TransformType;
    typedef Image<ScalarType, 3>            ImageType;    
    typedef itk::ImageFileReader<ImageType> ImageReaderType;
    typedef itk::ImageFileWriter<ImageType> WriterType;
    typedef ImageType::SizeType             SizeType;
    typedef ImageType::SpacingType          SpacingType;
    typedef ImageType::PointType            PointType;
    typedef ImageType::DirectionType        DirectionType;
    
    ImageReaderType::Pointer reader = ImageReaderType::New();
    reader->SetFileName( input );
    
    std::cout << "Reading: " << input << std::flush;
    try
    {
      reader->Update();
    }
    catch( itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;

    ImageType::Pointer inputimage = reader->GetOutput();
    ImageType::Pointer reference = 0;
    if (strcmp (ref, "NoFile"))
    {
      
      ImageReaderType::Pointer reader2 = ImageReaderType::New();
      reader2->SetFileName( ref );
      
      std::cout << "Reading: " << ref << std::flush;
      try
      {
	reader2->Update();
      }
      catch( itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << " Done." << std::endl;
      reference = reader2->GetOutput();
    }
    
    // read the affine matrix
    TransformType::Pointer transform = 0;
    if (strcmp (mat, "NoFile"))
    {
      std::cout << "Reading: " << mat;
      itk::TransformFactory< TransformType >::RegisterTransform ();
      
      typedef itk::TransformFileReader TransformReaderType;
      TransformReaderType::Pointer reader_t = TransformReaderType::New();
      reader_t->SetFileName ( mat );
      try
      {
	reader_t->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      transform = dynamic_cast<TransformType*>( reader_t->GetTransformList()->front().GetPointer() );
      std::cout << " Done." << std::endl;
    }

    
    SpacingType spacing;
    PointType origin;
    SizeType size;
    DirectionType direction;
    
    if (strcmp (ref, "NoFile"))
    {
      spacing   = reference->GetSpacing();
      origin    = reference->GetOrigin();
      size      = reference->GetLargestPossibleRegion().GetSize();
      direction = reference->GetDirection();
    }
    else if ( (sx != -1) && (sy != -1) && (sz != -1) )
    {
      spacing   = inputimage->GetSpacing();
      origin    = inputimage->GetOrigin();
      size      = inputimage->GetLargestPossibleRegion().GetSize();
      direction = inputimage->GetDirection();
      
      spacing[0] /= (double)(sx) / (double)(size[0]);
      spacing[1] /= (double)(sy) / (double)(size[1]);
      spacing[2] /= (double)(sz) / (double)(size[2]);
      
      size[0] = sx;
      size[1] = sy;
      size[2] = sz;    
    }
    else
    {
      spacing   = inputimage->GetSpacing();
      origin    = inputimage->GetOrigin();
      size      = inputimage->GetLargestPossibleRegion().GetSize();
      direction = inputimage->GetDirection();
    }
    
    typedef itk::ResampleImageFilter<ImageType, ImageType> FilterType;
    FilterType::Pointer filter = FilterType::New();
    
    typedef itk::LinearInterpolateImageFunction<ImageType, ScalarType>  InterpolatorType;
    InterpolatorType::Pointer interpolator = InterpolatorType::New();
    
    filter->SetInterpolator( interpolator );
    filter->SetInput ( inputimage );
    filter->SetOutputOrigin( origin );
    filter->SetOutputSpacing( spacing );
    filter->SetOutputDirection (direction);
    filter->SetSize( size );
    
    if(transform)
      filter->SetTransform( transform );
    
    std::cout << "Resampling...";
    try
    {
      filter->Update();
    }
    catch (itk::ExceptionObject &e )
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;

    
    WriterType::Pointer writer = WriterType::New();
    writer->SetInput( filter->GetOutput() );
    writer->SetFileName( output );
    
    std::cout << "Writing: " << output;
    try
    {
      writer->Update();
    }
    catch (itk::ExceptionObject &e )
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
    return 0;
  }
  
}
