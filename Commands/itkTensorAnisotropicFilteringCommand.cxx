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
#include "itkTensorAnisotropicFilteringCommand.h"

#include "itkAnisotropicDiffusionTensorImageFilter.h"
#include "itkLogTensorImageFilter.h"
#include "itkExpTensorImageFilter.h"
#include "itkTensorImageIO.h"

#include "GetPot.h"


namespace itk
{
  

  TensorAnisotropicFilteringCommand::TensorAnisotropicFilteringCommand()
  {
    m_ShortDescription = "Perform tensor Perona & Malik anistropic filtering using Log-Euclidean metric";
    m_LongDescription = "Usage: anisotropic_filter\n";
    m_LongDescription += "-i    [fileIn]\n";
    m_LongDescription += "-o    [fileOut]\n";
    m_LongDescription += "-k    [kappa]\n";
    m_LongDescription += "-l    [lambda]\n";
    m_LongDescription += "-dt   [DeltatT]\n";
    m_LongDescription += "-nite [NumberOfIterations]\n";
    m_LongDescription += "-rms  [RMS change: value below = convergence]\n\n";
    m_LongDescription +=m_ShortDescription;
  }

  TensorAnisotropicFilteringCommand::~TensorAnisotropicFilteringCommand()
  {}
  
  
  int TensorAnisotropicFilteringCommand::Execute (int argc, const char *argv[])
  {
    GetPot   cl(argc, const_cast<char**>(argv)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
    
    const bool IsInputPresent = cl.search(2,"-i","-I");
    const bool IsOutputPresent = cl.search(2,"-o","-O");
    
    if(!IsInputPresent || !IsOutputPresent)
    {
      std::cerr << "Input file and/or output not set" << std::endl;
      return -1;
    }

    // parse the arguments
    const double kappa = cl.follow(1.0, 2, "-k", "-K");
    const double lambda = cl.follow(1.0,2,"-l","-L");
    const double deltat =  cl.follow(1.0, 2, "-dt", "-DT");
    const int nite = cl.follow(10,2,"-nite","-NITE");
    const char* file_in = cl.follow("NoFile", 2, "-i","-I");
    const char* file_out = cl.follow("NoFile", 2, "-o","-O");
    const double rms = cl.follow (0.0,2,"-rms","-RMS");
    
    
    if(strcmp(file_in,"NoFile")==0 || strcmp(file_out,"NoFile")==0)
    {
      std::cerr << "Input file and/or output not set" << std::endl;
      return -1;
    }
    
    std::cout << "Processing anisotropic filtering of file: " << file_in << " with: " << std::endl;
    std::cout << "Kappa= " << kappa << std::endl;
    std::cout << "Lambda = " << lambda << std::endl;
    std::cout << "delta t= " << deltat <<std::endl;
    std::cout << "Nite= " << nite << std::endl;  
    std::cout << "Output: " << file_out << std::endl;
    
    // Read in a tensor field:
    typedef double                               ScalarType;  
    typedef itk::TensorImageIO<ScalarType, 3, 3>  IOType;
    typedef IOType::TensorImageType              TensorImageType;
    
    IOType::Pointer io = IOType::New();
    io->SetFileName(file_in);
    
    std::cout << "Reading: " << file_in;
    std::cout << std::flush;  
    try
    {
      io->Read();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;
    
    
    // log
    typedef itk::LogTensorImageFilter<TensorImageType,TensorImageType> LogFilterType;
    LogFilterType::Pointer myLog = LogFilterType::New();
    
    //TensorImageType::Pointer tens = io->GetOutput();  
    myLog->SetInput(io->GetOutput());
    
    
    // Anisotropic filtering:
    typedef itk::AnisotropicDiffusionTensorImageFilter<TensorImageType,TensorImageType>
      FilterType;
    FilterType::Pointer myFilter = FilterType::New();
    
    
    myFilter->SetNumberOfIterations(nite);
    myFilter->SetKappa(kappa);
    myFilter->SetLambda(lambda);
    myFilter->SetTimeStep(deltat);
    myFilter->SetRMSChange (rms);
    
    
    myFilter->SetInput (myLog->GetOutput() );
    
  
    // exp
    typedef itk::ExpTensorImageFilter<TensorImageType,TensorImageType> ExpFilterType;
    ExpFilterType::Pointer myExp = ExpFilterType::New();
    
    myExp->SetInput(myFilter->GetOutput());
    
    
    std::cout << "Pipeline started." << std::endl;
    std::cout << std::flush;  
    try
    {
      myExp->Update();
    }
    catch(itk::ExceptionObject&e)
    {     
      std::cerr << e;
      return -1;
    }
    std::cout << "Pipeline finished." << std::endl;
    

    // Write
    io->SetFileName(file_out);
    io->SetInput(myExp->GetOutput());
    
    std::cout << "Writing file: " << file_out;
    std::cout << std::flush;
    try
    {
      io->Write();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
    
    return 0;
    
  }
  
}
