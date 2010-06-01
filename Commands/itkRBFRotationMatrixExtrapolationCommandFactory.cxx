#include "itkRBFRotationMatrixExtrapolationCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkRBFRotationMatrixExtrapolationCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  RBFRotationMatrixExtrapolationCommandFactory::RBFRotationMatrixExtrapolationCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkRBFRotationMatrixExtrapolationCommand",
			   "Extrapolate a set of rotation matrices using radial basis functions (RBF)",
			   1,
			   CreateObjectFunction<RBFRotationMatrixExtrapolationCommand>::New());
  }
  
  RBFRotationMatrixExtrapolationCommandFactory::~RBFRotationMatrixExtrapolationCommandFactory()
  {
  }
  
  const char* 
  RBFRotationMatrixExtrapolationCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  RBFRotationMatrixExtrapolationCommandFactory::GetDescription(void) const
  {
    return "Extrapolate a set of rotation matrices using radial basis functions (RBF)";
  }
  
} // end namespace itk
