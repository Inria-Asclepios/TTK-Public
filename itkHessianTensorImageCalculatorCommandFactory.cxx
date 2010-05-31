#include "itkHessianTensorImageCalculatorCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkHessianTensorImageCalculatorCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  HessianTensorImageCalculatorCommandFactory::HessianTensorImageCalculatorCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkHessianTensorImageCalculatorCommand",
			   "Extract the largest eigenvalue of the Hessian of a tensor field",
			   1,
			   CreateObjectFunction<HessianTensorImageCalculatorCommand>::New());
  }
  
  HessianTensorImageCalculatorCommandFactory::~HessianTensorImageCalculatorCommandFactory()
  {
  }
  
  const char* 
  HessianTensorImageCalculatorCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  HessianTensorImageCalculatorCommandFactory::GetDescription(void) const
  {
    return "Extract the largest eigenvalue of the Hessian of a tensor field";
  }
  
} // end namespace itk
