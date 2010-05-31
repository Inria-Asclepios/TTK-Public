#ifndef _itk_TensorFlipCommand_h_
#define _itk_TensorFlipCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class TensorFlipCommand : public CommandObjectBase
  {
    
  public:
		
    typedef TensorFlipCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(TensorFlipCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "flip"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    TensorFlipCommand();
    ~TensorFlipCommand();
    
  private:
    TensorFlipCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
