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
#ifndef _itk_WarpFibersCommand_h_
#define _itk_WarpFibersCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class WarpFibersCommand : public CommandObjectBase
  {
    
  public:
		
    typedef WarpFibersCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(WarpFibersCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "warp_fibers"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    WarpFibersCommand();
    ~WarpFibersCommand();
    
  private:
    WarpFibersCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
