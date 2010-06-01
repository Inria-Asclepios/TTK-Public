#ifndef _itk_TensorConverterCommand_h_
#define _itk_TensorConverterCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class TensorConverterCommand : public CommandObjectBase
  {
    
  public:
		
    typedef TensorConverterCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(TensorConverterCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "tensor"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    TensorConverterCommand();
    ~TensorConverterCommand();
    
  private:
    TensorConverterCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
