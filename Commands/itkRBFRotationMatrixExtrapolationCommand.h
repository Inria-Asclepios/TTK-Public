#ifndef _itk_RBFRotationMatrixExtrapolationCommand_h_
#define _itk_RBFRotationMatrixExtrapolationCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class RBFRotationMatrixExtrapolationCommand : public CommandObjectBase
  {
    
  public:
		
    typedef RBFRotationMatrixExtrapolationCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(RBFRotationMatrixExtrapolationCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "extrapolate_rotmat_rbf"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    RBFRotationMatrixExtrapolationCommand();
    ~RBFRotationMatrixExtrapolationCommand();
    
  private:
    RBFRotationMatrixExtrapolationCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
