#ifndef _itk_HessianTensorImageCalculatorCommand_h_
#define _itk_HessianTensorImageCalculatorCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class HessianTensorImageCalculatorCommand : public CommandObjectBase
  {
    
  public:
		
    typedef HessianTensorImageCalculatorCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(HessianTensorImageCalculatorCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "hessian"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    HessianTensorImageCalculatorCommand();
    ~HessianTensorImageCalculatorCommand();
    
  private:
    HessianTensorImageCalculatorCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
