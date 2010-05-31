#ifndef _itk_TensorsToDWICommand_h_
#define _itk_TensorsToDWICommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class TensorsToDWICommand : public CommandObjectBase
  {
    
  public:
		
    typedef TensorsToDWICommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(TensorsToDWICommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "tensors_to_dwis"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    TensorsToDWICommand();
    ~TensorsToDWICommand();
    
  private:
    TensorsToDWICommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
