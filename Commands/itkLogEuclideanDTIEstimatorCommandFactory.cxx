#include "itkLogEuclideanDTIEstimatorCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkLogEuclideanDTIEstimatorCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  LogEuclideanDTIEstimatorCommandFactory::LogEuclideanDTIEstimatorCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkLogEuclideanDTIEstimatorCommand",
			   "Estimate tensors using a Log-Gaussian / Gaussian / Rician noise model and Log-Euclidean metrics",
			   1,
			   CreateObjectFunction<LogEuclideanDTIEstimatorCommand>::New());
  }
  
  LogEuclideanDTIEstimatorCommandFactory::~LogEuclideanDTIEstimatorCommandFactory()
  {
  }
  
  const char* 
  LogEuclideanDTIEstimatorCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  LogEuclideanDTIEstimatorCommandFactory::GetDescription(void) const
  {
    return "Estimate tensors using a Log-Gaussian / Gaussian / Rician noise model and Log-Euclidean metrics";
  }
  
} // end namespace itk
