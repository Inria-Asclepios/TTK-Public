#ifndef _itk_DTIEstimatorCommand_h_
#define _itk_DTIEstimatorCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class DTIEstimatorCommand : public CommandObjectBase
  {
    
  public:
		
    typedef DTIEstimatorCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(DTIEstimatorCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "estimate"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    DTIEstimatorCommand();
    ~DTIEstimatorCommand();
    
  private:
    DTIEstimatorCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
