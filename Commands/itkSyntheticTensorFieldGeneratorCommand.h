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
#ifndef _itk_SyntheticTensorFieldGeneratorCommand_h_
#define _itk_SyntheticTensorFieldGeneratorCommand_h_

#include "itkCommandObjectBase.h"
#include <cstring>

namespace itk {

  class SyntheticTensorFieldGeneratorCommand : public CommandObjectBase
  {
    
  public:
		
    typedef SyntheticTensorFieldGeneratorCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(SyntheticTensorFieldGeneratorCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "generate"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    SyntheticTensorFieldGeneratorCommand();
    ~SyntheticTensorFieldGeneratorCommand();
    
  private:
    SyntheticTensorFieldGeneratorCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
