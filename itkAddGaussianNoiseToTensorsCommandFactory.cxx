#include "itkAddGaussianNoiseToTensorsCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkAddGaussianNoiseToTensorsCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  AddGaussianNoiseToTensorsCommandFactory::AddGaussianNoiseToTensorsCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkAddGaussianNoiseToTensorsCommand",
			   "Add Gaussian noise on the tensor logarithm and exponentiate the result",
			   1,
			   CreateObjectFunction<AddGaussianNoiseToTensorsCommand>::New());
  }
  
  AddGaussianNoiseToTensorsCommandFactory::~AddGaussianNoiseToTensorsCommandFactory()
  {
  }
  
  const char* 
  AddGaussianNoiseToTensorsCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  AddGaussianNoiseToTensorsCommandFactory::GetDescription(void) const
  {
    return "Add Gaussian noise on the tensor logarithm and exponentiate the result";
  }
  
} // end namespace itk
