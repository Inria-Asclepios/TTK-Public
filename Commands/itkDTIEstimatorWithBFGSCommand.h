#pragma once

#include "itkCommandObjectBase.h"

namespace itk {

  class DTIEstimatorWithBFGSCommand : public CommandObjectBase
  {
    
  public:
		
    typedef DTIEstimatorWithBFGSCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(DTIEstimatorWithBFGSCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "estimate_bfgs"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    DTIEstimatorWithBFGSCommand();
    ~DTIEstimatorWithBFGSCommand();
    
  private:
    DTIEstimatorWithBFGSCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}
