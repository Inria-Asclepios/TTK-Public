#include "itkDTIEstimatorWithBFGSCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkDTIEstimatorWithBFGSCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  DTIEstimatorWithBFGSCommandFactory::DTIEstimatorWithBFGSCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkDTIEstimatorWithBFGSCommand",
			   "Estimate tensors using a Rician noise model and a BFGS optimizer",
			   1,
			   CreateObjectFunction<DTIEstimatorWithBFGSCommand>::New());
  }
  
  DTIEstimatorWithBFGSCommandFactory::~DTIEstimatorWithBFGSCommandFactory()
  {
  }
  
  const char* 
  DTIEstimatorWithBFGSCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  DTIEstimatorWithBFGSCommandFactory::GetDescription(void) const
  {
    return "Estimate tensors using a Rician noise model and a BFGS optimizer";
  }
  
} // end namespace itk
