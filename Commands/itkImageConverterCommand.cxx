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
#include "itkImageConverterCommand.h"

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <itkImageIOBase.h>
#include <itkNiftiImageIOFactory.h>
#include <itkMetaDataDictionary.h>
#include <itkMetaDataObject.h>

#include "ttkConfigure.h"

#ifdef TTK_USE_MIPS
#include "mipsInrimageImageIOFactory.h"
#endif

#ifdef TTK_USE_VTKINRIA3D
#include "itkGISImageIOFactory.h"
#endif


#ifdef Module_ITKIOPhilipsREC
#include <itkPhilipsRECImageIOFactory.h>
#include <itkPhilipsRECImageIO.h>
#include <itkPhilipsPAR.h>
#endif

// macro for templated ITK read and write
#define ReadWriteImageMacro(dimension, type)				\
  using CorrectImageType = itk::Image<type, dimension> ;			\
  itk::ImageFileReader< CorrectImageType >::Pointer Reader = itk::ImageFileReader< itk::Image<type, dimension> >::New(); \
  Reader->SetFileName ( filename1 );					\
  try									\
  {									\
    Reader->Update();							\
  }									\
  catch (itk::ExceptionObject &e) {					\
    std::cerr << e;							\
    return EXIT_FAILURE;						\
  }									\
  if (isparrec)								\
  {									\
    using CorrectDirectionType = CorrectImageType::DirectionType ;	\
    using CorrectPointType     = CorrectImageType::PointType ;		\
    CorrectDirectionType direction;					\
    direction.SetIdentity();						\
    for (unsigned int i=0; i<3; i++)					\
      for (unsigned int j=0; j<3; j++)					\
	direction[i][j] = correctdirection[i][j];			\
    CorrectPointType origin;						\
    origin.Fill(0.0);							\
    for (unsigned int i=0; i<3; i++)					\
      origin[i] = correctorigin[i];					\
    Reader->GetOutput()->SetDirection (direction);			\
    Reader->GetOutput()->SetOrigin (origin);				\
  }									\
  itk::ImageFileWriter< itk::Image<type, dimension> >::Pointer Writer = itk::ImageFileWriter< itk::Image<type, dimension> >::New(); \
  Writer->SetFileName ( filename2 );					\
  Writer->SetInput(Reader->GetOutput());				\
  try									\
  {									\
    Writer->Update();							\
  }									\
  catch (itk::ExceptionObject &e) {					\
    std::cerr << e;							\
    return EXIT_FAILURE;						\
  }									\

namespace itk
{

  ImageConverterCommand::ImageConverterCommand()
  {
    m_ShortDescription = "Convert an image file into another format";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "<input image> <output image>\n\n";
    m_LongDescription += m_ShortDescription;
  }

  ImageConverterCommand::~ImageConverterCommand() = default;

  int ImageConverterCommand::Execute(int narg, const char* arg[])
  {
  
    if(narg!=3)
    {
      std::cout << this->GetLongDescription() << std::endl;
      return EXIT_FAILURE;
    }
  
    const char* filename1 = arg[1];
    const char* filename2 = arg[2];
    
    itk::NiftiImageIOFactory::RegisterOneFactory();
#ifdef TTK_USE_MIPS
    itk::InrimageImageIOFactory::RegisterOneFactory();
#endif
#ifdef Module_ITKIOPhilipsREC
  itk::PhilipsRECImageIOFactory::RegisterOneFactory();
#endif
#ifdef TTK_USE_VTKINRIA3D
    itk::GISImageIOFactory::RegisterOneFactory();
#endif
  
    // dummy reading information to get type and dimension
    using ImageType  = FloatImageType;
    using ReaderType = itk::ImageFileReader<ImageType>;
    ReaderType::Pointer informationreader = ReaderType::New();
    informationreader->SetFileName(filename1);
    try
    {
      informationreader->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return EXIT_FAILURE;
    }
    
    std::cerr << "Dimension: " << informationreader->GetImageIO()->GetNumberOfDimensions() << std::endl;
    std::cerr << "Component Type: " << informationreader->GetImageIO()->GetComponentTypeAsString(informationreader->GetImageIO()->GetComponentType()) << std::endl;
    std::cerr << "Pixel Type: " << informationreader->GetImageIO()->GetPixelTypeAsString(informationreader->GetImageIO()->GetPixelType()) << std::endl;

    if(informationreader->GetImageIO()->GetPixelType() != itk::ImageIOBase::SCALAR)
    {
      std::cerr << "Only scalar images can be converted" << std::endl;
      return EXIT_FAILURE;
    }
    
    // real reading with dimension and type
    unsigned int dim = informationreader->GetImageIO()->GetNumberOfDimensions();
    itk::ImageIOBase::IOComponentType componenttype = informationreader->GetImageIO()->GetComponentType();

    bool isparrec = !strcmp (informationreader->GetImageIO()->GetNameOfClass(), "PhilipsRECImageIO");
    FloatImageType::DirectionType correctdirection;
    FloatImageType::PointType correctorigin = 0.0;
    if (isparrec)
    {
      correctdirection = this->ExtractPARRECImageOrientation(filename1);
      correctorigin = this->ExtractPARRECImageOrigin (filename1, correctdirection);
    }
    
    if (dim > 6)
    { std::cerr << "Dimension Too High." << dim<< std::endl; return -1; }

    switch (componenttype)
    {
	case itk::ImageIOBase::UCHAR:
	  if      (dim == 2) { ReadWriteImageMacro (2, unsigned char); }
	  else if (dim == 3) { ReadWriteImageMacro (3, unsigned char); }
	  else if (dim == 4) { ReadWriteImageMacro (4, unsigned char); }
	  else if (dim == 5) { ReadWriteImageMacro (5, unsigned char); }
	  else if (dim == 6) { ReadWriteImageMacro (6, unsigned char); }
	  break;
	case itk::ImageIOBase::CHAR:
	  if      (dim == 2) { ReadWriteImageMacro (2, char); }
	  else if (dim == 3) { ReadWriteImageMacro (3, char); }
	  else if (dim == 4) { ReadWriteImageMacro (4, char); }
	  else if (dim == 5) { ReadWriteImageMacro (5, char); }
	  else if (dim == 6) { ReadWriteImageMacro (6, char); }
	  break;
    	case itk::ImageIOBase::USHORT:
	  if      (dim == 2) { ReadWriteImageMacro (2, unsigned short); }
	  else if (dim == 3) { ReadWriteImageMacro (3, unsigned short); }
	  else if (dim == 4) { ReadWriteImageMacro (4, unsigned short); }
	  else if (dim == 5) { ReadWriteImageMacro (5, unsigned short); }
	  else if (dim == 6) { ReadWriteImageMacro (6, unsigned short); }
	  break;
    	case itk::ImageIOBase::SHORT:
	  if      (dim == 2) { ReadWriteImageMacro (2, short); }
	  else if (dim == 3) { ReadWriteImageMacro (3, short); }
	  else if (dim == 4) { ReadWriteImageMacro (4, short); }
	  else if (dim == 5) { ReadWriteImageMacro (5, short); }
	  else if (dim == 6) { ReadWriteImageMacro (6, short); }
	  break;
    	case itk::ImageIOBase::UINT:
	  if      (dim == 2) { ReadWriteImageMacro (2, unsigned int); }
	  else if (dim == 3) { ReadWriteImageMacro (3, unsigned int); }
	  else if (dim == 4) { ReadWriteImageMacro (4, unsigned int); }
	  else if (dim == 5) { ReadWriteImageMacro (5, unsigned int); }
	  else if (dim == 6) { ReadWriteImageMacro (6, unsigned int); }
	  break;
    	case itk::ImageIOBase::INT:
	  if      (dim == 2) { ReadWriteImageMacro (2, int); }
	  else if (dim == 3) { ReadWriteImageMacro (3, int); }
	  else if (dim == 4) { ReadWriteImageMacro (4, int); }
	  else if (dim == 5) { ReadWriteImageMacro (5, int); }
	  else if (dim == 6) { ReadWriteImageMacro (6, int); }
	  break;
    	case itk::ImageIOBase::ULONG:
	  if      (dim == 2) { ReadWriteImageMacro (2, unsigned long); }
	  else if (dim == 3) { ReadWriteImageMacro (3, unsigned long); }
	  else if (dim == 4) { ReadWriteImageMacro (4, unsigned long); }
	  else if (dim == 5) { ReadWriteImageMacro (5, unsigned long); }
	  else if (dim == 6) { ReadWriteImageMacro (6, unsigned long); }
	  break;
	case itk::ImageIOBase::LONG:
	  if      (dim == 2) { ReadWriteImageMacro (2, long); }
	  else if (dim == 3) { ReadWriteImageMacro (3, long); }
	  else if (dim == 4) { ReadWriteImageMacro (4, long); }
	  else if (dim == 5) { ReadWriteImageMacro (5, long); }
	  else if (dim == 6) { ReadWriteImageMacro (6, long); }
	  break;
	case itk::ImageIOBase::FLOAT:
	  if      (dim == 2) { ReadWriteImageMacro (2, float); }
	  else if (dim == 3) { ReadWriteImageMacro (3, float); }
	  else if (dim == 4) { ReadWriteImageMacro (4, float); }
	  else if (dim == 5) { ReadWriteImageMacro (5, float); }
	  else if (dim == 6) { ReadWriteImageMacro (6, float); }
	  break;
    	case itk::ImageIOBase::DOUBLE:
	  if      (dim == 2) { ReadWriteImageMacro (2, double); }
	  else if (dim == 3) { ReadWriteImageMacro (3, double); }
	  else if (dim == 4) { ReadWriteImageMacro (4, double); }
	  else if (dim == 5) { ReadWriteImageMacro (5, double); }
	  else if (dim == 6) { ReadWriteImageMacro (6, double); }
	  break;
    	default:
    	  std::cerr << "Unrecognized Component Type : " << informationreader->GetImageIO()->GetComponentTypeAsString(componenttype) << std::endl;
    	  return -1;
    	  break;  
    }

    if (isparrec)
    {
      std::ostringstream gradientfilename;
      gradientfilename << itksys::SystemTools::GetFilenameWithoutExtension (filename2)
		       <<".grad";
      GradientWriterType::Pointer gradientwriter = GradientWriterType::New();
      gradientwriter->SetGradientList (this->ExtractPARRECGradientDirections (filename1, correctdirection));
      gradientwriter->SetFileName (gradientfilename.str().c_str());
      try
      {
	gradientwriter->Update();
      }
      catch(itk::ExceptionObject &e)
      {
	std::cerr << e;
	return EXIT_FAILURE;
      }
    }    
    
    return EXIT_SUCCESS;
    
  }
  
  ImageConverterCommand::FloatImageType::PointType ImageConverterCommand::ExtractPARRECImageOrigin (const char* filename, FloatImageType::DirectionType direction)
  {
    
    using PointType = FloatImageType::PointType;
    PointType nullptrorigin;
    nullptrorigin[0] = nullptrorigin[1] = nullptrorigin[2] = 0.0;
    
#ifndef Module_ITKIOPhilipsREC
    std::cerr<<"cannot correct for PAR-REC angulation without Module_ITKIOPhilipsREC to ON"<<std::endl;
    return nullptrorigin;
#else
    
    
    itk::PhilipsRECImageIO::Pointer philipsIO = itk::PhilipsRECImageIO::New();
    
    philipsIO->SetFileName(filename);
    try
    {
      philipsIO->ReadImageInformation();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
    }
    
    itk::MetaDataDictionary PARheader = philipsIO->GetMetaDataDictionary();
    
    using OffCentreType = itk::PhilipsRECImageIO::OffCentreMidSliceType ;
    
    OffCentreType offcenter;
    
    bool valid = itk::ExposeMetaData<OffCentreType>(PARheader, "PAR_OffCentreMidSlice", offcenter);
    if (!valid)
    {
      std::cerr<<"cannot find off-center information in PAR header, no correction"<<std::endl;
      return nullptrorigin;
    }
    
    double dimensions[3];
    dimensions[0] = philipsIO->GetDimensions (0);
    dimensions[1] = philipsIO->GetDimensions (1);
    dimensions[2] = philipsIO->GetDimensions (2);
    
    FloatImageType::SpacingType midoffset;
    midoffset[0] = philipsIO->GetSpacing (0) * (dimensions[0] - 1) / 2.0;
    midoffset[1] = philipsIO->GetSpacing (1) * (dimensions[1] - 1) / 2.0;
    midoffset[2] = philipsIO->GetSpacing (2) * (dimensions[2] - 1) / 2.0;
    midoffset = direction * midoffset;
    
    PointType offcenterpoint;
    offcenterpoint[0] = offcenter[0];
    offcenterpoint[1] = offcenter[1];
    offcenterpoint[2] = offcenter[2];
    
    FloatImageType::DirectionType AFRtoLPS;
    AFRtoLPS.Fill (0);
    AFRtoLPS[0][2] = 1;
    AFRtoLPS[1][0] = 1;
    AFRtoLPS[2][1] = 1;
    
    offcenterpoint = AFRtoLPS * offcenterpoint;
    offcenterpoint -= midoffset;
    
    return offcenterpoint;
  
#endif
  }

  
  ImageConverterCommand::FloatImageType::DirectionType ImageConverterCommand::ExtractPARRECImageOrientation (const char* filename)
  {
    
    using DirectionType = FloatImageType::DirectionType ;
    
    DirectionType eyedir;
    eyedir.SetIdentity();
    
#ifndef Module_ITKIOPhilipsREC
    std::cerr<<"cannot correct for PAR-REC angulation without Module_ITKIOPhilipsREC to ON"<<std::endl;
    return eyedir;
#else
    
    itk::PhilipsRECImageIO::Pointer philipsIO = itk::PhilipsRECImageIO::New();
    
    philipsIO->SetFileName(filename);
    try
    {
      philipsIO->ReadImageInformation();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
    }
    
    itk::MetaDataDictionary PARheader = philipsIO->GetMetaDataDictionary();
    
    using AngulationType = itk::PhilipsRECImageIO::AngulationMidSliceType ;
    
    AngulationType angulation;
    int sliceorientation = 0;
    
    bool valid = itk::ExposeMetaData<AngulationType>(PARheader, "PAR_AngulationMidSlice", angulation);
    if (!valid)
    {
      std::cerr<<"cannot find angulation in PAR header, no correction"<<std::endl;
      return eyedir;
    }
    
    valid = itk::ExposeMetaData<int>(PARheader, "PAR_SliceOrientation", sliceorientation);
    if (!valid)
    {
      std::cerr<<"cannot find slice orientation in PAR header, no correction"<<std::endl;
      return eyedir;
    }
    
    DirectionType AFRtoLPS;
    AFRtoLPS.Fill (0);
    AFRtoLPS[0][2] = 1;
    AFRtoLPS[1][0] = 1;
    AFRtoLPS[2][1] = 1;
    
    DirectionType magicmatrix;
    magicmatrix.Fill (0);
    magicmatrix [0][0] = -1;
    magicmatrix [1][2] = 1;
    magicmatrix [2][1] = -1;
    
    DirectionType TRA;
    TRA.Fill (0);
    TRA [0][1] = 1;
    TRA [1][0] = -1;
    TRA [2][2] = -1;
    DirectionType SAG;
    SAG.Fill (0);
    SAG [0][0] = -1;
    SAG [1][2] = 1;
    SAG [2][1] = -1;
    DirectionType COR;
    COR.Fill (0);
    COR [0][1] = 1;
    COR [1][2] = 1;
    COR [2][0] = -1;
    
    
    DirectionType Torientation;
    
    switch(sliceorientation)
    {
      
	case PAR_SLICE_ORIENTATION_TRANSVERSAL: 
	  // Transverse - the REC data appears to be stored as right-left, 
	  // anterior-posterior, and inferior-superior.
	  // Verified using a marker on right side of brain.
	  Torientation = TRA;      
	  break;
	case PAR_SLICE_ORIENTATION_SAGITTAL: 
	  // Sagittal - the REC data appears to be stored as anterior-posterior, 
	  // superior-inferior, and right-left.
	  // Verified using marker on right side of brain.
	  Torientation = SAG;
	  break;
	case PAR_SLICE_ORIENTATION_CORONAL: 
	  // Coronal - the REC data appears to be stored as right-left, 
	  // superior-inferior, and anterior-posterior.
	  // Verified using marker on right side of brain.
	  // fall thru
	default:
	  Torientation = COR;
	  
    }
    
    
    double ap = angulation[0] * vnl_math::pi / 180.0;
    double fh = angulation[1] * vnl_math::pi / 180.0;
    double rl = angulation[2] * vnl_math::pi / 180.0;
    
    DirectionType Tap;
    Tap.Fill (0);
    Tap[0][0] = 1;
    Tap[1][1] = std::cos (ap);
    Tap[1][2] = - std::sin (ap);
    Tap[2][1] = std::sin (ap);
    Tap[2][2] = std::cos (ap);
    
    DirectionType Tfh;
    Tfh.Fill (0);
    Tfh[1][1] = 1;
    Tfh[0][0] = std::cos (fh);
    Tfh[0][2] = std::sin (fh);
    Tfh[2][0] = - std::sin (fh);
    Tfh[2][2] = std::cos (fh);
    
    DirectionType Trl;
    Trl.Fill (0);
    Trl[2][2] = 1;
    Trl[0][0] = std::cos (rl);
    Trl[0][1] = - std::sin (rl);
    Trl[1][0] = std::sin (rl);
    Trl[1][1] = std::cos (rl);
    
    DirectionType TR = AFRtoLPS * Trl * Tap * Tfh * magicmatrix.GetTranspose() * Torientation.GetTranspose();
    DirectionType retval;
    retval.SetIdentity();
    
    for (unsigned int i=0; i<3; i++)
      for (unsigned int j=0; j<3; j++)
	retval[i][j] = TR[i][j];
    
    return retval;
    
#endif
  }

  ImageConverterCommand::VectorListType ImageConverterCommand::ExtractPARRECGradientDirections (const char* filename, FloatImageType::DirectionType direction)
  {
    
    VectorListType gradients;
    
#ifndef Module_ITKIOPhilipsREC
    std::cerr<<"cannot correct for PAR-REC gradients without Module_ITKIOPhilipsREC to ON"<<std::endl;
    return gradients;
#else
    
    itk::PhilipsRECImageIO::Pointer philipsIO = itk::PhilipsRECImageIO::New();
    
    philipsIO->SetFileName(filename);
    try
    {
      philipsIO->ReadImageInformation();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
    }
    
    itk::MetaDataDictionary PARheader = philipsIO->GetMetaDataDictionary();
    
    using GradientDirectionType = itk::PhilipsRECImageIO::GradientDirectionType ;
    using GradientDirectionContainerType = itk::PhilipsRECImageIO::GradientDirectionContainerType ;
    
    GradientDirectionContainerType::Pointer parrecgradients = GradientDirectionContainerType::New();
    
    bool valid = itk::ExposeMetaData<GradientDirectionContainerType::Pointer>(PARheader, "PAR_GradientDirectionValues", parrecgradients);
    if (!valid)
    {
      std::cerr<<"cannot find gradient information in PAR header..."<<std::endl;
      return gradients;
    }

    
    FloatImageType::DirectionType inverse =  FloatImageType::DirectionType (direction.GetInverse());
    
    FloatImageType::DirectionType AFRtoLPS;
    AFRtoLPS.Fill (0);
    AFRtoLPS[0][2] = 1;
    AFRtoLPS[1][0] = 1;
    AFRtoLPS[2][1] = 1;
    
    for (unsigned int i=0; i<parrecgradients->Size(); i++)
    {
      GradientDirectionType parrecgradient = parrecgradients->ElementAt (i);
      VectorType gradient;
      for (unsigned int j=0; j<3; j++) gradient[j] = parrecgradient[j];
      gradient = inverse * AFRtoLPS * gradient;
      gradients.push_back (gradient);
    }
    
    return gradients;
  
#endif
  }

  
  
}
