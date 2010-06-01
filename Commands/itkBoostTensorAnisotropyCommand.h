#ifndef _itk_BoostTensorAnisotropyCommand_h_
#define _itk_BoostTensorAnisotropyCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class BoostTensorAnisotropyCommand : public CommandObjectBase
  {
    
  public:
		
    typedef BoostTensorAnisotropyCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(BoostTensorAnisotropyCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "boost_ani"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    BoostTensorAnisotropyCommand();
    ~BoostTensorAnisotropyCommand();
    
  private:
    BoostTensorAnisotropyCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
