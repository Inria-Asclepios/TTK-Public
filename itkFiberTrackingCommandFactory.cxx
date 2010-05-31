#include "itkFiberTrackingCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkFiberTrackingCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  FiberTrackingCommandFactory::FiberTrackingCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkFiberTrackingCommand",
			   "Perform streamline fiber tracking using a Log-Euclidean advection-diffusion scheme",
			   1,
			   CreateObjectFunction<FiberTrackingCommand>::New());
  }
  
  FiberTrackingCommandFactory::~FiberTrackingCommandFactory()
  {
  }
  
  const char* 
  FiberTrackingCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  FiberTrackingCommandFactory::GetDescription(void) const
  {
    return "Perform streamline fiber tracking using a Log-Euclidean advection-diffusion scheme";
  }
  
} // end namespace itk
