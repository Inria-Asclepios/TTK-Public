#ifndef _itk_TensorGradientMagnitudeCalculatorCommand_h_
#define _itk_TensorGradientMagnitudeCalculatorCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class TensorGradientMagnitudeCalculatorCommand : public CommandObjectBase
  {
    
  public:
		
    typedef TensorGradientMagnitudeCalculatorCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(TensorGradientMagnitudeCalculatorCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "gradient"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    TensorGradientMagnitudeCalculatorCommand();
    ~TensorGradientMagnitudeCalculatorCommand();
    
  private:
    TensorGradientMagnitudeCalculatorCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
