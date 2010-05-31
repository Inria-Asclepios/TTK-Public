#ifndef _itk_ResampleTensorImageCommand_h_
#define _itk_ResampleTensorImageCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class ResampleTensorImageCommand : public CommandObjectBase
  {
    
  public:
		
    typedef ResampleTensorImageCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(ResampleTensorImageCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "resample"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    ResampleTensorImageCommand();
    ~ResampleTensorImageCommand();
    
  private:
    ResampleTensorImageCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
