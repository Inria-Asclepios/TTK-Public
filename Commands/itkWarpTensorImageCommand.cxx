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
#include "itkWarpTensorImageCommand.h"

#include "GetPot.h"
#include "itkTensorImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkWarpTensorImageFilter.h"
#include "itkLogTensorImageFilter.h"
#include "itkExpTensorImageFilter.h"
#include "itkTensorLinearInterpolateImageFunction.h"


namespace itk
{

  WarpTensorImageCommand::WarpTensorImageCommand()
  {
    m_ShortDescription = "Warp a tensor image given a deformation field";
    m_LongDescription = "Usage: warp\n";
    m_LongDescription += "<-i input> <-d deformation field> <-t reorientation strategy (1 for FS or 2 for PPD)> <-o output>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  WarpTensorImageCommand::~WarpTensorImageCommand()
  {}

  

  int WarpTensorImageCommand::Execute(int narg, const char* arg[])
  {

    //itk::Object::GlobalWarningDisplayOff();

    GetPot cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
  
    
    const char* input = cl.follow("NoFile",2,"-I","-i");
    const char* output = cl.follow("NoFile",2,"-O","-o");
    const char* def = cl.follow("NoFile",2,"-d","-D");
    int reostrat = cl.follow(0,2,"-t","-T");
  
    using ScalarType            = double; 
    using IOType                = itk::TensorImageIO<ScalarType, 3, 3>;
    using TensorImageType       = IOType::TensorImageType;
    using VectorType            = Vector<ScalarType, 3>;
    using DisplacementFieldType = Image<VectorType, 3>;
    using LogFilterType         = LogTensorImageFilter<TensorImageType,TensorImageType>;
    using ExpFilterType         = ExpTensorImageFilter<TensorImageType,TensorImageType>;


    TensorImageType::Pointer Input = nullptr;
    
    {
      std::cout << "Reading: " << input << std::flush;
      IOType::Pointer reader = IOType::New();
      reader->SetFileName( input );
      try
      {
	reader->Read();
      }
      catch( itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << " Done." << std::endl;
      
      Input = reader->GetOutput();
    }

      
    {
      std::cout << "Loging..." << std::endl;
      LogFilterType::Pointer loger = LogFilterType::New();
      loger->SetInput ( Input );
      try
      {
	loger->Update();
      }
      catch( itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << "Done." << std::endl;

      Input = loger->GetOutput();
      
    }
    


    DisplacementFieldType::Pointer Displacement = nullptr;
    {
      std::cout << "Reading: " << def << std::flush;
      itk::ImageFileReader<DisplacementFieldType>::Pointer reader3 = 
	itk::ImageFileReader<DisplacementFieldType>::New();
      reader3->SetFileName( def );
      try
      {
	reader3->Update();
      }
      catch( itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      
      Displacement = reader3->GetOutput();
      
      std::cout << " Done." << std::endl;
    }
    
    

    {
      
      using InterpolatorType = itk::TensorLinearInterpolateImageFunction<TensorImageType, double>;
      InterpolatorType::Pointer interpolator = InterpolatorType::New();
      interpolator->NormalizeOn();
    
    
      // warp the result
      using WarperType = itk::WarpTensorImageFilter < TensorImageType, TensorImageType, DisplacementFieldType >  ;
      WarperType::Pointer warper = WarperType::New();
      warper->SetInput( Input );
      warper->SetOutputSpacing( Displacement->GetSpacing() );
      warper->SetOutputOrigin( Displacement->GetOrigin() );
      warper->SetOutputDirection( Displacement->GetDirection() );
      warper->SetDisplacementField( Displacement );
      warper->SetInterpolator ( interpolator );
      warper->SetNumberOfWorkUnits(1);
      
      switch (reostrat)
      {
	  case 1:
	    warper->SetReorientationStrategy(FS);
	    std::cout << "Reorientation Strategy: Finite Strain (FS)" << std::endl;
	    break;
	    
	  case 2:
	    warper->SetReorientationStrategy(PPD);
	    std::cout << "Reorientation Strategy: Preservation of Principal Direction (PPD)" << std::endl;
	    break;
	    
	  default:
	    warper->SetReorientationStrategy(FS);
	    std::cout << "Reorientation Strategy: Finite Strain (FS)" << std::endl;
	    break;
      }
    
      std::cout << "Warping: " << std::flush;
      try
      {
	warper->Update();
      }
      catch( itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << " Done." << std::endl;

      Input = warper->GetOutput();

    }
    


    
    {
      std::cout << "Exping..." << std::endl;
      ExpFilterType::Pointer exper = ExpFilterType::New();
      exper->SetInput ( Input );
      try
      {
	exper->Update();
      }
      catch( itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << " Done." << std::endl;

      Input = exper->GetOutput();
    }
    

    
    std::cout << "Writing: " << output << std::flush;
    IOType::Pointer writer = IOType::New();
    writer->SetInput( Input );
    writer->SetFileName( output );
    
    try
    {
      writer->Write();
    }
    catch( itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
    return 0;
    
  }

} // end of namespace

