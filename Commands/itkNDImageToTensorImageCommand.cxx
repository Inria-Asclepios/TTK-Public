/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: $
  Language:  C++
  Date:      $Date: 2010-08-17 09:58:00 +0200 (mar., 17 août 2010) $
  Version:   $Revision: 97 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkNDImageToTensorImageCommand.h"

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageRegionConstIteratorWithIndex.h>
#include <itkImageRegionIteratorWithIndex.h>

#include "itkTensor.h"
#include "itkTensorImageIO.h"

#include <iostream>
#include "GetPot.h"
#include <errno.h>

int atoi_check(const char * str)
{
  char *endptr;
  long val = strtol(str, &endptr, 0);

  /* Check for various possible errors */
  if ( (errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || val
      >= INT_MAX || val <= INT_MIN )
    {
      std::cout << std::endl;
      std::cout << "Cannot parse integer. Out of bounds." << std::endl;
      exit(EXIT_FAILURE);
    }

  if ( endptr == str || *endptr != '\0' )
    {
      std::cout << std::endl;
      std::cout << "Cannot parse integer. Contains non-digits or is empty."
          << std::endl;
      exit(EXIT_FAILURE);
    }

  return val;
}

std::vector<unsigned int> parseUIntVector(const std::string & str)
{
  std::vector<unsigned int> vect;

  std::string::size_type crosspos = str.find('x', 0);

  if ( crosspos == std::string::npos )
    {
      // only one uint
      vect.push_back(static_cast<unsigned int> (atoi_check(str.c_str())));
      return vect;
    }

  // first uint
  vect.push_back(static_cast<unsigned int> (atoi_check(
      (str.substr(0, crosspos)).c_str())));

  while ( true )
    {
      std::string::size_type crossposfrom = crosspos;
      crosspos = str.find('x', crossposfrom + 1);

      if ( crosspos == std::string::npos )
        {
          vect.push_back(static_cast<unsigned int> (atoi_check((str.substr(
              crossposfrom + 1, str.length() - crossposfrom - 1)).c_str())));
          return vect;
        }

      vect.push_back(static_cast<unsigned int> (atoi_check((str.substr(
          crossposfrom + 1, crosspos - crossposfrom - 1)).c_str())));
    }
}

namespace itk
{
    NDImageToTensorImageCommand::NDImageToTensorImageCommand()
    {
      m_ShortDescription = "Convert a 4D image to a ttk compatible tensor file";
      m_LongDescription = "Usage:\n";
      m_LongDescription += "<-i input> <-n 4D to tensor index (default 0x1x2x3x4x5)> <-s scale factor (default: 1.0)> <-o output>\n\n";
      m_LongDescription += m_ShortDescription;
      // 0x3x1x5x4x2
    }

    NDImageToTensorImageCommand::~NDImageToTensorImageCommand()
    {
    }

    int NDImageToTensorImageCommand::Execute (int narg, const char* arg[])
    {
    
      GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
      if( cl.size() == 1 || cl.search(2, "--help", "-h") )
      {
        std::cout << this->GetLongDescription() << std::endl;
        return -1;
      }

      const char* input   = cl.follow ("", 2, "-i", "-I");
      const char* output  = cl.follow ("output.nii.gz", 2, "-o", "-O");
      double scale        = cl.follow (1.0, 2, "-s", "-S");
      std::vector<unsigned int> ndToTensorIndices = 
          parseUIntVector ( cl.follow ("0x1x2x3x4x5", 2, "-n", "-N"));

      if (ndToTensorIndices.size()!=6) 
      {
        std::cerr << "error: length of 4D to tensor indices is different than 6";
        return -1;
      }

      typedef float ScalarType;
      typedef Image<ScalarType, 4>        ImageType;
      typedef ImageFileReader <ImageType> ImageReaderType;

      ImageType::Pointer image = 0;
      {
        ImageReaderType::Pointer reader = ImageReaderType::New();
        reader->SetFileName (input);
        try
        {
          reader->Update();
        }
        catch (ExceptionObject &e)
        {
          std::cerr << e;
          return -1;
        }
        image = reader->GetOutput();
      }

      if (image->GetLargestPossibleRegion().GetSize(3)<6)
      {
        std::cerr << "image 4th dimension is less than 6, cannot convert";
        return -1;
      }

      // create a tensor image
      typedef Tensor<ScalarType, 3> TensorType;
      typedef Image<TensorType, 3>  TensorImageType;

      TensorImageType::Pointer tensorImage = TensorImageType::New();
      TensorImageType::PointType origin;
      TensorImageType::SpacingType spacing;
      TensorImageType::DirectionType direction;
      for (int i=0; i<3; i++) 
      {
        origin[i] = image->GetOrigin()[i];
        spacing[i] = image->GetSpacing()[i];
        for (int j=0; j<3; j++)
            direction (i, j) = image->GetDirection()(i,j);
      }
      tensorImage->SetOrigin (origin);
      tensorImage->SetSpacing (spacing);
      tensorImage->SetDirection (direction);
      TensorImageType::RegionType region;
      for (int i=0; i<3; i++)
          region.SetSize (i, image->GetLargestPossibleRegion().GetSize(i));
      tensorImage->SetRegions (region);
      try
      {
        tensorImage->Allocate();
      }
      catch (ExceptionObject &e)
      {
        std::cerr << e;
        return -1;
      }

      typedef ImageRegionIteratorWithIndex <TensorImageType> TensorImageIteratorType;
      typedef ImageType::IndexType ImageIndexType;

      TensorImageIteratorType it (tensorImage, tensorImage->GetLargestPossibleRegion());

      while (!it.IsAtEnd())
      {
          TensorType t;
          ImageIndexType index;
          index.SetElement (0, it.GetIndex().GetElement(0));
          index.SetElement (1, it.GetIndex().GetElement(1));
          index.SetElement (2, it.GetIndex().GetElement(2));

          index.SetElement (3, ndToTensorIndices[0]);
          t[0] = image->GetPixel (index) * scale;
          index.SetElement (3, ndToTensorIndices[1]);
          t[1] = image->GetPixel (index) * scale;
          index.SetElement (3, ndToTensorIndices[2]);
          t[2] = image->GetPixel (index) * scale;
          index.SetElement (3, ndToTensorIndices[3]);
          t[3] = image->GetPixel (index) * scale;
          index.SetElement (3, ndToTensorIndices[4]);
          t[4] = image->GetPixel (index) * scale;
          index.SetElement (3, ndToTensorIndices[5]);
          t[5] = image->GetPixel (index) * scale;

          it.Set(t);
      
          ++it;
      }

      typedef TensorImageIO<ScalarType, 3, 3> TensorIOType;

      {
        TensorIOType::Pointer writer = TensorIOType::New();
        writer->SetInput (tensorImage);
        writer->SetFileName (output);
        try
        {
          writer->Write();
        }
        catch (ExceptionObject &e)
        {
          std::cerr << e;
          return -1;
        }
      }

      return 0;
    }

} // end of namespace
