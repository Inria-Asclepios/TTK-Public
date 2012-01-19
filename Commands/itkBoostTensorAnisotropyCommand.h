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
#ifndef _itk_BoostTensorAnisotropyCommand_h_
#define _itk_BoostTensorAnisotropyCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class BoostTensorAnisotropyCommand : public CommandObjectBase
  {
    
  public:
		
    typedef BoostTensorAnisotropyCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(BoostTensorAnisotropyCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "boost_ani"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    BoostTensorAnisotropyCommand();
    ~BoostTensorAnisotropyCommand();
    
  private:
    BoostTensorAnisotropyCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
