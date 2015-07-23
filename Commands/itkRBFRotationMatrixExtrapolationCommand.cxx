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
#include "itkRBFRotationMatrixExtrapolationCommand.h"

#include "itkRBFInterpolationRotationMatrixImageFilter.h"

#include <itkImageFileReader.h>
#include <itkPoint.h>
#include <itkImageRegionConstIteratorWithIndex.h>

#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsWriter.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>

#include "GetPot.h"

namespace itk
{

  RBFRotationMatrixExtrapolationCommand::RBFRotationMatrixExtrapolationCommand()
  {
    m_ShortDescription = "Extrapolate a set of rotation matrices using radial basis functions (RBF)";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "-i    [Input Scalar Image = domain of diffusion]\n";
    m_LongDescription += "-rot  [vtkUnstructuredGrid File]\n";
    m_LongDescription += "-s    [Sigma]\n";
    m_LongDescription += "-g    [Gamma]\n";
    m_LongDescription += "-t    [number of threads]\n";
    m_LongDescription += "-o    [Output Rotation Matrix Image]\n\n";
    m_LongDescription += m_ShortDescription;
  }

  RBFRotationMatrixExtrapolationCommand::~RBFRotationMatrixExtrapolationCommand()
  {}

  
  int RBFRotationMatrixExtrapolationCommand::Execute(int narg, const char* arg[])
  {
    
    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
    
    const bool IsInputPresent    = cl.search(2,"-i","-I");
    const bool AreSourcesPresent = cl.search(1,"-rot");  
    const bool IsOutputPresent   = cl.search(2,"-o","-O");
    
    if(!IsInputPresent || !AreSourcesPresent || !IsOutputPresent)
    {
      std::cerr << "Input and (or) mask and (or) output not set." << std::endl;
      return -1;
    }
    
    const char* filemask = cl.follow("NoFile", 2, "-i","-I");
    const char* output   = cl.follow("NoFile", 2, "-o","-O");
    const char* matrixFile = cl.follow("NoFile",1,"-rot");
    
    if( strcmp(matrixFile,"NoFile")==0 || strcmp(output,"NoFile")==0
	|| strcmp(filemask,"NoFile")==0)
    {
      std::cerr << "Input and (or) mask and (or) output not set." << std::endl;
      return -1;
    }
    
    const double sigma = cl.follow(1.0, 2, "-s","-S");
    const double gamma = cl.follow(1.0, 2, "-g","-G");
    const int t        = cl.follow (1, 2, "-t", "-T");
    
    std::cout << "Processing RBF interpolation of file: " << matrixFile << ", with: " << std::endl;
    std::cout << "Sigma: " << sigma << std::endl;
    std::cout << "Gamma: " << gamma << std::endl;
    std::cout << "Mask: " << filemask << std::endl;
    std::cout << "Output: " << output << std::endl;
    std::cout << std::flush;
    
    // typedefs
    typedef double ScalarType;
    typedef itk::Matrix<ScalarType, 3, 3>        MatrixType;
    typedef itk::Image<MatrixType, 3>            MatrixImageType;
    typedef itk::Image<ScalarType, 3>            ImageType;
    typedef itk::RBFInterpolationRotationMatrixImageFilter<ImageType, MatrixImageType>
      FilterType;
    typedef FilterType::VectorOfPixelType        VectorOfMatrixType;
    typedef FilterType::PointType                PointType;
    typedef FilterType::VectorOfPointsType       VectorOfPointsType;
    typedef itk::ImageFileReader<ImageType>      ImageFileReaderType;

    
    // read the input image
    ImageFileReaderType::Pointer imReader = ImageFileReaderType::New();
    imReader->SetFileName(filemask);
    std::cout << "Reading: " << filemask << std::flush;
    try
    {
      imReader->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e << std::endl;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
    ImageType::Pointer input = imReader->GetOutput();
    
  
    // read the model
    std::cout << "Reading: " << matrixFile;
    vtkUnstructuredGridReader* matReader = vtkUnstructuredGridReader::New();
    matReader->SetFileName(matrixFile);
    vtkUnstructuredGrid* matrices = matReader->GetOutput();
    matReader->Update();
    std::cout << " Done." << std::endl;
    
    
    if( !matrices->GetPointData()->GetArray ("Frenet") )
    {
      std::cout << "Error: Array Frenet was not found." << std::endl;
      return -1;
    }
    
  
    std::cout << "Converting...";
    // convert the model to a vector of matrix + points
    VectorOfMatrixType vecM;
    VectorOfPointsType vecP;
    int numPoints = matrices->GetNumberOfPoints();
    
    vtkDoubleArray* matArray = vtkDoubleArray::SafeDownCast (matrices->GetPointData()->GetArray("Frenet"));
    if( !matArray )
    {
      std::cout << "Error: Cannot cast array." << std::endl;
      return -1;
    }
    
    for(int i=0;i<numPoints;i++)
    {
      double pt[3];
      matrices->GetPoint(i,pt);
      PointType p;
      for(int m=0;m<3;m++)
	p[m]=pt[m];
      
      double matCoefs[9];
      matArray->GetTuple(i,matCoefs);
      
      MatrixType M;
      M (0,0) = matCoefs[0];
      M (1,0) = matCoefs[1];
      M (2,0) = matCoefs[2];
      M (0,1) = matCoefs[3];
      M (1,1) = matCoefs[4];
      M (2,1) = matCoefs[5];
      M (0,2) = matCoefs[6];
      M (1,2) = matCoefs[7];
      M (2,2) = matCoefs[8];
      
      vecM.push_back( M );
      vecP.push_back( p );
      
    }
    matReader->Delete();
    std::cout << "Done." << std::endl;
    
    
    FilterType::Pointer myFilter = FilterType::New();
    myFilter->SetInput(input);
    myFilter->SetRotationMatrices(vecM);
    myFilter->SetPoints(vecP);
    myFilter->SetSigma(sigma);
    myFilter->SetGamma(gamma);
    myFilter->SetNumberOfThreads (t);
    
    
    std::cout << "Filtering..." << std::flush;
    try
    {
      myFilter->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e << std::endl;
      return -1;
    }
    std::cout << "Done." << std::endl;
    
    /*
      VectorOfMatrixType test = FilterType::InterpolateValuesAt (vecM,vecP,vecP,1,1);
      for( unsigned int i=0;i<test.size();i++)
      {
      std::cout << "Pos: " << vecP[i] << std::endl;
      std::cout << "Real: " << vecM[i] << std::endl;
      std::cout << "Int: " << test[i] << std::endl;
      
      }*/
    
  

    MatrixImageType::Pointer itkMatImage = myFilter->GetOutput();
    //MatrixImageType::SizeType size = itkMatImage->GetLargestPossibleRegion().GetSize();
    MatrixImageType::PointType origin = itkMatImage->GetOrigin();
    MatrixImageType::SpacingType spacing = itkMatImage->GetSpacing();
    
    //double origin2[3]  = {origin[0],origin[1],origin[2]};
    //double spacing2[3] = {spacing[0],spacing[1],spacing[2]};
    
    //vtkStructuredPoints* matImage = vtkStructuredPoints::New();
    vtkUnstructuredGrid* matImage = vtkUnstructuredGrid::New();
    //matImage->SetDimensions (size[0], size[1], size[2]);
    //matImage->SetOrigin ( origin2 );
    //matImage->SetSpacing ( spacing2 );
    
    vtkDoubleArray* matImageArray = vtkDoubleArray::New();
    matImageArray->SetName ("Frenet");
    matImageArray->SetNumberOfComponents (9);
    
    vtkPoints* myPoints = vtkPoints::New();
    
    typedef itk::ImageRegionConstIteratorWithIndex<MatrixImageType> IteratorType;
    IteratorType it (itkMatImage, itkMatImage->GetLargestPossibleRegion() );
    while( !it.IsAtEnd() )
    {
      
      MatrixType m = it.Get();
      double coef[9];
      coef[0] = m (0,0);
      coef[1] = m (1,0);
      coef[2] = m (2,0);
      coef[3] = m (0,1);
      coef[4] = m (1,1);
      coef[5] = m (2,1);
      coef[6] = m (0,2);
      coef[7] = m (1,2);
      coef[8] = m (2,2);
      
      matImageArray->InsertNextTuple (coef);
      
      MatrixImageType::IndexType index = it.GetIndex();
      myPoints->InsertNextPoint (origin[0]+index[0]*spacing[0], origin[1]+index[1]*spacing[1],
				 origin[2]+index[2]*spacing[2]);
      
      ++it;
    }
    
    matImage->SetPoints (myPoints);
    matImage->GetPointData()->AddArray (matImageArray);
    
    //vtkStructuredPointsWriter* writer = vtkStructuredPointsWriter::New();
    vtkUnstructuredGridWriter* writer = vtkUnstructuredGridWriter::New();
    writer->SetInputData (matImage);
    writer->SetFileName (output);
    std::cout << "Writing..." << std::flush;
    writer->Write();  
    std::cout << "Done." << std::endl;
    
    myPoints->Delete();
    matImage->Delete();
    matImageArray->Delete();
    writer->Delete();
    
    return 0;
  }
  
}
