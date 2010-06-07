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
#ifndef _itk_NormalizeTensorsCommand_h_
#define _itk_NormalizeTensorsCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class NormalizeTensorsCommand : public CommandObjectBase
  {
    
  public:
		
    typedef NormalizeTensorsCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(NormalizeTensorsCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "normalize"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    NormalizeTensorsCommand();
    ~NormalizeTensorsCommand();
    
  private:
    NormalizeTensorsCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
