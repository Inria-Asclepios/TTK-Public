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
#ifndef _itk_LogDomainDemonsTensorRegistrationCommand_h_
#define _itk_LogDomainDemonsTensorRegistrationCommand_h_

#include "itkCommandObjectBase.h"

struct LogDomainDemonsTensorRegistrationCommandArguments;

namespace itk {

  class LogDomainDemonsTensorRegistrationCommand : public CommandObjectBase
  {
    
  public:
		
    typedef LogDomainDemonsTensorRegistrationCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(LogDomainDemonsTensorRegistrationCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "register_log_symmetric_demons"; }

    int atoi_check (const char *str);
    std::vector<unsigned int> parseUIntVector(const std::string & str);
    void parseOpts(int argc, const char **argv, struct LogDomainDemonsTensorRegistrationCommandArguments & args);
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    LogDomainDemonsTensorRegistrationCommand();
    ~LogDomainDemonsTensorRegistrationCommand();
    
  private:
    LogDomainDemonsTensorRegistrationCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
