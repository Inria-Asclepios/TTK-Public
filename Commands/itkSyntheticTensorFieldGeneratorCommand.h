#ifndef _itk_SyntheticTensorFieldGeneratorCommand_h_
#define _itk_SyntheticTensorFieldGeneratorCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class SyntheticTensorFieldGeneratorCommand : public CommandObjectBase
  {
    
  public:
		
    typedef SyntheticTensorFieldGeneratorCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(SyntheticTensorFieldGeneratorCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "generate"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    SyntheticTensorFieldGeneratorCommand();
    ~SyntheticTensorFieldGeneratorCommand();
    
  private:
    SyntheticTensorFieldGeneratorCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
