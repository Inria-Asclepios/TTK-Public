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
#ifndef _itk_WarpImageCommand_h_
#define _itk_WarpImageCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class WarpImageCommand : public CommandObjectBase
  {
    
  public:
		
    typedef WarpImageCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(WarpImageCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "warp_image"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    WarpImageCommand();
    ~WarpImageCommand();
    
  private:
    WarpImageCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
