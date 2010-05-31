#include "itkRBFTensorExtrapolationCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkRBFTensorExtrapolationCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  RBFTensorExtrapolationCommandFactory::RBFTensorExtrapolationCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkRBFTensorExtrapolationCommand",
			   "Extrapolate a set of tensors using radial basis functions (RBF)",
			   1,
			   CreateObjectFunction<RBFTensorExtrapolationCommand>::New());
  }
  
  RBFTensorExtrapolationCommandFactory::~RBFTensorExtrapolationCommandFactory()
  {
  }
  
  const char* 
  RBFTensorExtrapolationCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  RBFTensorExtrapolationCommandFactory::GetDescription(void) const
  {
    return "Extrapolate a set of tensors using radial basis functions (RBF)";
  }
  
} // end namespace itk
