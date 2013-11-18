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
#include "itkResampleTensorImageCommand.h"

#include "itkLogTensorImageFilter.h"
#include "itkExpTensorImageFilter.h"
#include "itkResampleTensorImageFilter.h"
#include "itkAffineTensorTransform.h"
#include "itkTensorLinearInterpolateImageFunction.h"
#include "itkTensorImageIO.h"
#include <itkTransformFileReader.h>
#include <itkTransformFactory.h>
#include <itkMatrixOffsetTransformBase.h>
#include <itkAffineTransform.h>

#include "GetPot.h"

#include "ttkConfigure.h"

#ifdef TTK_USE_MIPS
#include "mipsInrimageImageIOFactory.h"
#endif


namespace itk
{

  ResampleTensorImageCommand::ResampleTensorImageCommand()
  {
    m_ShortDescription = "Resample tensors w.r.t. a linear matrix and a reference image";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i input> <-m matrix> <-r file> <-l 0/1 (log-Euclidean on/off)> <-t threads> <-o output>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  ResampleTensorImageCommand::~ResampleTensorImageCommand()
  {}

  int ResampleTensorImageCommand::Execute(int argc, const char* argv[])
  {
    
    GetPot cl(argc, const_cast<char**>(argv)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
    
    const bool IsInputPresent = cl.search(2,"-I","-i");
    const bool IsOutputPresent = cl.search(2,"-O","-o");

    if( !IsInputPresent || !IsOutputPresent )
    {
      std::cerr << "Error: Input and (or) output not set." << std::endl;
      return -1;
    }
  
    const char* tensorFile = cl.follow("NoFile",2,"-I","-i");
    const char* outFile = cl.follow("NoFile",2,"-O","-o");
    const char* mat = cl.follow("NoFile",2,"-m","-M");
    const char* ref = cl.follow("NoFile",2,"-r","-R");
    //const bool bal = cl.follow (false, 2, "-b", "-B");
    const bool le = cl.follow (1,2,"-l","-L");
    const int threads = cl.follow(1,2,"-t","-T");
    

    
    typedef double                                ScalarType;  
    typedef itk::TensorImageIO<ScalarType, 3, 3>  IOType;
    typedef IOType::TensorImageType               TensorImageType;
    typedef itk::ResampleTensorImageFilter<TensorImageType,TensorImageType> FilterType;
  
    IOType::Pointer myIO = IOType::New();
    myIO->SetFileName(tensorFile);
    
    std::cout << "Reading: " << tensorFile << std::endl;
    try
    {
      myIO->Read();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e << std::endl;
      return -1;
    }
  
    TensorImageType::Pointer tensors = myIO->GetOutput();
    
    
    // log:
    if( le )
    {
      typedef itk::LogTensorImageFilter<TensorImageType, TensorImageType> LogFilterType;
      LogFilterType::Pointer myLog = LogFilterType::New();
      myLog->SetInput(tensors);
      myLog->SetNumberOfThreads(threads);
      
      try
      {
	myLog->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e << std::endl;
	return -1;
      }
      
      tensors = myLog->GetOutput();
    }
    
    
    // read the affine matrix
    std::cout << "Reading: " << mat;
    typedef itk::AffineTensorTransform< ScalarType, 3 >         TensorTransformType;
    typedef itk::MatrixOffsetTransformBase< ScalarType, 3 ,3 >  TransformType;
    typedef itk::AffineTransform< ScalarType, 3>                AffineTransformType;
    
    /*
    // read the affine matrix
    std::ifstream buffer (mat);
    if( buffer.fail() )
    {
    std::cerr << "Error: Cannot read file " << mat << std::endl;
    return -1;
    }
    
    
    TransformType::MatrixType       matrix;
    TransformType::OutputVectorType translation;
    
    if( bal ) // baladin's style
    {
    // skip 2 first characters
    char junk[512];
    buffer >> junk;
    buffer >> junk;
    
    for( unsigned int i=0 ;i<3; i++)
    {
    buffer >> matrix (i,0);
    buffer >> matrix (i,1);
    buffer >> matrix (i,2);
    buffer >> translation[i];
    }
    transform->SetMatrix (matrix);
    transform->SetTranslation (translation);
    
    TransformType::Pointer inv_transform = TransformType::New();
    transform->GetInverse(inv_transform);
    
    transform = inv_transform;
    }
    else
    {  
    
    // skip the first 12 floats
    char junk [512];
    for( unsigned int i=0; i<12; i++)
    {
    buffer >> junk;
    }
    
    for( unsigned int i=0 ;i<3; i++)
    {
      buffer >> matrix (i,0);
      buffer >> matrix (i,1);
      buffer >> matrix (i,2);
      }
      
    for( unsigned int i=0; i<3; i++)
    {
    buffer >> translation[i];
    }
    
    transform->SetMatrix (matrix);
    transform->SetTranslation (translation);
    
    TransformType::Pointer inv_transform = TransformType::New();
    transform->GetInverse(inv_transform);
    
    transform = inv_transform;
    }
    buffer.close();
    
    std::cout << "Matrix is: " << std::endl;
    std::cout << matrix << std::endl;
    std::cout << "Translation is: " << std::endl;
    std::cout << translation << std::endl;
    
    std::cout << transform << std::endl;
    */
    
    
    TransformType::Pointer transform = 0;
    {
      itk::TransformFactory< TransformType >::RegisterTransform ();
      itk::TransformFactory< AffineTransformType >::RegisterTransform ();
      
      typedef itk::TransformFileReader TransformReaderType;
      TransformReaderType::Pointer reader = TransformReaderType::New();
      reader->SetFileName ( mat );
      try
      {
	reader->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      transform = dynamic_cast<TransformType*>( reader->GetTransformList()->front().GetPointer() );
    }
    
    std::cout << " Done." << std::endl;
    

    TensorTransformType::Pointer tensor_transform = TensorTransformType::New();
    tensor_transform->SetMatrix ( transform->GetMatrix() );
    tensor_transform->SetTranslation ( transform->GetTranslation() );
    
    
    FilterType::Pointer filter = FilterType::New();
    
    typedef itk::TensorLinearInterpolateImageFunction<TensorImageType, double>  InterpolatorType;
    InterpolatorType::Pointer interpolator = InterpolatorType::New();

    
    filter->SetTensorInterpolator( interpolator );
    filter->SetInput ( tensors );
    filter->SetNumberOfThreads(threads);
    //filter->SetUseReferenceImage (true);
    //filter->SetReferenceImage ( myIO->GetOutput() );
    
#ifdef TTK_USE_MIPS
    itk::InrimageImageIOFactory::RegisterOneFactory();
#endif
    
    typedef itk::Image<double, 3> ImageType;
    itk::ImageFileReader< ImageType >::Pointer io2 = itk::ImageFileReader< ImageType >::New();
    io2->SetFileName( ref );
    try
    {
      io2->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    
    ImageType::Pointer reference = io2->GetOutput();
    ImageType::SpacingType spacing = reference->GetSpacing();
    ImageType::PointType   origin  = reference->GetOrigin();
    ImageType::DirectionType  direction  = reference->GetDirection();
    ImageType::SizeType size =   reference->GetLargestPossibleRegion().GetSize();
    filter->SetOutputOrigin( origin );
    filter->SetOutputSpacing( spacing );
    filter->SetOutputDirection( direction );
    filter->SetSize( size );
    
    /*
      TransformType::OutputVectorType translation1;
      const double imageCenterX = origin[0] + spacing[0] * size[0] / 2.0;
      const double imageCenterY = origin[1] + spacing[1] * size[1] / 2.0;
      const double imageCenterZ = origin[0] + spacing[0] * size[0] / 2.0;
      
      translation1[0] =   -imageCenterX;
      translation1[1] =   -imageCenterY;
      translation1[2] =   -imageCenterZ;
      
      transform->Translate( translation1 );
      
      const double degreesToRadians = atan(1.0) / 45.0;
      const double angle = degree * degreesToRadians;
      
      TransformType::OutputVectorType vec;
      vec[0] = 0.0;
      vec[1] = 0.0;
      vec[2] = 0.1;  
      transform->Rotate3D(vec, -angle, false );
      
      
      TransformType::OutputVectorType translation2;
      translation2[0] =   imageCenterX;
      translation2[1] =   imageCenterY;
      translation2[2] =   imageCenterZ;
      transform->Translate( translation2, false );
    */
    
    
    filter->SetTensorTransform( tensor_transform );
    
    try
    {
      filter->Update();
    }
    catch (itk::ExceptionObject &e )
    {
      std::cerr << e;
      return -1;
    }
    
    tensors = filter->GetOutput();
    
    if (le )
    {
      // exp:
      typedef itk::ExpTensorImageFilter<TensorImageType, TensorImageType> ExpFilterType;
      ExpFilterType::Pointer myExp = ExpFilterType::New();
      
      myExp->SetInput( filter->GetOutput() );
      myExp->SetNumberOfThreads(threads);
      
      std::cout << "Pipeline started." << std::endl;
      std::cout << std::flush;
      try
      {
	myExp->Update();
      }
      catch(itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << "Pipeline finished." << std::endl;
      
      tensors = myExp->GetOutput();
    }
    
    
    myIO->SetFileName(outFile);
    myIO->SetInput( tensors );
    
    std::cout << "Writing: " << outFile << std::flush;
    try
    {
      myIO->Write();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e << std::endl;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
    return 0;
    
  }
  
}
