#include "itkConsolidateFiberBundleCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkConsolidateFiberBundleCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  ConsolidateFiberBundleCommandFactory::ConsolidateFiberBundleCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkConsolidateFiberBundleCommand",
			   "Consolidate a fiber bundle by retrieving point data from all fibers",
			   1,
			   CreateObjectFunction<ConsolidateFiberBundleCommand>::New());
  }
  
  ConsolidateFiberBundleCommandFactory::~ConsolidateFiberBundleCommandFactory()
  {
  }
  
  const char* 
  ConsolidateFiberBundleCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  ConsolidateFiberBundleCommandFactory::GetDescription(void) const
  {
    return "Consolidate a fiber bundle by retrieving point data from all fibers";
  }
  
} // end namespace itk
