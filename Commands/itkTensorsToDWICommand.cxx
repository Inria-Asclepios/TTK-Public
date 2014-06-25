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
#include <itkImageFileReader.h>

#include <itkGradientFileReader.h>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "GetPot.h"


namespace itk
{
  
  
  TensorsToDWICommand::TensorsToDWICommand()
  {
    m_ShortDescription = "Create DWI from tensors using the Stejskal & Tanner diffusion equation (and a list of gradients)";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i input> <-x b0image> <-m max B0 value if B0 image unavailable> <-g gradients (bvec file)> <-b b-values (bval file) ><-o output> <-e extension>\n\n";
    m_LongDescription += m_ShortDescription;
  }


  TensorsToDWICommand::~TensorsToDWICommand()
  {}


  float TensorsToDWICommand::ReadBvalueFromBvalFile (const char* filename)
  {
    float ret = 0;
    
    std::ifstream in;
    in.open ( filename, std::ios::in | std::ios::binary );
    if( in.fail() )
    {
      in.close();
      itkExceptionMacro ( "The file could not be opened for read access "
			  << std::endl << "Filename: \"" << filename << "\"" );
    }

    std::ostringstream InData;
    // in.get ( InData );
    std::filebuf *pbuf;
    pbuf=in.rdbuf();
    
    // get file size using buffer's members
    int size=static_cast<int>(pbuf->pubseekoff (0,std::ios::end,std::ios::in));
    pbuf->pubseekpos (0,std::ios::in);
    
    // allocate memory to contain file data
    char* buffer=new char[size+1];
    
    // get file data
    pbuf->sgetn (buffer,size);
    buffer[size]='\0';
    itkDebugMacro ( "Read file gradient Data" );
    InData << buffer;
    
    delete[] buffer;
    std::string data = InData.str();
    in.close();

    std::istringstream stream(data);
    std::string item;
    char delim = ' ';
    std::vector<float> bvals;

    while (std::getline(stream, item, delim))
    {
      float bval = std::atof (item.c_str());
      if (bval > 0.01)
      {
	bvals.push_back(bval);
	std::cout << "Found b-factor of " << bval << std::endl;
      }
    }

    if (!bvals.empty())
      ret = bvals[0];
    
    return ret;
  }
  
  
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
    const char* gradients_file = cl.follow ("NoFile",2,"-g","-G");
    const char* bval_file = cl.follow ("NoFile",2,"-b","-B");
    const char* b0_file = cl.follow ("NoFile",2,"-x","-X");
    const char* extension= cl.follow ("mha", 2, "-e", "-E");
    const char* file_out= cl.follow ("NoFile", 2, "-o", "-O");
    int b0value = cl.follow (32767, 2, "-m", "-M");
    
    typedef double                               ScalarType;
    typedef itk::TensorImageIO<ScalarType, 3, 3> TensorIOType;
    typedef TensorIOType::TensorType             TensorType;
    typedef TensorIOType::TensorImageType        TensorImageType;
    typedef itk::Image<double, 3>                ImageType;
    typedef itk::Image<unsigned short, 3>        LightImageType;
    typedef itk::ImageFileReader<ImageType>      ReaderType;
    
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


    ImageType::Pointer myB0 = NULL;

    const bool IsB0Present       = cl.search(2,"-x","-X");

    if (IsB0Present)
    {
      std::cout<<"reading "<<b0_file<<"..."<<std::flush;
      ReaderType::Pointer imreader = ReaderType::New();
      imreader->SetFileName (b0_file);
      try
      {
	
	imreader->Update();
      } catch (itk::ExceptionObject &e)
      {
    	std::cerr << e;
    	return EXIT_FAILURE;
      }
      
      myB0 = imreader->GetOutput();
      std::cout<<"\t OK."<<std::endl;
    } else {
      
      // create a B0 image by taking the norm of the tensor field * scale:
      typedef itk::TensorToScalarTensorImageFilter<TensorImageType, ImageType>
	TensorToScalarFilterType;
      
      typedef itk::TensorToL2NormFunction<TensorType, ScalarType>
	TensorFunctionType;
      
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
      
      myB0 = myFilter1->GetOutput();
      
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
    }
    
    // typedef itk::TensorsToDWITensorImageFilter<TensorImageType, LightImageType>
    //   TensorsToDWIFilter;
    typedef itk::TensorsToDWITensorImageFilter<TensorImageType, ImageType>
      TensorsToDWIFilter;
    typedef TensorsToDWIFilter::GradientType      GradientType;
    typedef TensorsToDWIFilter::GradientListType  GradientListType;
  
    GradientListType myGradients;

    typedef itk::GradientFileReader GradientReaderType;
    GradientReaderType::Pointer gradientreader = GradientReaderType::New();
    
    gradientreader->SetFileName (gradients_file);
    gradientreader->Update();
    myGradients = gradientreader->GetGradientList();
    
    // for
    // read the file bval
    float bvalue = ReadBvalueFromBvalFile (bval_file);    

    std::cout<<"number of gradients in list: "<<myGradients.size()<<std::endl;
    
    TensorsToDWIFilter::Pointer myFilter2 = TensorsToDWIFilter::New();
    myFilter2->SetBaselineImage (myB0);
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

    unsigned int numOutputs = myFilter2->GetNumberOfOutputs();
    for( unsigned int i=0; i<numOutputs; i++)
    {
      sprintf (filename, "%s%.3d.%s", file_out, i, extension);    
      std::cout << "Saving: " << filename;
      
      itk::ImageFileWriter<ImageType>::Pointer writer = itk::ImageFileWriter<ImageType>::New();
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
