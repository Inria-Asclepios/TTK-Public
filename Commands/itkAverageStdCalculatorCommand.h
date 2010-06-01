#ifndef _itk_AverageStdCalculatorCommand_h_
#define _itk_AverageStdCalculatorCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class AverageStdCalculatorCommand : public CommandObjectBase
  {
    
  public:
		
    typedef AverageStdCalculatorCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(AverageStdCalculatorCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "average_std_image"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    AverageStdCalculatorCommand();
    ~AverageStdCalculatorCommand();
    
  private:
    AverageStdCalculatorCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
