#include "itkHessianTensorImageCalculatorCommand.h"

#include "itkTensorImageIO.h"
#include "itkLogTensorImageFilter.h"
#include <itkImage.h>
#include <itkImageFileWriter.h>
#include <itkHessianRecursiveGaussianImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>

#include <iostream>
#include "GetPot.h"

namespace itk
{

  HessianTensorImageCalculatorCommand::HessianTensorImageCalculatorCommand()
  {
    m_ShortDescription = "Extract the largest eigenvalue of the Hessian of a tensor field";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<-i input> <-s sigma> <-o output>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  HessianTensorImageCalculatorCommand::~HessianTensorImageCalculatorCommand()
  {}
  
  int HessianTensorImageCalculatorCommand::Execute (int narg, const char* arg[])
  {
    
  // parse arguments
    GetPot cl (narg, const_cast<char**>(arg));
    if( cl.size() == 1 || cl.search (2,"--help","-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }

    const char* file_in = cl.follow("NoFile", 2, "-i","-I");
    const char* file_out = cl.follow("NoFile", 2, "-o","-O");
    const double sigma = cl.follow (1.0, 2, "-s", "-S");

    typedef double                               ScalarType;
    typedef itk::TensorImageIO<ScalarType, 3, 3> TensorIOType;
    typedef TensorIOType::TensorType             TensorType;
    typedef TensorIOType::TensorImageType        TensorImageType;
    
    
    typedef itk::Image<ScalarType, 3>            ImageType;
    
    
    
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
    
    std::cout << "Done." << std::endl;
    
    TensorImageType::Pointer myTensorImage = myIO->GetOutput();
    
    
    std::cout << "Loging...";
    std::cout << std::flush;
    
    // log the tensor field
    typedef itk::LogTensorImageFilter<TensorImageType, TensorImageType> LogFilterType;
    LogFilterType::Pointer myLoger = LogFilterType::New();
    myLoger->SetInput (myTensorImage);
    try
    {
      myLoger->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }
    
    std::cout << "Done." << std::endl;
    
    myTensorImage = myLoger->GetOutput();
    
    
    std::cout << "Converting...";
    std::cout << std::flush;
    
    std::vector<ImageType::Pointer> ImageVector;
    
    // convert the tensor image into 6 scalar images
    for( unsigned int i=0 ;i<6; i++)
    {
      ImageType::Pointer image = ImageType::New();
      
      TensorImageType::RegionType region = myTensorImage->GetLargestPossibleRegion();
      
      image->SetRegions (region);
      image->SetSpacing (myTensorImage->GetSpacing());
      image->SetOrigin (myTensorImage->GetOrigin());
      image->SetDirection (myTensorImage->GetDirection());
      
      image->Allocate();
      
      itk::ImageRegionConstIteratorWithIndex<TensorImageType>  itIn (myTensorImage, myTensorImage->GetLargestPossibleRegion());
      itk::ImageRegionIteratorWithIndex<ImageType> itOut(image, image->GetLargestPossibleRegion());
      
      while( !itIn.IsAtEnd() )
      {
	
	TensorType T = itIn.Get();
	
	itOut.Set (T[i]);
	
	++itIn;
	++itOut;
      }
      
      
      ImageVector.push_back (image);
      
    }
    
    std::cout << "Done." << std::endl;
    
    std::cout << "Hessianing...";
    std::cout << std::flush;
    // now filters by computing the Hessian of each image
    typedef itk::HessianRecursiveGaussianImageFilter<ImageType>
      HessianFilterType;
    typedef HessianFilterType::OutputImageType HessianImageType;
    typedef HessianImageType::PixelType        HessianPixelType;
    
    std::vector<HessianImageType::Pointer> HessianImageVector;
    
    for( unsigned int i=0; i<6; i++)
    {
      HessianFilterType::Pointer myHessian = HessianFilterType::New();
      myHessian->SetInput (ImageVector[i]);
      myHessian->SetSigma (sigma);
      
      try
      {
	myHessian->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      
      HessianImageVector.push_back (myHessian->GetOutput());
    }
    
    std::cout << "Done." << std::endl;
    
    ImageVector.clear();
    
    std::cout << "Combining...";
    std::cout << std::flush;
    // finally: combines everything and extract the max eigenvalue  
    
    ImageType::Pointer myFinalImage  = ImageType::New();
    //ImageType::Pointer myFinalImage2 = ImageType::New();
    //ImageType::Pointer myFinalImage3 = ImageType::New();
    TensorImageType::RegionType region = myTensorImage->GetLargestPossibleRegion();
    
    myFinalImage->SetRegions (region);
    myFinalImage->SetSpacing (myTensorImage->GetSpacing());
    myFinalImage->SetOrigin (myTensorImage->GetOrigin());
    myFinalImage->SetDirection (myTensorImage->GetDirection());
    myFinalImage->Allocate();
    /*
      myFinalImage2->SetRegions (region);
      myFinalImage2->SetSpacing (myTensorImage->GetSpacing());
      myFinalImage2->SetOrigin (myTensorImage->GetOrigin());
      myFinalImage2->SetDirection (myTensorImage->GetDirection());
      myFinalImage2->Allocate();
      
      myFinalImage3->SetRegions (region);
      myFinalImage3->SetSpacing (myTensorImage->GetSpacing());
      myFinalImage3->SetOrigin (myTensorImage->GetOrigin());
      myFinalImage3->SetDirection (myTensorImage->GetDirection());
      myFinalImage3->Allocate();
    */
    
    typedef itk::ImageRegionConstIteratorWithIndex<HessianImageType> HessianImageIteratorType;
    
    std::vector<HessianImageIteratorType> HessianIteratorList;
    
    for( unsigned int i=0; i<6; i++)
    {
      HessianImageIteratorType it (HessianImageVector[i], HessianImageVector[i]->GetLargestPossibleRegion());
      HessianIteratorList.push_back (it);
    }
    
    itk::ImageRegionIteratorWithIndex<ImageType> itOut (myFinalImage, myFinalImage->GetLargestPossibleRegion());
    //itk::ImageRegionIteratorWithIndex<ImageType> itOut2 (myFinalImage2, myFinalImage2->GetLargestPossibleRegion());
    //itk::ImageRegionIteratorWithIndex<ImageType> itOut3 (myFinalImage3, myFinalImage3->GetLargestPossibleRegion());
    
    while( !itOut.IsAtEnd() )
    {
      
      TensorType SuperT;
      TensorType SuperT2;
      
      vnl_matrix<double> HH (3,18,0.0);
      
      for( unsigned int i=0; i<6; i++)
      {
	HessianPixelType H = HessianIteratorList[i].Get();
	
	// create a tensor struct for norm
	TensorType T;
	T[0] = H[0];
	T[1] = H[1];
	T[2] = H[3];
	T[3] = H[2];
	T[4] = H[4];
	T[5] = H[5];
	
	HH (0,i*3+0) = T[0];
	HH (1,i*3+0) = T[1];
	HH (2,i*3+0) = T[3];
	HH (0,i*3+1) = T[1];
	HH (1,i*3+1) = T[2];
	HH (2,i*3+1) = T[4];
	HH (0,i*3+2) = T[3];
	HH (1,i*3+2) = T[4];
	HH (2,i*3+2) = T[5];
	
	/*
	  double max = -999999999;
	  for( unsigned int j=0; j<6; j++)
	  {
	  if ( T[j] > max )
	  {
          max = T[j];
	  }
	  }
	  
	  SuperT[i]  = T.GetNorm();
	  SuperT2[i] = T.GetEigenvalue (2);
	*/
	//SuperT2[i] = max;
      }
      
      
      vnl_svd<double> MySVD (HH);
      
      //itOut.Set (SuperT.GetEigenvalue (2));
      //itOut2.Set (SuperT2.GetEigenvalue (2));
      itOut.Set (MySVD.W (0));
      
      
      ++itOut;
      //++itOut2;
      //++itOut3;
      for( unsigned int i=0; i<6; i++)
      {
	++(HessianIteratorList[i]);
      }
      
    }
    
    std::cout << "Done." << std::endl;
    
    
    std::cout << "Writing...";
    std::cout << std::flush;
    
    
    
    std::string sfile_out = file_out;
    std::string::size_type pos = sfile_out.rfind (".gipl");
    if( pos != std::string::npos )
    {
      typedef itk::Image<unsigned short, 3> LightImageType;
      
      itk::RescaleIntensityImageFilter<ImageType, LightImageType>::Pointer rescaler=
	itk::RescaleIntensityImageFilter<ImageType, LightImageType>::New();
      
      rescaler->SetOutputMinimum ( 0 );
      rescaler->SetOutputMaximum ( 32767 );
      rescaler->SetInput ( myFinalImage );
      try
      {
	rescaler->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      
      itk::ImageFileWriter<LightImageType>::Pointer writer =
	itk::ImageFileWriter<LightImageType>::New();
      writer->SetFileName (file_out);
      writer->SetInput (rescaler->GetOutput());
      try
      {
	writer->Write();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      
    }
    else
    {
      // write the result:
      itk::ImageFileWriter<ImageType>::Pointer writer = itk::ImageFileWriter<ImageType>::New();
      writer->SetFileName (file_out);
      writer->SetInput (myFinalImage);
      try
      {
	writer->Write();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return -1;
      }
      
    }

    
    /*
      itk::ImageFileWriter<ImageType>::Pointer writer2 = itk::ImageFileWriter<ImageType>::New();
      std::string out2 = s_fileout + "_maxev.nrrd";
      writer2->SetFileName (out2.c_str());
      writer2->SetInput (myFinalImage2);
      try
      {
      writer2->Write();
      }
      catch (itk::ExceptionObject &e)
      {
      std::cerr << e;
      return -1;
      }
      
      
      itk::ImageFileWriter<ImageType>::Pointer writer3 = itk::ImageFileWriter<ImageType>::New();
      std::string out3 = s_fileout + "_tensorev.nrrd";
      writer3->SetFileName (out3.c_str());
      writer3->SetInput (myFinalImage3);
      try
      {
      writer3->Write();
      }
      catch (itk::ExceptionObject &e)
      {
      std::cerr << e;
      return -1;
      }
    */
    
    std::cout << "Done." << std::endl;
    
    
    return 0;
  }
  
}
