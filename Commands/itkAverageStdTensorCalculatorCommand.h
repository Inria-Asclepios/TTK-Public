#ifndef _itk_AverageStdTensorCalculatorCommand_h_
#define _itk_AverageStdTensorCalculatorCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class AverageStdTensorCalculatorCommand : public CommandObjectBase
  {
    
  public:
		
    typedef AverageStdTensorCalculatorCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(AverageStdTensorCalculatorCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "average_std"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    AverageStdTensorCalculatorCommand();
    ~AverageStdTensorCalculatorCommand();
    
  private:
    AverageStdTensorCalculatorCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
