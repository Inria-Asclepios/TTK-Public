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
#include "itkTensorsToDWICommand.h"

#include "itkTensorImageIO.h"
#include "itkTensorToScalarTensorImageFilter.h"
#include "itkTensorToL2NormFunction.h"
#include "itkTensorsToDWITensorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"
#include <itkImage.h>
#include <itkImageFileWriter.h>

#include <iostream>
#include "GetPot.h"


namespace itk
{

  TensorsToDWICommand::TensorsToDWICommand()
  {
    m_ShortDescription = "Create DWI from tensors using the Stejskal & Tanner diffusion equation (and a list of gradients)";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i input> <-b bvalue> <-m max value> <-g gradients> <-o output> <-e extension>\n\n";
    m_LongDescription += m_ShortDescription;
  }


  TensorsToDWICommand::~TensorsToDWICommand()
  {}


  int TensorsToDWICommand::Execute (int narg, const char* arg[])
  {
    
    // parse arguments
    GetPot cl (narg, const_cast<char**>(arg));
    if( cl.size() == 1 || cl.search (2,"--help","-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
    
    const char* file_in = cl.follow ("NoFile",2,"-i","-I");
    const double bvalue = cl.follow (1.0, 2, "-b", "-B");
    const char* gradients_file = cl.follow ("NoFile",2,"-g","-G");
    const char* extension= cl.follow ("mha", 2, "-e", "-E");
    const char* file_out= cl.follow ("NoFile", 2, "-o", "-O");
    int b0value = cl.follow (32767, 2, "-m", "-M");
    
    using ScalarType      = double;
    using TensorIOType    = itk::TensorImageIO<ScalarType, 3, 3>;
    using TensorType      = TensorIOType::TensorType;
    using TensorImageType = TensorIOType::TensorImageType;
    using ImageType       = itk::Image<double, 3>;
    using LightImageType  = itk::Image<unsigned short, 3>;
    
    std::cout << "Reading: " << file_in;
    std::cout << std::flush;
    
    // read in the tensor field
    TensorIOType::Pointer myIO = TensorIOType::New();
    myIO->SetFileName (file_in);
    try
    {
      myIO->Read();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    std::cout << " Done." << std::endl;
    
    TensorImageType::Pointer myTensorImage = myIO->GetOutput();

    // create a B0 image by taking the norm of the tensor field * scale:
    using TensorToScalarFilterType = itk::TensorToScalarTensorImageFilter<TensorImageType, ImageType>;    
    using TensorFunctionType = itk::TensorToL2NormFunction<TensorType, ScalarType>;

    TensorToScalarFilterType::Pointer myFilter1 = TensorToScalarFilterType::New();
    TensorFunctionType::Pointer myFunction = TensorFunctionType::New();
    myFilter1->SetTensorToScalarFunction ( myFunction );
    myFilter1->SetInput (myTensorImage);
    
    try
    {
      myFilter1->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    ImageType::Pointer myB0 = myFilter1->GetOutput();
    
    itk::RescaleIntensityImageFilter<ImageType, LightImageType>::Pointer rescaler=
      itk::RescaleIntensityImageFilter<ImageType, LightImageType>::New();
  
    rescaler->SetOutputMinimum ( 0 );
    rescaler->SetOutputMaximum ( b0value );
    rescaler->SetInput ( myB0 );
    try
    {
      rescaler->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }

    using TensorsToDWIFilter = itk::TensorsToDWITensorImageFilter<TensorImageType, LightImageType>;
    using GradientType       = TensorsToDWIFilter::GradientType;
    using GradientListType   = TensorsToDWIFilter::GradientListType;
  
    GradientListType myGradients;
    
    // read the fileGrad
    std::ifstream fileg (gradients_file, std::ifstream::in);
    if( fileg.fail() )
    {
      std::cerr << "Error: Cannot open " << gradients_file << " for reading." << std::endl;
      return -1;
    }
    
    // first: must be the number of gradients
    int numOfGrad = 0;
    fileg >> numOfGrad;
    
    for(int i=0; i<numOfGrad; i++)
    {
      GradientType g;
      fileg >> g[0];
      fileg >> g[1];
      fileg >> g[2];
      std::cout << g << std::endl;
      if (g[0]!=0.0 && g[1]!=0.0 && g[2]!=0.0)
      {
	g.Normalize();
      }
      myGradients.push_back ( g );
    }
    
    TensorsToDWIFilter::Pointer myFilter2 = TensorsToDWIFilter::New();
    myFilter2->SetBaselineImage (rescaler->GetOutput());
    myFilter2->SetBValue (bvalue);
    myFilter2->SetGradientList (myGradients);
    myFilter2->SetInput (myTensorImage);
    
    try
    {
      myFilter2->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    // save the results
    char filename[512];
    sprintf (filename, "%s%.2d.%s", file_out, 0, extension);
    std::cout << "Saving: " << filename;
    itk::ImageFileWriter<LightImageType>::Pointer writer0 = itk::ImageFileWriter<LightImageType>::New();
    writer0->SetFileName (filename);
    writer0->SetInput (rescaler->GetOutput());
    
    try
    {
      writer0->Write();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
    unsigned int numOutputs = myFilter2->GetNumberOfOutputs();
    for( unsigned int i=0; i<numOutputs; i++)
    {
      sprintf (filename, "%s%.2d.%s", file_out, i+1, extension);    
      std::cout << "Saving: " << filename;
      
      itk::ImageFileWriter<LightImageType>::Pointer writer = itk::ImageFileWriter<LightImageType>::New();
      writer->SetFileName (filename);
      writer->SetInput (myFilter2->GetOutput (i));
      
      try
      {
	writer->Write();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << " Done." << std::endl;
      
    }

    return 0;
  }
}
