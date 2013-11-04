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
#include "itkNPTRemoverCommand.h"

#include "itkRemoveNonPositiveTensorsTensorImageFilter.h"
#include "itkTensorImageIO.h"
#include <itkImageFileReader.h>
#include <itkImage.h>

#include "GetPot.h"


namespace itk
{


  NPTRemoverCommand::NPTRemoverCommand()
  {
    m_ShortDescription = "Remove any non-positive tensors";
    m_LongDescription = "Usage: npt_remover\n";
    m_LongDescription += "<-i input> <-r 0/1: NPT removing type (0: null tensor / 1: mean of neighbors)> <-o output>\n\n";
    m_LongDescription += m_ShortDescription;
  }


  NPTRemoverCommand::~NPTRemoverCommand()
  {}
  

  int NPTRemoverCommand::Execute(int narg, const char *arg[])
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
    
    
    const char* fileIn = cl.follow("NoFile",2,"-i","-I");
    const char* fileOut = cl.follow("NoFile",2,"-o","-O");
    const int RemovingType = cl.follow (0,2,"-r","-R");
    
    typedef double ScalarType;  
    typedef itk::Image<ScalarType, 3>                      ImageType;
    typedef itk::TensorImageIO<ScalarType, 3, 3>           IOType;
    typedef IOType::TensorImageType                        TensorImageType;    
    typedef itk::RemoveNonPositiveTensorsTensorImageFilter<TensorImageType,TensorImageType>
      RemoveNPTFilterType;
    
    
    IOType::Pointer myIO = IOType::New();
    myIO->SetFileName (fileIn);
    try
    {
      myIO->Read();
    } catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    

    
    // now: filter
    RemoveNPTFilterType::Pointer myRemover = RemoveNPTFilterType::New();
    myRemover->SetInput ( myIO->GetOutput() );
    myRemover->SetRemovingType (RemovingType);
    std::cout << "Removing NPT..." << std::flush;
    try
    {
      myRemover->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;


    
    // write the output
    myIO->SetFileName(fileOut);
    myIO->SetInput (myRemover->GetOutput());
    
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
