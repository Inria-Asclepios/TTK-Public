#include "itkDTIMultiEstimatorCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkDTIMultiEstimatorCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  DTIMultiEstimatorCommandFactory::DTIMultiEstimatorCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkDTIMultiEstimatorCommand",
			   "Estimate tensors using a linear least-square approach; all possible gradient combinations are reconstructed",
			   1,
			   CreateObjectFunction<DTIMultiEstimatorCommand>::New());
  }
  
  DTIMultiEstimatorCommandFactory::~DTIMultiEstimatorCommandFactory()
  {
  }
  
  const char* 
  DTIMultiEstimatorCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  DTIMultiEstimatorCommandFactory::GetDescription(void) const
  {
    return "Estimate tensors using a linear least-square approach; all possible gradient combinations are reconstructed";
  }
  
} // end namespace itk
