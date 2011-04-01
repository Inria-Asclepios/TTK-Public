#ifndef _itk_ExtractBundleWithROICommand_h_
#define _itk_ExtractBundleWithROICommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class ExtractBundleWithROICommand : public CommandObjectBase
  {
    
  public:
		
    typedef ExtractBundleWithROICommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(ExtractBundleWithROICommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "extract_bundle"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    ExtractBundleWithROICommand();
    ~ExtractBundleWithROICommand();
    
  private:
    ExtractBundleWithROICommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
