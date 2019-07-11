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
#ifndef _itk_ImageConverterCommand_h_
#define _itk_ImageConverterCommand_h_

#include "itkCommandObjectBase.h"
#include "itkImage.h"

#include "itkGradientFileWriter.h"


namespace itk {

  class ImageConverterCommand : public CommandObjectBase
  {
    
  public:
		
    using Self               = ImageConverterCommand;
    using Superclass         = CommandObjectBase;
    using Pointer            = SmartPointer <Self>;
    using ConstPointer       = SmartPointer <const Self>;
    using GradientWriterType = GradientFileWriter;
    using VectorListType     = GradientWriterType::VectorListType;
    using VectorType         = GradientWriterType::VectorType;
    
    itkTypeMacro(ImageConverterCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName() override
    { return "image"; }
    
    int Execute(int nargs, const char *args[]) override;

    using FloatImageType = Image<float,3>;
    
  protected:
    ImageConverterCommand();
    ~ImageConverterCommand() override;

    FloatImageType::DirectionType ExtractPARRECImageOrientation (const char* filename);
    FloatImageType::PointType ExtractPARRECImageOrigin (const char* filename, FloatImageType::DirectionType direction);
    VectorListType ExtractPARRECGradientDirections (const char* filename, FloatImageType::DirectionType direction);
    
  private:
    ImageConverterCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
