#include "itkSheetTrackingCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkSheetTrackingCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  SheetTrackingCommandFactory::SheetTrackingCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkSheetTrackingCommand",
			   "Perform streamline sheet tracking using a Log-Euclidean advection-diffusion scheme",
			   1,
			   CreateObjectFunction<SheetTrackingCommand>::New());
  }
  
  SheetTrackingCommandFactory::~SheetTrackingCommandFactory()
  {
  }
  
  const char* 
  SheetTrackingCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  SheetTrackingCommandFactory::GetDescription(void) const
  {
    return "Perform streamline sheet tracking using a Log-Euclidean advection-diffusion scheme";
  }
  
} // end namespace itk
