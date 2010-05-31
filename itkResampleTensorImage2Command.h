#ifndef _itk_ResampleTensorImage2Command_h_
#define _itk_ResampleTensorImage2Command_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class ResampleTensorImage2Command : public CommandObjectBase
  {
    
  public:
		
    typedef ResampleTensorImage2Command Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(ResampleTensorImage2Command, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "resample2"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    ResampleTensorImage2Command();
    ~ResampleTensorImage2Command();
    
  private:
    ResampleTensorImage2Command(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
