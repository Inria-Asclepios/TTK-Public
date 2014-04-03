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
#include "itkAddGaussianNoiseToTensorsCommand.h"

#include "itkAddGaussianNoiseTensorImageFilter.h"
#include "itkTensorImageIO.h"
#include <itkImageFileReader.h>
#include <itkImage.h>

#include "GetPot.h"

namespace itk
{

  AddGaussianNoiseToTensorsCommand::AddGaussianNoiseToTensorsCommand()
  {
    m_ShortDescription = "Add Gaussian noise on the tensor logarithm and exponentiate the result";
    m_LongDescription = "Usage: gaussian_noise\n";
    m_LongDescription += "-i [tensor field]\n";
    m_LongDescription += "-v [variance]\n";
    m_LongDescription += "-o [Output file]\n\n";
    m_LongDescription += m_ShortDescription;
  }

  AddGaussianNoiseToTensorsCommand::~AddGaussianNoiseToTensorsCommand()
  {}


  int AddGaussianNoiseToTensorsCommand::Execute (int narg, const char *arg[])
  {

    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
  
    const bool IsInputPresent    = cl.search(2,"-i","-I");
    const bool IsOutputPresent   = cl.search(2,"-o","-O");
    
    if(!IsInputPresent || !IsOutputPresent )
    {
      std::cerr << "Error: Input and (or) output not set." << std::endl;
      return -1;
    }

    
    const char* fileIn   = cl.follow("NoFile",2,"-i","-I");
    const char* fileOut  = cl.follow("NoFile",2,"-o","-O");
    const double variance = cl.follow(1.0, 2,"-v","-V");
    
    typedef double ScalarType;  
    typedef itk::TensorImageIO<ScalarType, 3, 3>           IOType;
    typedef IOType::TensorImageType                        TensorImageType;    
    typedef itk::AddGaussianNoiseTensorImageFilter<TensorImageType,TensorImageType>
      FilterType;
    
    
    IOType::Pointer myIO = IOType::New();
    myIO->SetFileName (fileIn);
    try
    {
      myIO->Read();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
  
    FilterType::Pointer myFilter = FilterType::New();
    myFilter->SetInput ( myIO->GetOutput() );
    myFilter->SetVariance (variance);
    myFilter->SetNumberOfThreads (1);
    
    // now: filter
    std::cout << "Adding noise to tensors ..." << std::flush << std::endl;
    try
    {
      myFilter->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;
  
    // write the output
    myIO->SetFileName(fileOut);
    myIO->SetInput (myFilter->GetOutput());
    
    std::cout << "Writing..." << std::flush;
    try
    {
      myIO->Write();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;
    
    return 0;
    
  }
  
}
