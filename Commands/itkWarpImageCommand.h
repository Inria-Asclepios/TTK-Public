#ifndef _itk_WarpImageCommand_h_
#define _itk_WarpImageCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class WarpImageCommand : public CommandObjectBase
  {
    
  public:
		
    typedef WarpImageCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(WarpImageCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "warp_image"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    WarpImageCommand();
    ~WarpImageCommand();
    
  private:
    WarpImageCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
