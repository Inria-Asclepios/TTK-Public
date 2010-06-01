#ifndef _itk_Normalize2TensorsCommand_h_
#define _itk_Normalize2TensorsCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class Normalize2TensorsCommand : public CommandObjectBase
  {
    
  public:
		
    typedef Normalize2TensorsCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(Normalize2TensorsCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "normalize2"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    Normalize2TensorsCommand();
    ~Normalize2TensorsCommand();
    
  private:
    Normalize2TensorsCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
