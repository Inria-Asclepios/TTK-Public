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
#ifndef _itk_Average4DImageCommand_h_
#define _itk_Average4DImageCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class Average4DImageCommand : public CommandObjectBase
  {
    
  public:
		
    typedef Average4DImageCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(Average4DImageCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "average4d"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    Average4DImageCommand();
    ~Average4DImageCommand();
    
  private:
    Average4DImageCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
