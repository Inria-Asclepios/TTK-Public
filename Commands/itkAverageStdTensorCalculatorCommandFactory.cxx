#include "itkAverageStdTensorCalculatorCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkAverageStdTensorCalculatorCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  AverageStdTensorCalculatorCommandFactory::AverageStdTensorCalculatorCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkAverageStdTensorCalculatorCommand",
			   "Calculate the average and covariance matrix of a set of tensor fields",
			   1,
			   CreateObjectFunction<AverageStdTensorCalculatorCommand>::New());
  }
  
  AverageStdTensorCalculatorCommandFactory::~AverageStdTensorCalculatorCommandFactory()
  {
  }
  
  const char* 
  AverageStdTensorCalculatorCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  AverageStdTensorCalculatorCommandFactory::GetDescription(void) const
  {
    return "Calculate the average and covariance matrix of a set of tensor fields";
  }
  
} // end namespace itk
