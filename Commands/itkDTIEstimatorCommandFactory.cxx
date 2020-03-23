#include "itkDTIEstimatorCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkDTIEstimatorCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  DTIEstimatorCommandFactory::DTIEstimatorCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkDTIEstimatorCommand",
			   "Estimate tensors using a linear least-square approach",
			   1,
			   CreateObjectFunction<DTIEstimatorCommand>::New());
  }
  
  DTIEstimatorCommandFactory::~DTIEstimatorCommandFactory()
  {
  }
  
  const char* 
  DTIEstimatorCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  DTIEstimatorCommandFactory::GetDescription(void) const
  {
    return "Estimate tensors using a linear least-square approach";
  }
  
} // end namespace itk
