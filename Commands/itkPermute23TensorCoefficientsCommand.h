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
#ifndef _itk_Permute23TensorCoefficientsCommand_h_
#define _itk_Permute23TensorCoefficientsCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class Permute23TensorCoefficientsCommand : public CommandObjectBase
  {
    
  public:
		
    typedef Permute23TensorCoefficientsCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(Permute23TensorCoefficientsCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "permute23"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    Permute23TensorCoefficientsCommand();
    ~Permute23TensorCoefficientsCommand();
    
  private:
    Permute23TensorCoefficientsCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
