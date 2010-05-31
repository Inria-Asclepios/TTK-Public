#ifndef _itk_TensorToScalarCommand_h_
#define _itk_TensorToScalarCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class TensorToScalarCommand : public CommandObjectBase
  {
    
  public:
		
    typedef TensorToScalarCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(TensorToScalarCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "scalar"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    TensorToScalarCommand();
    ~TensorToScalarCommand();
    
  private:
    TensorToScalarCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
