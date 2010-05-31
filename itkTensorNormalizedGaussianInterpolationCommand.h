#ifndef _itk_TensorNormalizedGaussianInterpolationCommand_h_
#define _itk_TensorNormalizedGaussianInterpolationCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class TensorNormalizedGaussianInterpolationCommand : public CommandObjectBase
  {
    
  public:
		
    typedef TensorNormalizedGaussianInterpolationCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(TensorNormalizedGaussianInterpolationCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "normalized_gaussian_interpolation"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    TensorNormalizedGaussianInterpolationCommand();
    ~TensorNormalizedGaussianInterpolationCommand();
    
  private:
    TensorNormalizedGaussianInterpolationCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
