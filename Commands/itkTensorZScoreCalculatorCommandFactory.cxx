#include "itkTensorZScoreCalculatorCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkTensorZScoreCalculatorCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  TensorZScoreCalculatorCommandFactory::TensorZScoreCalculatorCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkTensorZScoreCalculatorCommand",
			   "Calculate a tensor z-score given an average tensor field and a covariance matrix field",
			   1,
			   CreateObjectFunction<TensorZScoreCalculatorCommand>::New());
  }
  
  TensorZScoreCalculatorCommandFactory::~TensorZScoreCalculatorCommandFactory()
  {
  }
  
  const char* 
  TensorZScoreCalculatorCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  TensorZScoreCalculatorCommandFactory::GetDescription(void) const
  {
    return "Calculate a tensor z-score given an average tensor field and a covariance matrix field";
  }
  
} // end namespace itk
