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
#include "itkScaleTensorCommand.h"

#include "itkScaleTensorImageFilter.h"
#include "itkTensorImageIO.h"
#include <itkImageFileReader.h>
#include <itkImage.h>

#include "GetPot.h"

namespace itk
{

  ScaleTensorCommand::ScaleTensorCommand()
  {
    m_ShortDescription = "Scale tensors by a given factor";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "-i <input>\n";
    m_LongDescription += "-s <scaling value>\n";
    m_LongDescription += "-l <use log-scale>\n";
    m_LongDescription += "-o <output>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  ScaleTensorCommand::~ScaleTensorCommand()
  {}

  int ScaleTensorCommand::Execute(int narg, const char* arg[])
  {
    
    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
    
    const char* fileIn   = cl.follow("",2,"-i","-I");
    const char* fileOut  = cl.follow("",2,"-o","-O");
    double scaling       = cl.follow (1.0, 2, "-s", "-S");
    bool useLogScale     = cl.search (2,"-l","-L");


    typedef double ScalarType;  
    typedef itk::Image<ScalarType, 3>                      ImageType;
    typedef itk::TensorImageIO<ScalarType, 3, 3>           IOType;
    typedef IOType::TensorImageType                        TensorImageType;    
    typedef itk::ScaleTensorImageFilter<TensorImageType, TensorImageType>
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
    myFilter->SetScalingFactor ( scaling );
    myFilter->SetUseLogScale ( useLogScale );
    
    // now: filter
    try
    {
      myFilter->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    
    
    // write the output
    myIO->SetFileName(fileOut);
    myIO->SetInput ( myFilter->GetOutput() );
    
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
