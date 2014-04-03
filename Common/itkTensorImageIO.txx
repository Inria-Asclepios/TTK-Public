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
#ifndef _itk_TensorImageIO_txx_
#define _itk_TensorImageIO_txx_

#include "itkTensorImageIO.h"

#include "ttkConfigure.h"

#ifdef TTK_USE_MIPS
#include <inrimage/Inrimage.h>
#include <linearAlgebra/Vec3.h>
//using namespace yav;
#endif

#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIterator.h>
#include <itkImageRegionConstIteratorWithIndex.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkDiffusionTensor3D.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkVectorImage.h>
#include <itkVector.h>
#include <itkVariableLengthVector.h>

#include <itkNrrdImageIO.h>
#include <itkNiftiImageIO.h>

#include <vtkStructuredPointsReader.h>
#include <vtkStructuredPointsWriter.h>
#include <vtkStructuredPoints.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>

#include <vtksys/SystemTools.hxx>
#include <vtksys/stl/string>

namespace itk
{


  template<class T, unsigned int TensorDimension, unsigned int ImageDimension>
  bool
  TensorImageIO<T,TensorDimension,ImageDimension>
  ::CheckExtension (const char* filename, const char* ext) const
  {
    const char* found = strstr(filename, ext);

    // return true if extension is found and is at the end of the filename
    return ( found && found == ( filename+strlen(filename)-strlen(ext) ) );
  }

  template<class T, unsigned int TensorDimension, unsigned int ImageDimension>
  void
  TensorImageIO<T,TensorDimension,ImageDimension>
  ::Read (void)
  {

    if(m_FileName == "")
    {
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error: FileName is not set.");
    }

    vtksys_stl::string ext = vtksys::SystemTools::GetFilenameExtension (m_FileName.c_str());
    if( ext=="" )
    {
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error: Extension is not set.");
    }
   
    try
    {
      if ( CheckExtension(m_FileName.c_str(), ".vtk") )
      {
        this->ReadVTK (m_FileName.c_str());
      }
      else if ( CheckExtension(m_FileName.c_str(), ".nrrd") || CheckExtension(m_FileName.c_str(), ".nhdr") )
      {
        this->ReadNrrd (m_FileName.c_str());
      }
      else if ( CheckExtension(m_FileName.c_str(), ".inr.gz") )
      {
        this->ReadInrimage (m_FileName.c_str());
      }
      else if ( CheckExtension(m_FileName.c_str(), ".nii") || CheckExtension(m_FileName.c_str(), ".nii.gz") )
      {
        this->ReadNifti (m_FileName.c_str());
      }
      else if ( CheckExtension(m_FileName.c_str(), ".mha") || CheckExtension(m_FileName.c_str(), ".mhd") )
      {
        this->ReadMha (m_FileName.c_str());
      }
      else
      {
        throw itk::ExceptionObject (__FILE__,__LINE__,"Error: extension not recognized."
                                    " Supported extenstions are:\n-vtk\n-nrrd\n-nhdr\n-inr.gz\n-nii\n-nii.gz\n-mha\n-mhd.");
      }
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error in TensorImageIO::Read()");
    }
    
  }



  template<class T, unsigned int TensorDimension, unsigned int ImageDimension>
  void
  TensorImageIO<T,TensorDimension,ImageDimension>
  ::ReadInrimage (const char* filename)
  {
#ifdef TTK_USE_MIPS
    // Read the inrimage
    yav::Inrimage* tens = NULL;
    try
    {
      tens = new yav::Inrimage (filename);
      if(tens->getVdim() != TensorType::NDegreesOfFreedom ||
         tens->getVectorialMode() != VM_INTERLACED)
        throw itk::ExceptionObject(__FILE__,__LINE__,"Error: the dimensions or vectorial mode are not compatible with template arguments.");
    }
    catch (...)
    {
      delete tens;
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error while reading. ");
      
    }

    // get the important information about size, spacing, origin, etc.
    SizeType size = {{ tens->getX(), tens->getY(), tens->getZ() }};
    SpacingType spacing;
    spacing[0] = tens->getVX();
    spacing[1] = tens->getVY();
    spacing[2] = tens->getVZ();
    IndexType index = {{0,0,0}};
    float origin[3] = { tens->getTranslation()[0], tens->getTranslation()[1],
                        tens->getTranslation()[2] };

    RegionType region;
    region.SetSize (size);
    region.SetIndex (index);
        
    // Allocate the output image
    if(m_Output)
    {
      m_Output->Initialize();
    }
    
    m_Output->SetRegions (region);
    m_Output->SetSpacing (spacing);
    m_Output->SetOrigin (origin);

    // we only use the rotational part of the
    // inrimage transformation matrix, as the translation is in the origin.
    // note that we take the Transformation matrix instead of the rotation instance
    // of the inrimage as the rotation instance is in voxel coordinates.
    // Note also that it SHOULD be a rigid rotation matrix,
    // in theory its det. should be 1...
    
    yav::Matrix4x4<double> matrix = tens->getTransformationMatrix();
    DirectionType direction;
    
    for (unsigned int i=0; i<3; i++)
      for (unsigned int j=0; j<3; j++)
	direction[i][j] = matrix[i][j];
    
    m_Output->SetDirection(direction);

    try
    {
      m_Output->Allocate();      
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error in TensorImageIO::Read() during allocation.");
    }        


    // Copy the input to the output and free the input image
    tens->convertCast(yav::Inrimage::WT_DOUBLE_VECTOR);
    double *buffer = (double*)(tens->getData());
    
    typedef ImageRegionIterator<TensorImageType> IteratorType;
    IteratorType it(m_Output, m_Output->GetLargestPossibleRegion());
    unsigned long numPixels = m_Output->GetLargestPossibleRegion().GetNumberOfPixels();
    unsigned long step = numPixels/100;
    unsigned int ind = 0;

    this->UpdateProgress ( 0.0 );
    
    while(!it.IsAtEnd())
    {

      unsigned int offset = ind*(TensorType::NDegreesOfFreedom);      
      T* coeff = new T[TensorType::NDegreesOfFreedom];

      for(unsigned int i=0;i<(TensorType::NDegreesOfFreedom);i++)
        coeff[i] = static_cast<T>(buffer[offset+i]);
            
      TensorType Tens(coeff);
      delete [] coeff;
      
      it.Set (Tens);
      
      ++it;
      ++ind;

      if( (ind%step)==0 )
      {
        this->UpdateProgress ( double(ind)/double(numPixels) );
      }
      
    }

    this->UpdateProgress ( 1.0 );
    
    delete tens;    
#else
	std::cerr << "TTK was not compiled with inrimage support, sorry!" << std::endl;
#endif
  }
  

  template<class T, unsigned int TensorDimension, unsigned int ImageDimension>
  void
  TensorImageIO<T,TensorDimension,ImageDimension>
  ::ReadVTK (const char* filename)
  {

    // VTK only supports 3D image with 3x3 tensors
    if( TensorDimension!=3 || ImageDimension!=3 )
    {
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error: VTK only supports 3D images and 3x3 tensors.");
    }
    
    
    // actually read the file
    vtkStructuredPointsReader* reader = vtkStructuredPointsReader::New();
    reader->SetFileName( filename );
    
    if( !reader->IsFileStructuredPoints() )
    {
      reader->Delete();      
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: File is not a VTK structured points.");
    }
    
    vtkStructuredPoints* tensors = reader->GetOutput();
    reader->Update();
    
    // Allocate the output image
    if( m_Output )
    {
      m_Output->Initialize();
    }
    
    typename TensorImageType::SizeType size;
    double spacing[3];
    double origin[3];
    for(unsigned int i=0; i<3; i++)
    {
      size[i] = tensors->GetDimensions()[i];
      spacing[i] = tensors->GetSpacing()[i];
      origin[i] = tensors->GetOrigin()[i];
    }
    
    typename TensorImageType::RegionType region;
    typename TensorImageType::IndexType index = {{0,0,0}};
    
    region.SetSize(size);
    region.SetIndex(index);
    
    m_Output->SetRegions(region);
    m_Output->SetSpacing(spacing);
    m_Output->SetOrigin(origin);
    
    try
    {
      m_Output->Allocate();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e << std::endl;
      reader->Delete();
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error during memory allocation");
    }
    

    vtkDoubleArray* data = (vtkDoubleArray*)(tensors->GetPointData()->GetTensors());

    typedef ImageRegionIterator<TensorImageType> IteratorType;
    IteratorType it(m_Output, m_Output->GetLargestPossibleRegion());

    unsigned long numPixels = m_Output->GetLargestPossibleRegion().GetNumberOfPixels();
    unsigned long step = numPixels/100;
    unsigned int ind = 0;

    this->UpdateProgress ( 0.0 );
    
    while( !it.IsAtEnd() )
    {
      
      double coefs[9];
      data->GetTuple(ind,coefs);
      
      T newCoefs[6];
      newCoefs[0] = static_cast<T>(coefs[0]);
      newCoefs[1] = static_cast<T>(coefs[3]);
      newCoefs[2] = static_cast<T>(coefs[4]);
      newCoefs[3] = static_cast<T>(coefs[6]);
      newCoefs[4] = static_cast<T>(coefs[7]);
      newCoefs[5] = static_cast<T>(coefs[8]);
      
      TensorType tensor(newCoefs);
      it.Set (tensor);
      
      ++ind;
      ++it;

      if( (ind%step)==0 )
      {
        this->UpdateProgress ( double(ind)/double(numPixels) );
      }
    }

    this->UpdateProgress ( 1.0 );
    
    reader->Delete();
  }



  
  template<class T, unsigned int TensorDimension, unsigned int ImageDimension>
  void
  TensorImageIO<T,TensorDimension,ImageDimension>
  ::ReadNrrd (const char* filename)
  {    
    typedef double                                   PixelType;
    typedef itk::Image <PixelType, ImageDimension>   ImageType;
    
    itk::NrrdImageIO::Pointer myIO = itk::NrrdImageIO::New();
    myIO->SetFileName(filename);
    myIO->ReadImageInformation();
    
    unsigned int numComponents = myIO->GetNumberOfComponents();
    
    if( numComponents!=DegreesOfFreedom )
    {
      char message[512];
      sprintf(message, "Error: Number of components should be: %d, and actually is: %d.", DegreesOfFreedom, numComponents);
      throw itk::ExceptionObject(__FILE__,__LINE__,message);
    }
    
    if (myIO->GetPixelType() != itk::ImageIOBase::DIFFUSIONTENSOR3D)
    {
	// Two cases : image is stored as float or double
    	if (myIO->GetComponentType() == itk::ImageIOBase::DOUBLE) 
    	{
      	  typedef itk::Vector<double, DegreesOfFreedom>     VectorType;
      	  typedef itk::Image <VectorType, ImageDimension>   VectorImageType;
    
	  typedef itk::ImageFileReader<VectorImageType> ReaderType;
    
	  typename ReaderType::Pointer myReader = ReaderType::New();
      
          myReader->SetFileName (filename);
      	  myReader->SetImageIO(myIO);
     
	  try
      	  {
            myReader->Update();
      	  }
      	  catch (itk::ExceptionObject &e)
      	  {
            std::cerr << e;
            throw itk::ExceptionObject (__FILE__,__LINE__,"Error in TensorImageIO::ReadNrrd()");
      	  }
    
	  typename VectorImageType::Pointer myImage = myReader->GetOutput();
     
	  if( m_Output )
       	  {
            m_Output->Initialize();
      	  }

      	  typename ImageType::RegionType region = myImage->GetLargestPossibleRegion();
    
	  m_Output->SetRegions (region);
      	  m_Output->SetSpacing (myImage->GetSpacing());
      	  m_Output->SetOrigin (myImage->GetOrigin());
      	  m_Output->SetDirection (myImage->GetDirection());
 
	  try
      	  {
       	    m_Output->Allocate();      
      	  }
      	  catch (itk::ExceptionObject &e)
      	  {
            std::cerr << e;
            throw itk::ExceptionObject (__FILE__,__LINE__,"Error in TensorImageIO::Read() during allocation.");
      	  }
    
	  itk::ImageRegionConstIteratorWithIndex<VectorImageType>  itIn (myImage, myImage->GetLargestPossibleRegion());
      	  itk::ImageRegionIteratorWithIndex<TensorImageType> itOut(m_Output, m_Output->GetLargestPossibleRegion());

      	  unsigned long numPixels = m_Output->GetLargestPossibleRegion().GetNumberOfPixels();
      	  unsigned long step = numPixels/100;
      	  unsigned int ind = 0;
    
	  this->UpdateProgress ( 0.0 );
    
	  while(!itOut.IsAtEnd())
       	  {
       	  
            VectorType vec = itIn.Get();
            TensorType tensor;
      
              for( unsigned int j=0; j<DegreesOfFreedom; j++)
              {
                  tensor[j] = static_cast<typename TensorType::ValueType>(vec[j]);
              }
              
              itOut.Set (tensor);      
              ++itOut;
              ++itIn;
              ++ind;
              
              if( (ind%step)==0 )
              {
                  this->UpdateProgress ( double(ind)/double(numPixels) );
              }
      
	 }

         this->UpdateProgress ( 1.0 );
       }
       else
       {
	 //Case image is in float
      	 typedef itk::Vector<float, DegreesOfFreedom>     VectorType;
      	 typedef itk::Image <VectorType, ImageDimension>   VectorImageType;
    
	 typedef itk::ImageFileReader<VectorImageType> ReaderType;
    
	 typename ReaderType::Pointer myReader = ReaderType::New();
	
      	 myReader->SetFileName (filename);
         myReader->SetImageIO(myIO);
     
	 try
      	 {
           myReader->Update();
      	 }
      	 catch (itk::ExceptionObject &e)
      	 {
           std::cerr << e;
           throw itk::ExceptionObject (__FILE__,__LINE__,"Error in TensorImageIO::ReadNrrd()");
      	 }
    
	 typename VectorImageType::Pointer myImage = myReader->GetOutput();
     
	 if( m_Output )
      	 {
           m_Output->Initialize();
      	 }

      	 typename ImageType::RegionType region = myImage->GetLargestPossibleRegion();
    
	 m_Output->SetRegions (region);
      	 m_Output->SetSpacing (myImage->GetSpacing());
      	 m_Output->SetOrigin (myImage->GetOrigin());
      	 m_Output->SetDirection (myImage->GetDirection());
 
         try
      	 {
           m_Output->Allocate();      
      	 }
      	 catch (itk::ExceptionObject &e)
      	 {
           std::cerr << e;
           throw itk::ExceptionObject (__FILE__,__LINE__,"Error in TensorImageIO::Read() during allocation.");
      	 }
    
	 itk::ImageRegionConstIteratorWithIndex<VectorImageType>  itIn (myImage, myImage->GetLargestPossibleRegion());
      	 itk::ImageRegionIteratorWithIndex<TensorImageType> itOut(m_Output, m_Output->GetLargestPossibleRegion());

         unsigned long numPixels = m_Output->GetLargestPossibleRegion().GetNumberOfPixels();
      	 unsigned long step = numPixels/100;
      	 unsigned int ind = 0;
    
	 this->UpdateProgress ( 0.0 );
    
	 while(!itOut.IsAtEnd())
         {
       	   VectorType vec = itIn.Get();
           TensorType tensor;
           
           for( unsigned int j=0; j<DegreesOfFreedom; j++)
           {
                tensor[j] = static_cast<typename TensorType::ValueType>(vec[j]);
           }

           itOut.Set (tensor);
      
	   ++itOut;
           ++itIn;
           ++ind;

           if( (ind%step)==0 )
           {
             this->UpdateProgress ( double(ind)/double(numPixels) );
           }
      
	 }

         this->UpdateProgress ( 1.0 );
      }    
    }
    else
    {
      if( TensorDimension!=3 )
      {
        throw itk::ExceptionObject (__FILE__,__LINE__,"Error: Nrrd format with diffusiontensor3d only supports 3x3 tensors.");
      }
    
    
      typedef itk::DiffusionTensor3D<double>             DiffPixelType;
      typedef itk::Image <DiffPixelType, ImageDimension> DiffImageType;
    
      typedef itk::ImageFileReader<DiffImageType> ReaderType;
    
      typename ReaderType::Pointer myReader = ReaderType::New();
      myReader->SetFileName (filename);
      myReader->SetImageIO(myIO);
    
      try
      {
        myReader->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
      	throw itk::ExceptionObject (__FILE__,__LINE__,"Error in TensorImageIO::ReadNrrd()");
      }

    
      typename DiffImageType::Pointer myImage = myReader->GetOutput();

      if( m_Output )
      {
	m_Output->Initialize();
      }

      typename DiffImageType::RegionType region = myImage->GetLargestPossibleRegion();
    
      m_Output->SetRegions (region);
      m_Output->SetSpacing (myImage->GetSpacing());
      m_Output->SetOrigin (myImage->GetOrigin());
      m_Output->SetDirection (myImage->GetDirection());
 
      try
      {
        m_Output->Allocate();      
      }
      catch (itk::ExceptionObject &e)
      {
        std::cerr << e;
        throw itk::ExceptionObject (__FILE__,__LINE__,"Error in TensorImageIO::Read() during allocation.");
      }
    
      itk::ImageRegionConstIteratorWithIndex<DiffImageType>  itIn (myImage, myImage->GetLargestPossibleRegion());
      itk::ImageRegionIteratorWithIndex<TensorImageType> itOut(m_Output, m_Output->GetLargestPossibleRegion());

      unsigned long numPixels = m_Output->GetLargestPossibleRegion().GetNumberOfPixels();
      unsigned long step = numPixels/100;
      unsigned int ind = 0;

      this->UpdateProgress ( 0.0 );
    
      while(!itOut.IsAtEnd())
      {
      
        DiffPixelType d3d = itIn.Get();
      	TensorType tensor;
      	for( int j=0; j<3; j++)
        {
	  for(int i=0; i<=j; i++)
          {
	    tensor.SetComponent ( i, j, static_cast<T>(d3d (i,j)));
          }
        }

        itOut.Set (tensor);
      
        ++itOut;
      	++itIn;
      	++ind;

        if( (ind%step)==0 )
        {
          this->UpdateProgress ( double(ind)/double(numPixels) );
        }
      
      }

      this->UpdateProgress ( 1.0 );
    }
  }

  template<class T, unsigned int TensorDimension, unsigned int ImageDimension>
  void
  TensorImageIO<T,TensorDimension,ImageDimension>
  ::ReadNifti (const char* filename)
  {
    typedef T                                        PixelType;
    typedef itk::Image <PixelType, ImageDimension>   ImageType;
    
    itk::NiftiImageIO::Pointer myIO = itk::NiftiImageIO::New();
    myIO->SetFileName(filename);
    myIO->ReadImageInformation();
    
	
    unsigned int numComponents = myIO->GetNumberOfComponents();
    
    if( numComponents!=DegreesOfFreedom )
    {
      char message[512];
      sprintf(message, "Error: Number of components should be: %d, and actually is: %d.", DegreesOfFreedom, numComponents);
      throw itk::ExceptionObject(__FILE__,__LINE__,message);
    }
    
    typedef itk::Vector<PixelType, DegreesOfFreedom>  VectorType;
    typedef itk::Image <VectorType, ImageDimension>   VectorImageType;
    
    typedef itk::ImageFileReader<VectorImageType> ReaderType;
    
    typename ReaderType::Pointer myReader = ReaderType::New();
    myReader->SetFileName (filename);
    myReader->SetImageIO(myIO);
    
    try
    {
      myReader->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error in TensorImageIO::ReadNifti()");
    }
    
			
    typename VectorImageType::Pointer myImage = myReader->GetOutput();
    
    if( m_Output )
    {
      m_Output->Initialize();
    }

    typename ImageType::RegionType region = myImage->GetLargestPossibleRegion();
    
    m_Output->SetRegions (region);
    m_Output->SetSpacing (myImage->GetSpacing());
    m_Output->SetOrigin (myImage->GetOrigin());
    m_Output->SetDirection (myImage->GetDirection());
 
    try
    {
      m_Output->Allocate();      
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error in TensorImageIO::Read() during allocation.");
    }
    
    itk::ImageRegionConstIteratorWithIndex<VectorImageType>  itIn (myImage, myImage->GetLargestPossibleRegion());
    itk::ImageRegionIteratorWithIndex<TensorImageType> itOut(m_Output, m_Output->GetLargestPossibleRegion());

    unsigned long numPixels = m_Output->GetLargestPossibleRegion().GetNumberOfPixels();
    unsigned long step = numPixels/100;
    unsigned int ind = 0;

    this->UpdateProgress ( 0.0 );
    
    while(!itOut.IsAtEnd())
    {
      
      VectorType vec = itIn.Get();
      TensorType tensor;
      
      for( unsigned int j=0; j<DegreesOfFreedom; j++)
      {
        tensor[j] = vec[j];
      }
      
      itOut.Set (tensor);
      
      ++itOut;
      ++itIn;
      ++ind;

      if( (ind%step)==0 )
      {
        this->UpdateProgress ( double(ind)/double(numPixels) );
      }
      
    }

    this->UpdateProgress ( 1.0 );
	
    }

  
  template<class T, unsigned int TensorDimension, unsigned int ImageDimension>
  void
  TensorImageIO<T,TensorDimension,ImageDimension>
  ::ReadMha (const char* filename)
  {

    typedef T                                        PixelType;
    typedef itk::Image <PixelType, ImageDimension>   ImageType;
    
    itk::MetaImageIO::Pointer myIO = itk::MetaImageIO::New();
    myIO->SetFileName(filename);
    myIO->ReadImageInformation();
    
	
    unsigned int numComponents = myIO->GetNumberOfComponents();
    
    if( numComponents!=DegreesOfFreedom )
    {
      char message[512];
      sprintf(message, "Error: Number of components should be: %d, and actually is: %d.", DegreesOfFreedom, numComponents);
      throw itk::ExceptionObject(__FILE__,__LINE__,message);
    }
    
    typedef itk::Vector<PixelType, DegreesOfFreedom>  VectorType;
    typedef itk::Image <VectorType, ImageDimension>   VectorImageType;
    
    typedef itk::ImageFileReader<VectorImageType> ReaderType;
    
    typename ReaderType::Pointer myReader = ReaderType::New();
    myReader->SetFileName (filename);
    myReader->SetImageIO(myIO);
    
    try
    {
      myReader->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error in TensorImageIO::ReadNifti()");
    }
    
			
    typename VectorImageType::Pointer myImage = myReader->GetOutput();
    
    if( m_Output )
    {
      m_Output->Initialize();
    }

    typename ImageType::RegionType region = myImage->GetLargestPossibleRegion();
    
    m_Output->SetRegions (region);
    m_Output->SetSpacing (myImage->GetSpacing());
    m_Output->SetOrigin (myImage->GetOrigin());
    m_Output->SetDirection (myImage->GetDirection());
 
    try
    {
      m_Output->Allocate();      
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error in TensorImageIO::Read() during allocation.");
    }
    
    itk::ImageRegionConstIteratorWithIndex<VectorImageType>  itIn (myImage, myImage->GetLargestPossibleRegion());
    itk::ImageRegionIteratorWithIndex<TensorImageType> itOut(m_Output, m_Output->GetLargestPossibleRegion());

    unsigned long numPixels = m_Output->GetLargestPossibleRegion().GetNumberOfPixels();
    unsigned long step = numPixels/100;
    unsigned int ind = 0;

    this->UpdateProgress ( 0.0 );
    
    while(!itOut.IsAtEnd())
    {
      
      VectorType vec = itIn.Get();
      TensorType tensor;
      
      for( unsigned int j=0; j<DegreesOfFreedom; j++)
      {
        tensor[j] = static_cast<typename TensorType::ValueType>(vec[j]);
      }
      
      itOut.Set (tensor);
      
      ++itOut;
      ++itIn;
      ++ind;

      if( (ind%step)==0 )
      {
        this->UpdateProgress ( double(ind)/double(numPixels) );
      }
      
    }

    this->UpdateProgress ( 1.0 );

  }
  
  
  
  

  template<class T, unsigned int TensorDimension, unsigned int ImageDimension>
  void
  TensorImageIO<T,TensorDimension,ImageDimension>
  ::Write (void)
  {

    if(m_Input.IsNull())
    {
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: input is not set.");
    }
    
    if(m_FileName == "")
    {
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: FileName is not set.");
    }
    
    
    vtksys_stl::string ext = vtksys::SystemTools::GetFilenameExtension (m_FileName.c_str());
    if( ext=="" )
    {
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error: Extension is not set.");
    }
    
    
    try
    {
      if ( CheckExtension(m_FileName.c_str(), ".vtk") )
      {
        this->WriteVTK (m_FileName.c_str());
      }
      else if ( CheckExtension(m_FileName.c_str(), ".nrrd") || CheckExtension(m_FileName.c_str(), ".nhdr") )
      {
        this->WriteNrrd (m_FileName.c_str());
      }
      else if ( CheckExtension(m_FileName.c_str(), ".inr.gz") )
      {
        this->WriteInrimage (m_FileName.c_str());
      }
      else if ( CheckExtension(m_FileName.c_str(), ".nii") || CheckExtension(m_FileName.c_str(), ".nii.gz") )
      {
        this->WriteNifti (m_FileName.c_str());
      }
      else if ( CheckExtension(m_FileName.c_str(), ".mha") || CheckExtension(m_FileName.c_str(), ".mhd") )
      {
        this->WriteMha (m_FileName.c_str());
      }
      else
      {
        throw itk::ExceptionObject (__FILE__,__LINE__,"Error: extension not recognized."
                                    " Supported extenstions are:\n-vtk\n-nrrd\n-nhdr\n-inr.gz\n-nii\n-nii.gz\n-mha\n-mhd.");
      }
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error in TensorImageIO::Write()");
    }
    
  }
  
  

  template<class T, unsigned int TensorDimension, unsigned int ImageDimension>
  void
  TensorImageIO<T,TensorDimension,ImageDimension>
  ::WriteVTK (const char* filename)
  {

    // VTK only supports 3D image with 3x3 tensors
    if( TensorDimension!=3 || ImageDimension!=3 )
    {
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error: VTK only supports 3D images and 3x3 tensors.");
    }
    
    
    typename TensorImageType::SizeType size = m_Input->GetLargestPossibleRegion().GetSize();
    int numVoxels = 1;
    int dims[3];
    double origin[3];
    double spacing[3];
    
    for(unsigned int i=0; i<3; i++)
    {
      numVoxels *= size[i];
      dims[i] = size[i];
      origin[i] = m_Input->GetOrigin()[i];
      spacing[i] = m_Input->GetSpacing()[i];
    }
    
    
    vtkDoubleArray* data = vtkDoubleArray::New();
    data->SetNumberOfComponents(9);
    data->SetNumberOfTuples(numVoxels);

    typedef ImageRegionConstIterator<TensorImageType> IteratorType;
    IteratorType it(m_Input,m_Input->GetLargestPossibleRegion());

    unsigned long step = numVoxels/100;
    unsigned int ind   = 0;

    this->UpdateProgress ( 0.0 );
    
    while( !it.IsAtEnd() )
    {
      TensorType tensor = it.Get();
      double buffer[9];
      buffer[0] = tensor.GetNthComponent(0);
      buffer[1] = tensor.GetNthComponent(1);
      buffer[2] = tensor.GetNthComponent(3);
      buffer[3] = tensor.GetNthComponent(1);
      buffer[4] = tensor.GetNthComponent(2);
      buffer[5] = tensor.GetNthComponent(4);
      buffer[6] = tensor.GetNthComponent(3);
      buffer[7] = tensor.GetNthComponent(4);
      buffer[8] = tensor.GetNthComponent(5);
      
      data->SetTuple(ind,buffer);
      ++ind;
      ++it;

      if( (ind%step)==0 )
      {
        this->UpdateProgress ( double(ind)/double(numVoxels) );
      }
    }

    this->UpdateProgress ( 1.0 );
    
    vtkStructuredPoints* tensors = vtkStructuredPoints::New();
    tensors->SetDimensions(dims);
    tensors->SetSpacing(spacing);
    tensors->SetOrigin(origin);
    tensors->GetPointData()->SetTensors(data);
    data->Delete();
    //tensors->Update();

    vtkStructuredPointsWriter* writer = vtkStructuredPointsWriter::New();
    writer->SetFileTypeToBinary();
    writer->SetFileName( m_FileName.c_str() );
    writer->SetInputData(tensors);
    writer->Write();

    tensors->Delete();
    writer->Delete();
  }


  

  template<class T, unsigned int TensorDimension, unsigned int ImageDimension>
  void
  TensorImageIO<T,TensorDimension,ImageDimension>
  ::WriteNrrd (const char* filename)
  {	

    typedef itk::Vector<double, DegreesOfFreedom>     VectorType;
    typedef itk::Image <VectorType, ImageDimension>    ImageType;

    typename ImageType::Pointer myTensorImage = ImageType::New();

    typename TensorImageType::RegionType region = m_Input->GetLargestPossibleRegion();

    myTensorImage->SetRegions (region);
    myTensorImage->SetSpacing (m_Input->GetSpacing());
    myTensorImage->SetOrigin (m_Input->GetOrigin());
    myTensorImage->SetDirection (m_Input->GetDirection());

    try
    {
      myTensorImage->Allocate();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error during memory allocation.");
    }


    typedef ImageRegionConstIterator<TensorImageType> IteratorType;
    IteratorType it(m_Input,m_Input->GetLargestPossibleRegion());
    
    itk::ImageRegionIteratorWithIndex<ImageType> itOut(myTensorImage, myTensorImage->GetLargestPossibleRegion());

    unsigned long numPixels = myTensorImage->GetLargestPossibleRegion().GetNumberOfPixels();
    unsigned long step = numPixels/100;
    unsigned int ind = 0;

    this->UpdateProgress ( 0.0 );
    
    while( !it.IsAtEnd() )
    {
      TensorType tensor = it.Get();
      VectorType vec;
      
      // storing convention is in line first
      for( unsigned int i=0; i<DegreesOfFreedom; i++)
      {
        vec[i] = static_cast<double>(tensor[i]);
      }    
     
      itOut.Set (vec);

      ++it;
      ++itOut;
      ++ind;

      if( (ind%step)==0 )
      {
        this->UpdateProgress ( double(ind)/double(numPixels) );
      }
      
    }

    this->UpdateProgress ( 1.0 );
    
    
    typename itk::ImageFileWriter<ImageType>::Pointer myWriter = itk::ImageFileWriter<ImageType>::New();
    myWriter->SetFileName(filename);
    myWriter->SetInput(myTensorImage);
    try
    {
		myWriter->Write();
    }
    catch(itk::ExceptionObject &e)
    {
		std::cerr << e;
		throw itk::ExceptionObject(__FILE__,__LINE__,"Error in TensorImageIO::WriteNrrd()");
    }

  }



  template<class T, unsigned int TensorDimension, unsigned int ImageDimension>
  void
  TensorImageIO<T,TensorDimension,ImageDimension>
  ::WriteNifti (const char* filename)
  {	
    
    typedef itk::Vector<T,      DegreesOfFreedom>    VectorType;
    typedef itk::Image<VectorType, ImageDimension>   VectorImageType;

    typename VectorImageType::Pointer myTensorImage = VectorImageType::New();
    
    typename TensorImageType::RegionType region = m_Input->GetLargestPossibleRegion();
    
    myTensorImage->SetRegions (region);
    myTensorImage->SetSpacing (m_Input->GetSpacing());
    myTensorImage->SetOrigin (m_Input->GetOrigin());
    myTensorImage->SetDirection (m_Input->GetDirection());

    try
    {
      myTensorImage->Allocate();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error during memory allocation.");
    }


    typedef ImageRegionConstIterator<TensorImageType> IteratorType;
    IteratorType it(m_Input,m_Input->GetLargestPossibleRegion());
    
    itk::ImageRegionIteratorWithIndex<VectorImageType> itOut(myTensorImage, myTensorImage->GetLargestPossibleRegion());

    unsigned long numPixels = myTensorImage->GetLargestPossibleRegion().GetNumberOfPixels();
    unsigned long step = numPixels/100;
    unsigned int ind = 0;

    this->UpdateProgress ( 0.0 );
    
    while( !it.IsAtEnd() )
    {
      TensorType tensor = it.Get();
      VectorType vec;
	  
      for( unsigned int i=0; i<DegreesOfFreedom; i++)
      {
        vec[i] = tensor[i];
      }
      
      itOut.Set (vec);
      
      ++it;
      ++itOut;
      ++ind;

      if( (ind%step)==0 )
      {
        this->UpdateProgress ( double(ind)/double(numPixels) );
      }
      
    }

    this->UpdateProgress ( 1.0 );
    
    
    typename itk::ImageFileWriter<VectorImageType>::Pointer myWriter = itk::ImageFileWriter<VectorImageType>::New();
    myWriter->SetFileName(filename);
    myWriter->SetInput(myTensorImage);
    try
    {
		myWriter->Write();
    }
    catch(itk::ExceptionObject &e)
    {
		std::cerr << e;
		throw itk::ExceptionObject(__FILE__,__LINE__,"Error in TensorImageIO::WriteNrrd()");
    }

  }


  template<class T, unsigned int TensorDimension, unsigned int ImageDimension>
  void
  TensorImageIO<T,TensorDimension,ImageDimension>
  ::WriteMha (const char* filename)
  {
    
    this->WriteNifti (filename);

  }
  
  

  
  template<class T, unsigned int TensorDimension, unsigned int ImageDimension>
  void
  TensorImageIO<T,TensorDimension,ImageDimension>
  ::WriteInrimage (const char* filename)
  {
#ifdef TTK_USE_MIPS
    SizeType size = m_Input->GetLargestPossibleRegion().GetSize();
    
    int numVox=1;
    for( unsigned int i=0; i<ImageDimension; i++)
    {
      numVox *= size[i];
    }
    
    if( numVox==0 )
    {
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Tensor field has null dimensions.");
    }
    
    SpacingType spacing = m_Input->GetSpacing();
    PointType origin = m_Input->GetOrigin();
    yav::Inrimage* inr = new yav::Inrimage(size[0],
                                 size[1],
                                 size[2],
                                 yav::Inrimage::WT_DOUBLE_VECTOR,
                                 TensorType::NDegreesOfFreedom,
                                 VM_INTERLACED,
                                 spacing[0],
                                 spacing[1],
                                 spacing[2]);
    // Completement debile: on est oblige de passer par
    // la structure Vec3 pour mettre l'origine de l'image
    // a jour. Y'en a qui ont encore pense tres fort pour
    // nous pondre quelque chose comme ca.
    yav::Vec3<double> orig;
    orig[0] = origin[0];
    orig[1] = origin[1];
    orig[2] = origin[2];
    inr->setTranslation(orig);

    
    // we only use the rotational part of the
    // inrimage transformation matrix, as the translation is in the origin.
    // note that we take the Transformation matrix instead of the rotation instance
    // of the inrimage as the rotation instance is in voxel coordinates.
    // Note also that it SHOULD be a rigid rotation matrix,
    // in theory its det. should be 1...
    
    DirectionType direction = m_Input->GetDirection();
    yav::Rotation3D matrix;    
    
    for (unsigned int i=0; i<3; i++)
      for (unsigned int j=0; j<3; j++)
	matrix[i][j] = direction[i][j];
    
    inr->setRotation(matrix);
    
    // Get the buffer
    double* buffer = (double*)inr->getData();

    typedef ImageRegionConstIterator<TensorImageType> IteratorType;
    IteratorType it (m_Input, m_Input->GetLargestPossibleRegion());

    unsigned long step = numVox/100;
    unsigned int ind = 0;

    this->UpdateProgress ( 0.0 );
    
    while(!it.IsAtEnd())
    {
      int offset = (int)(TensorType::NDegreesOfFreedom)*ind;
      TensorType Tens = it.Get();
      for(unsigned int i=0;i<(TensorType::NDegreesOfFreedom);i++)
      {
        buffer[offset + i] = Tens.GetNthComponent (i);
      }
      
      ++it;
      ++ind;

      if( (ind%step)==0 )
      {
        this->UpdateProgress ( double(ind)/double(numVox) );
      }
    }
    
    try
    {      
      inr->write(m_FileName.c_str());
    }
    catch (...)
    {      
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error while writing.");
    }

    this->UpdateProgress ( 1.0 );
    
    delete inr;
#else
	std::cerr << "TTK was not compiled with inrimage support, sorry!" << std::endl;
#endif
  }

}



#endif
