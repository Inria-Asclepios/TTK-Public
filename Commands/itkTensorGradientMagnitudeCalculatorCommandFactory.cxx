#include "itkTensorGradientMagnitudeCalculatorCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkTensorGradientMagnitudeCalculatorCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  TensorGradientMagnitudeCalculatorCommandFactory::TensorGradientMagnitudeCalculatorCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkTensorGradientMagnitudeCalculatorCommand",
			   "Calculate the gradient magnitude of a tensor image",
			   1,
			   CreateObjectFunction<TensorGradientMagnitudeCalculatorCommand>::New());
  }
  
  TensorGradientMagnitudeCalculatorCommandFactory::~TensorGradientMagnitudeCalculatorCommandFactory()
  {
  }
  
  const char* 
  TensorGradientMagnitudeCalculatorCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  TensorGradientMagnitudeCalculatorCommandFactory::GetDescription(void) const
  {
    return "Calculate the gradient magnitude of a tensor image";
  }
  
} // end namespace itk
