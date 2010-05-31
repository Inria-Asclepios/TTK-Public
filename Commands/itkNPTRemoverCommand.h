#ifndef _itk_NPTRemoverCommand_h_
#define _itk_NPTRemoverCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class NPTRemoverCommand : public CommandObjectBase
  {
    
  public:
		
    typedef NPTRemoverCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(NPTRemoverCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "npt_remover"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    NPTRemoverCommand();
    ~NPTRemoverCommand();
    
  private:
    NPTRemoverCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
