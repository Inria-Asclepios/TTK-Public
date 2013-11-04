#include "itkExtractBundleWithROICommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkExtractBundleWithROICommand.h"
#include "itkVersion.h"

namespace itk
{
  
  ExtractBundleWithROICommandFactory::ExtractBundleWithROICommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkExtractBundleWithROICommand",
			   "Extract a bundle given a set of fibers and a ROI",
			   1,
			   CreateObjectFunction<ExtractBundleWithROICommand>::New());
  }
  
  ExtractBundleWithROICommandFactory::~ExtractBundleWithROICommandFactory()
  {
  }
  
  const char* 
  ExtractBundleWithROICommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  ExtractBundleWithROICommandFactory::GetDescription(void) const
  {
    return "Extract a bundle given a set of fibers and a ROI";
  }
  
} // end namespace itk
