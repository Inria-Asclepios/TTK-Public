#ifndef _itk_TensorAnisotropicFilteringCommand_h_
#define _itk_TensorAnisotropicFilteringCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class TensorAnisotropicFilteringCommand : public CommandObjectBase
  {
    
  public:
		
    typedef TensorAnisotropicFilteringCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(TensorAnisotropicFilteringCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "anisotropic_filter"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    TensorAnisotropicFilteringCommand();
    ~TensorAnisotropicFilteringCommand();
    
  private:
    TensorAnisotropicFilteringCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
