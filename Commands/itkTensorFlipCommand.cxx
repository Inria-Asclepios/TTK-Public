/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL:$
  Language:  C++
  Date:      $Date:$
  Version:   $Revision:$

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkTensorFlipCommand.h"

#include "itkFlipTensorImageFilter.h"
#include "itkTensorImageIO.h"
#include <itkImageFileReader.h>
#include <itkImage.h>

#include "GetPot.h"


namespace itk
{
  
  TensorFlipCommand::TensorFlipCommand()
  {
    m_ShortDescription = "Flip tensors w.r.t. x, y or z axis";
    m_LongDescription = "Usage: flip\n";
    m_LongDescription += "<-i input> <-a 0/1/2: axis (X: 0, Y: 1, Z: 2)> <-o output>\n\n";
    m_LongDescription += m_ShortDescription;
  }
  
  TensorFlipCommand::~TensorFlipCommand()
  {}
  
  int TensorFlipCommand::Execute(int narg, const char *arg[])
  {
    
    GetPot   cl(narg, const_cast<char**>(arg) ); // argument parser
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
    const int   FlipAxis = cl.follow(0,       2,"-a","-A");
    
    typedef double ScalarType;  
    typedef itk::Image<ScalarType, 3>                      ImageType;
    typedef itk::TensorImageIO<ScalarType, 3, 3>           IOType;
    typedef IOType::TensorImageType                        TensorImageType;    
    typedef itk::FlipTensorImageFilter<TensorImageType,TensorImageType>
      FlipFilterType;
  
    
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
    
    
    
    FlipFilterType::Pointer myFlipper = FlipFilterType::New();
    myFlipper->SetInput ( myIO->GetOutput() );
    myFlipper->SetFlipAxis (FlipAxis, 1);
  
    // now: filter
    std::cout << "Flipping Tensors ..." << std::flush << std::endl;
    try
    {
      myFlipper->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;
    
    // write the output
    myIO->SetFileName(fileOut);
    myIO->SetInput (myFlipper->GetOutput());
    
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
