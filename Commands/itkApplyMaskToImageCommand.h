#ifndef _itk_ApplyMaskToImageCommand_h_
#define _itk_ApplyMaskToImageCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class ApplyMaskToImageCommand : public CommandObjectBase
  {
    
  public:
		
    typedef ApplyMaskToImageCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(ApplyMaskToImageCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "apply_mask"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    ApplyMaskToImageCommand();
    ~ApplyMaskToImageCommand();
    
  private:
    ApplyMaskToImageCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
