#pragma once

#include "itkCommandObjectBase.h"

namespace itk {

  class TensorZScoreCalculatorCommand : public CommandObjectBase
  {
    
  public:
		
    typedef TensorZScoreCalculatorCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(TensorZScoreCalculatorCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "zscore"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    TensorZScoreCalculatorCommand();
    ~TensorZScoreCalculatorCommand();
    
  private:
    TensorZScoreCalculatorCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}
