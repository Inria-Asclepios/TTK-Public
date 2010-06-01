#ifndef _itk_LogEuclideanDTIEstimatorCommand_h_
#define _itk_LogEuclideanDTIEstimatorCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class LogEuclideanDTIEstimatorCommand : public CommandObjectBase
  {
    
  public:
		
    typedef LogEuclideanDTIEstimatorCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(LogEuclideanDTIEstimatorCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "estimate_le"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    LogEuclideanDTIEstimatorCommand();
    ~LogEuclideanDTIEstimatorCommand();
    
  private:
    LogEuclideanDTIEstimatorCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
