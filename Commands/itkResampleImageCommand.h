#ifndef _itk_ResampleImageCommand_h_
#define _itk_ResampleImageCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class ResampleImageCommand : public CommandObjectBase
  {
    
  public:
		
    typedef ResampleImageCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(ResampleImageCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "resample_image"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    ResampleImageCommand();
    ~ResampleImageCommand();
    
  private:
    ResampleImageCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
