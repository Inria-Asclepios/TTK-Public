#ifndef _itk_SparseTensorsExtrapolationCommand_h_
#define _itk_SparseTensorsExtrapolationCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class SparseTensorsExtrapolationCommand : public CommandObjectBase
  {
    
  public:
		
    typedef SparseTensorsExtrapolationCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(SparseTensorsExtrapolationCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "sparse_extrapolation"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    SparseTensorsExtrapolationCommand();
    ~SparseTensorsExtrapolationCommand();
    
  private:
    SparseTensorsExtrapolationCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
