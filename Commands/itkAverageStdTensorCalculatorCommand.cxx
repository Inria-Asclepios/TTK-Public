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
#include "itkAverageStdTensorCalculatorCommand.h"

#include "itkTensorImageIO.h"
#include "itkAverageStdTensorImageFilter.h"

#include <iostream>
#include "GetPot.h"

namespace itk
{


  AverageStdTensorCalculatorCommand::AverageStdTensorCalculatorCommand()
  {
    m_ShortDescription = "Calculate the average and covariance matrix of a set of tensor fields";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "-i <file list of tensor fields> -o <output file (without extension - nii.gz will be produced)>\n\n";
    m_LongDescription += m_ShortDescription;
  }
  
  AverageStdTensorCalculatorCommand::~AverageStdTensorCalculatorCommand()
  {}

  int AverageStdTensorCalculatorCommand::Execute (int narg, const char* arg[])
  {

    itk::Object::GlobalWarningDisplayOff();
    
    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
    
    const bool IsInputPresent    = cl.search(2,"-i","-I");
    const bool IsOutputPresent   = cl.search(2,"-o","-O");
    
    if(!IsInputPresent || !IsOutputPresent)
    {
      std::cerr << "Error: Input and (or) output not set." << std::endl;
      return -1;
    }

    const char* fileIn = cl.follow("NoFile",2,"-i","-I");
    const char* fileOut = cl.follow("NoFile",2,"-o","-O");
  
  
    using ScalarType = double ;
    
    using TensorIOType       = itk::TensorImageIO<ScalarType, 3, 3>;
    using TensorImageType    = TensorIOType::TensorImageType;
    using CovMatrixIOType    = itk::TensorImageIO<ScalarType, 6, 3>;
    using CovMatrixImageType = CovMatrixIOType::TensorImageType;
    
    using AverageFilterType  = itk::AverageStdTensorImageFilter<TensorImageType, TensorImageType>;
    
    TensorImageType::Pointer mean = nullptr;
    CovMatrixImageType::Pointer covMatrix = nullptr;
    
    {
      AverageFilterType::Pointer myAverage = AverageFilterType::New();
      
      // read the fileIn
      std::ifstream file(fileIn,std::ifstream::in);
      // first number must be the num. of images:
      int numOfImages=0;
      file >> numOfImages;
      std::cout << "Number of Images: " << numOfImages << std::endl;
      
      for(int i=0;i<numOfImages;i++)
      {
	char filename[256];
	file >> filename;
	TensorIOType::Pointer myReader = TensorIOType::New();
	myReader->SetFileName(filename);
	std::cout << "Reading " << filename << std::flush;
	try
	{
	  myReader->Read();
	}
	catch(itk::ExceptionObject &e)
	{
	  std::cerr << e;
	  return -1;
	}
	std::cout << " Done." << std::endl;
	myAverage->SetInput(i, myReader->GetOutput());
      }
      
      
      std::cout << "Averaging..." << std::flush;
      try
      {
	myAverage->Update();
      }
      catch(itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      std::cout << "Done." << std::endl;
      
      mean = myAverage->GetOutput();
      covMatrix = myAverage->GetCovarianceMatrixImage();
      
    }
    
    std::cout << "Writing resuts..." << std::flush;
    std::string s_output = fileOut;
    {
      TensorIOType::Pointer writer1 = TensorIOType::New();
      writer1->SetInput( mean );
      std::string mean_output = s_output + "_mean.nii.gz";
      writer1->SetFileName( mean_output.c_str() );
      try
      {
	writer1->Write();
      }
      catch(itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
    }
    
    
    {
      CovMatrixIOType::Pointer writer2 = CovMatrixIOType::New();
      writer2->SetInput( covMatrix );
      std::string std_output = s_output + "_covariance_matrix.inr.gz";
      writer2->SetFileName( std_output.c_str() );
      try
      {
	writer2->Write();
      }
      catch( itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
    }
    std::cout << "Done." << std::endl;
    
    return 0;
  }
  
}
