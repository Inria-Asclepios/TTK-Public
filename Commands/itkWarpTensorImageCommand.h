#ifndef _itk_WarpTensorImageCommand_h_
#define _itk_WarpTensorImageCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class WarpTensorImageCommand : public CommandObjectBase
  {
    
  public:
		
    typedef WarpTensorImageCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(WarpTensorImageCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "warp"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    WarpTensorImageCommand();
    ~WarpTensorImageCommand();
    
  private:
    WarpTensorImageCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
