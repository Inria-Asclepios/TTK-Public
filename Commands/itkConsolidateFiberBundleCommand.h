#ifndef _itk_ConsolidateFiberBundleCommand_h_
#define _itk_ConsolidateFiberBundleCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class ConsolidateFiberBundleCommand : public CommandObjectBase
  {
    
  public:
		
    typedef ConsolidateFiberBundleCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(ConsolidateFiberBundleCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "consolidate"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    ConsolidateFiberBundleCommand();
    ~ConsolidateFiberBundleCommand();
    
  private:
    ConsolidateFiberBundleCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
