#ifndef _itk_DWIBrainMaskExtractorCommand_h_
#define _itk_DWIBrainMaskExtractorCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class DWIBrainMaskExtractorCommand : public CommandObjectBase
  {
    
  public:
		
    typedef DWIBrainMaskExtractorCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(DWIBrainMaskExtractorCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "dwi_mask_extract"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    DWIBrainMaskExtractorCommand();
    ~DWIBrainMaskExtractorCommand();
    
  private:
    DWIBrainMaskExtractorCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
