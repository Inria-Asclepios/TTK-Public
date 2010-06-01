#ifndef _itk_AddGaussianNoiseToDWICommand_h_
#define _itk_AddGaussianNoiseToDWICommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class AddGaussianNoiseToDWICommand : public CommandObjectBase
  {
    
  public:
		
    typedef AddGaussianNoiseToDWICommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(AddGaussianNoiseToDWICommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "add_noise_dwi"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    AddGaussianNoiseToDWICommand();
    ~AddGaussianNoiseToDWICommand();
    
  private:
    AddGaussianNoiseToDWICommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
