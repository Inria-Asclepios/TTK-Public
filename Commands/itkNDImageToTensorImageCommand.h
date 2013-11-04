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
#ifndef _itk_NDImageToTensorImageCommand_h_
#define _itk_NDImageToTensorImageCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class NDImageToTensorImageCommand : public CommandObjectBase
  {

  public:
    typedef NDImageToTensorImageCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(NDImageToTensorImageCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "4dtotensor"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
     NDImageToTensorImageCommand();
    ~NDImageToTensorImageCommand();
    
  private:
    NDImageToTensorImageCommand(const Self&);
    void operator=(const Self&);
  };
}

#endif
