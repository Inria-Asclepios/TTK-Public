#ifndef _itk_FiberTrackingCommand_h_
#define _itk_FiberTrackingCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class FiberTrackingCommand : public CommandObjectBase
  {
    
  public:
		
    typedef FiberTrackingCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(FiberTrackingCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "tractography"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    FiberTrackingCommand();
    ~FiberTrackingCommand();
    
  private:
    FiberTrackingCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
