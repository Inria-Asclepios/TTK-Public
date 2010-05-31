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
