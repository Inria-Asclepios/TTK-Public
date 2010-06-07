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
#include "itkDWIBrainMaskExtractorCommand.h"

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkBinaryErodeImageFilter.h>
#include <itkBinaryDilateImageFilter.h>
#include <itkVotingBinaryHoleFillingImageFilter.h>
#include <itkVotingBinaryIterativeHoleFillingImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryCrossStructuringElement.h>
#include <itkAndImageFilter.h>
#include <itkRecursiveGaussianImageFilter.h> 
#include <itkMaskImageFilter.h>

#include <iostream>
#include "GetPot.h"

//#define DEBUG_ME

namespace itk
{

  typedef double ScalarType;
  typedef unsigned short IntegerType;
  typedef Image<IntegerType, 3>            ImageType;
  typedef Image<IntegerType, 3>         ImageMaskType;


  DWIBrainMaskExtractorCommand::DWIBrainMaskExtractorCommand()
  {
    m_ShortDescription = "Extract a brain mask from a DWI";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i b0 file> <-o output mask>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  DWIBrainMaskExtractorCommand::~DWIBrainMaskExtractorCommand()
  {}


  void CopyImage( ImageMaskType::Pointer target, ImageMaskType::Pointer source)
  {
    itk::ImageRegionConstIterator<ImageMaskType> itIn( source, source->GetLargestPossibleRegion() );
    itk::ImageRegionIterator<ImageMaskType> itOut( target, target->GetLargestPossibleRegion() );
    
    while( !itOut.IsAtEnd() )
    {
      itOut.Set( itIn.Get() );
      ++itIn;
      ++itOut;
    }
  }
  
  
  bool CompareImages( ImageMaskType::Pointer im1, ImageMaskType::Pointer im2)
  {
    itk::ImageRegionConstIterator<ImageMaskType> itIn( im1, im1->GetLargestPossibleRegion() );
    itk::ImageRegionConstIterator<ImageMaskType> itOut( im2, im2->GetLargestPossibleRegion() );
    
    while( !itOut.IsAtEnd() )
    {
      if( itOut.Value() != itIn.Value() )
      {
	return false;
      }
      ++itOut;
      ++itIn;
    }
    
    return true;
  }
  

  int ComputeHistogram( ImageType::Pointer image)
  {
    // IMPORTANT: IMAGE MUST BE UNSIGNED SHORT
    int N=65535;
    int* histogram = new int[N];
    for( int i=0; i<=N; i++)
    {
      histogram[i] = 0;
    }
    
    itk::ImageRegionConstIterator<ImageType> itIn( image, image->GetLargestPossibleRegion() );
    
    long totVoxels = 0;
    
    int max = -1;
    int min = 9999999;
    while( !itIn.IsAtEnd() )
    {
      histogram[ (int)(itIn.Value()) ]++;
      
      if( itIn.Value()>max )
      {
	max = itIn.Value();
      }
      
      if( itIn.Value()<min )
      {
	min = itIn.Value();
      }
      ++itIn;
      ++totVoxels;
    }
    
    
    //int EPS = 1;
    int seuil = 0;
    //int newseuil = (max + min)/2;
    
    N = max;
    
    double V = 0.0;
    int S = 0;
    
    double mean = 0.0;
    for( int i=min; i<=max; i++)
    {
      mean += (double)(i) * (double)histogram[i];
    }
    mean /= (double)totVoxels;
    
    //std::cout << "Min/Max/Mean: " << min << "/" << max << "/" << mean << std::endl;
    
    //while( abs(newseuil - seuil)>EPS )
    for( seuil = min; seuil<=max; seuil++)
    {
      //seuil = newseuil;
      
      // compute the classes:
      double mean0 = 0.0;
      double mean1 = 0.0;
      //double std0 = 0.0;
      //double std1 = 0.0;
      
      double num0 = 0.0;
      double num1 = 0.0;
      for( int i=min; i<seuil; i++)
      {
	//mean0 += (double)(histogram[i])/(double)(totVoxels) * (double)(i);
	mean0 += (double)histogram[i] * (double)i;
	num0  += (double)histogram[i];
      }
      
      for( int i=seuil; i<max; i++)
      {
	//mean1 += (double)(histogram[i])/(double)(totVoxels) * (double)(i);
	mean1 += (double)histogram[i] * (double)i;
	num1  += (double)histogram[i];
      }
      
      if( num0 )
      {
	mean0/=num0;
      }
      if( num1 )
      {
	mean1/=num1;
      }
      
      /*
	for( int i=0; i<seuil; i++)
	{
	std0 += (double)histogram[i]/(double)totVoxels* ((double)i - mean0)*((double)i - mean0);
	//std0 += (double)histogram[i]/num0* ((double)i - mean0)*((double)i - mean0);
	}
	
	for( int i=seuil; i<65536; i++)
	{
	std1 += (double)histogram[i]/(double)totVoxels* ((double)i - mean1)* ((double)i - mean1);
	//std1 += (double)histogram[i]/num1* ((double)i - mean1)* ((double)i - mean1);
	}
      */
      
      
      //std0 = sqrt(std0);
      //std1 = sqrt(std1);
      
      //std::cout << "Classe 0: " << mean0 << " " << std0 << std::endl;
      //std::cout << "Classe 1: " << mean1 << " " << std1 << std::endl;
      
      //std::cout << "Classe 0: " << mean0 << std::endl;
      //std::cout << "Classe 1: " << mean1 << std::endl;
      
      //double alpha = std0*std1/(std0+std1);
      
      //std::cout << "alpha: " << alpha << " " << std0 << " " << std1 << std::endl;
      //newseuil = (int) (alpha*(mean0/std0 + mean1/std1));		
      
      //std::cout << "New threshold: " << newseuil << std::endl;
      
      /*
	if( newseuil > 65535 || newseuil<0)
	{
	std::cerr << "Error: threshold is too low or high, exiting" << std::endl;
	return -1;
	}*/
      
      double Vm = num0 * (mean0 - mean)*(mean0 - mean) + num1*(mean1 - mean)*(mean1 - mean);
      if( Vm > V )
      {
	V = Vm;
	S = seuil;
	//std::cout << "New seuil: " << S << std::endl;
	//getchar();
      }
      
    }
    
    delete [] histogram;
    
    std::cout << "Seuil: " << S << std::endl;
    
    return S;
  }
  

  int DWIBrainMaskExtractorCommand::Execute( int narg, const char* arg[])
  {
    itk::Object::GlobalWarningDisplayOff();
    
    GetPot cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
    
    const char* input = cl.follow("NoFile",2,"-I","-i");
    const char* output = cl.follow("NoFile",2,"-O","-o");

    typedef itk::ImageFileReader<ImageType> ImageReaderType;
    typedef itk::ImageFileWriter<ImageMaskType> WriterType;
    
    ImageReaderType::Pointer reader = ImageReaderType::New();
    reader->SetFileName( input );
    
    std::cout << "Reading: " << input << std::flush;
    try
    {
      reader->Update();
    }
    catch( itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << " Done." << std::endl;
    
    ImageType::Pointer image = reader->GetOutput();
    
    // gaussian filter
    itk::RecursiveGaussianImageFilter<ImageType,ImageType>::Pointer gaussian
      = itk::RecursiveGaussianImageFilter<ImageType,ImageType>::New();
    gaussian->SetInput( image );
    gaussian->SetSigma( 1.0 );
    
    try
    {
      gaussian->Update();
    }
    catch( itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }

    // threshold the image
    itk::BinaryThresholdImageFilter<ImageType,ImageMaskType>::Pointer threshold = 
      itk::BinaryThresholdImageFilter<ImageType,ImageMaskType>::New();
    
    threshold->SetInput( gaussian->GetOutput() );
    
    IntegerType seuil = static_cast<IntegerType>( ComputeHistogram( gaussian->GetOutput() ) );
    threshold->SetLowerThreshold( seuil );
    
    std::cout << "Thresholding..." << std::flush;
    try
    {
      threshold->Update();
    }
    catch( itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;
    
#ifdef DEBUG_ME
    {
      WriterType::Pointer writer = WriterType::New();
      writer->SetInput( threshold->GetOutput() );
      writer->SetFileName( "AfterThreshold.hdr" );
      writer->Update();
    }
#endif
    
    // erode to remove background noise
    typedef itk::BinaryBallStructuringElement<IntegerType, 3> StructuralElementType;
    StructuralElementType ball;
    
    itk::BinaryErodeImageFilter<ImageMaskType,ImageMaskType,StructuralElementType>::Pointer erode = 
      itk::BinaryErodeImageFilter<ImageMaskType,ImageMaskType,StructuralElementType>::New();
    
    ball.SetRadius( 3 );
    
    erode->SetInput( threshold->GetOutput() );
    erode->SetKernel( ball );
    
    std::cout << "Eroding..." << std::flush;
    try
    {
      erode->Update();
    }
    catch( itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;
    
#ifdef DEBUG_ME
    {
      WriterType::Pointer writer = WriterType::New();
      writer->SetInput( erode->GetOutput() );
      writer->SetFileName( "AfterErode.hdr" );
      writer->Update();
    }
#endif
    
    
    typedef BinaryCrossStructuringElement<IntegerType, 3> CrossType;
    
    typedef BinaryDilateImageFilter<ImageMaskType,ImageMaskType,CrossType> DilateFilterType;
    typedef AndImageFilter<ImageMaskType,ImageMaskType,ImageMaskType> AndFilterType;
    
    ImageMaskType::Pointer M0 = threshold->GetOutput();
    ImageMaskType::Pointer Mn = ImageMaskType::New();
    Mn->SetRegions( M0->GetLargestPossibleRegion() );
    Mn->SetSpacing( M0->GetSpacing() );
    Mn->SetOrigin( M0->GetOrigin() );
    Mn->Allocate();
    
    ImageMaskType::Pointer Mnplus1 = erode->GetOutput();
    
    
    CrossType cross;
    cross.SetRadius( 1 );
    //unsigned long rad[3]={3,3,3};
    
    //ball2.SetRadius( rad );
    
    std::cout << "Conditional reconstruction..." << std::flush;
    int iter = 0;
    do
    {
      std::cout << "Iteration: " << iter++ << std::endl;
      CopyImage( Mn, Mnplus1);
      
      DilateFilterType::Pointer dilater = DilateFilterType::New();
      dilater->SetInput( Mn );
      dilater->SetKernel( cross );
      
      try
      {
	dilater->Update();
      }
      catch( itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      
      AndFilterType::Pointer andfilter = AndFilterType::New();
      andfilter->SetInput(0, M0);
      andfilter->SetInput(1, dilater->GetOutput() );
      
      try
      {
	andfilter->Update();
      }
      catch( itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      
      Mnplus1 = andfilter->GetOutput();
      
      /*
	#ifdef DEBUG_ME
	{
	WriterType::Pointer writer = WriterType::New();
	writer->SetInput( andfilter->GetOutput() );
	char filename[512];
	sprintf( filename, "CondReconstruction_iter_%d.hdr", iter);
	writer->SetFileName( filename );
	writer->Update();
	}
	#endif*/
      
    } while( !CompareImages( Mn, Mnplus1) );
    
    std::cout << "Done." << std::endl;
    
#ifdef DEBUG_ME
    {
      WriterType::Pointer writer = WriterType::New();
      writer->SetInput( Mn );
      writer->SetFileName( "AfterCondReconstruction.hdr" );
      writer->Update();
    }
#endif
    // now fill the holes
    
    itk::VotingBinaryIterativeHoleFillingImageFilter< ImageMaskType >::Pointer filler =
      itk::VotingBinaryIterativeHoleFillingImageFilter< ImageMaskType >::New();
    filler->SetInput( Mn );
    filler->SetMaximumNumberOfIterations (1000);
    
    std::cout << "Filling the holes..." << std::flush;
    try
    {
      filler->Update();
    }
    catch( itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    std::cout << "Done." << std::endl;
    
    
    // write the image
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName( output );
    writer->SetInput( filler->GetOutput() );
    
    std::cout << "Writing: " << output << std::flush;
    try
    {
      writer->Update();
    }
    catch( itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    std::cout << " Done." << std::endl;
    
    return 0;
    
  }
  
}
