#include "itkTensorNormalizedGaussianInterpolationCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkTensorNormalizedGaussianInterpolationCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  TensorNormalizedGaussianInterpolationCommandFactory::TensorNormalizedGaussianInterpolationCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkTensorNormalizedGaussianInterpolationCommand",
			   "Extrapolate sparse tensors on a regular grid using a normalized Gaussian",
			   1,
			   CreateObjectFunction<TensorNormalizedGaussianInterpolationCommand>::New());
  }
  
  TensorNormalizedGaussianInterpolationCommandFactory::~TensorNormalizedGaussianInterpolationCommandFactory()
  {
  }
  
  const char* 
  TensorNormalizedGaussianInterpolationCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  TensorNormalizedGaussianInterpolationCommandFactory::GetDescription(void) const
  {
    return "Extrapolate sparse tensors on a regular grid using a normalized Gaussian";
  }
  
} // end namespace itk
