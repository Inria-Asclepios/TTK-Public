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
#ifndef _itk_DWIBrainMaskExtractorCommand_h_
#define _itk_DWIBrainMaskExtractorCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class DWIBrainMaskExtractorCommand : public CommandObjectBase
  {
    
  public:
		
    using Self         = DWIBrainMaskExtractorCommand;
    using Superclass   = CommandObjectBase;
    using Pointer      = SmartPointer <Self>;
    using ConstPointer = SmartPointer <const Self>;
    
    itkTypeMacro(DWIBrainMaskExtractorCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName() override
    { return "dwi_mask_extract"; }
    
    int Execute(int nargs, const char *args[]) override;
    
  protected:
    DWIBrainMaskExtractorCommand();
    ~DWIBrainMaskExtractorCommand() override;
    
  private:
    DWIBrainMaskExtractorCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
