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
#ifndef _itk_DemonsTensorRegistrationCommand_h_
#define _itk_DemonsTensorRegistrationCommand_h_

#include "itkCommandObjectBase.h"

struct DemonsTensorRegistrationCommandArguments;

namespace itk {

  class DemonsTensorRegistrationCommand : public CommandObjectBase
  {
    
  public:
		
    typedef DemonsTensorRegistrationCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(DemonsTensorRegistrationCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "register_demons"; }

    int atoi_check(const char * str);
    std::vector<unsigned int> parseUIntVector(const std::string & str);
    void parseOpts(int argc, const char **argv, struct DemonsTensorRegistrationCommandArguments & args);
      
    int Execute(int nargs, const char *args[]);
    
  protected:
    DemonsTensorRegistrationCommand();
    ~DemonsTensorRegistrationCommand();
    
  private:
    DemonsTensorRegistrationCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
