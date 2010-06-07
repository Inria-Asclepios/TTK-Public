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
#include "itkBoostTensorAnisotropyCommand.h"


#include "itkLogTensorImageFilter.h"
#include "itkExpTensorImageFilter.h"
#include "itkResampleTensorImageFilter.h"
#include "itkCenteredRigid2DTransform.h"
#include "itkAffineTensorTransform.h"
#include "itkTransformFileWriter.h"
#include "itkTransformFileReader.h"
#include "itkTransformFactory.h"
#include "itkTensorLinearInterpolateImageFunction.h"
#include "itkTensorImageIO.h"
#include <itkFixedCenterOfRotationAffineTransform.h>

#include "GetPot.h"


namespace itk
{

  BoostTensorAnisotropyCommand::BoostTensorAnisotropyCommand()
  {
    m_ShortDescription = "Boost the tensor anisotropy (FA)";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i input> <-r ratio (1.0 -- 3.0)> <-o output>\n\n";
    m_LongDescription += m_ShortDescription;
  }
  
  BoostTensorAnisotropyCommand::~BoostTensorAnisotropyCommand()
  {}


  int BoostTensorAnisotropyCommand::Execute(int argc, const char* argv[])
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
    double  ratio = cl.follow(2.0,2,"-r","-R");
    const char* outFile = cl.follow("NoFile",2,"-O","-o");
    
    typedef double                                ScalarType;  
    typedef itk::Image<ScalarType,3>              ImageType;
    typedef itk::TensorImageIO<ScalarType, 3, 3>  IOType;
    typedef IOType::TensorImageType               TensorImageType;
    typedef TensorImageType::SizeType    SizeType;
    typedef TensorImageType::SpacingType SpacingType;
    typedef TensorImageType::PointType   PointType;
    
    
    std::cout<<"reading list : "<<tensorFile<<std::endl;
    IOType::Pointer reader = IOType::New();
    reader->SetFileName(tensorFile);
    
    try
    {
      reader->Read();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e << std::endl;
      return -1;
    }
    
    TensorImageType::Pointer tensors = reader->GetOutput();
    std::cout << "done."<<std::endl;
    
    
    TensorImageType::Pointer output = TensorImageType::New();
    
    output->SetRegions (tensors->GetLargestPossibleRegion());
    output->SetOrigin(tensors->GetOrigin());
    output->SetSpacing(tensors->GetSpacing());  
    output->SetDirection(tensors->GetDirection());
    output->Allocate();
    
    itk::ImageRegionIterator<TensorImageType> itIn(tensors, tensors->GetLargestPossibleRegion());
    itk::ImageRegionIterator<TensorImageType> itOut(output, output->GetLargestPossibleRegion());
    itk::ContinuousIndex<double, 3> index;
    
    TensorImageType::PointType x;
    TensorImageType::PixelType pix;
    
    itOut.GoToBegin();
    itIn.GoToBegin();
    
    TensorImageType::PixelType::MatrixType U;
    TensorImageType::PixelType::MatrixType D;
    
    std::cout<<"ratio : "<<ratio<<std::endl;
    
    while( !itOut.IsAtEnd() )
    {
      pix = itIn.Get();
      
      D.Fill (0.0);
      D[0][0] = pix.GetEigenvalue (0);
      D[1][1] = pix.GetEigenvalue (1);
      D[2][2] = pix.GetEigenvalue (2) * (ratio);
      
      for (unsigned int i=0; i<3; i++)
	for (unsigned int j=0; j<3; j++)
	  U[i][j] = pix.GetEigenvector (j)[i];
      
      pix.SetVnlMatrix (U * D * U.GetTranspose());
      
      itOut.Set ( pix );
      ++itIn;
      ++itOut;
    }
    

    IOType::Pointer writer = IOType::New();
    
    writer->SetFileName(outFile);
    writer->SetInput( output );
    
    std::cout << "Writing: " << outFile << std::flush;
    try
    {
      writer->Write();
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
