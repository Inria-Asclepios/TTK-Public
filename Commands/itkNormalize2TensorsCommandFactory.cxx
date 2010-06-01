#include "itkNormalize2TensorsCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkNormalize2TensorsCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  Normalize2TensorsCommandFactory::Normalize2TensorsCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkNormalize2TensorsCommand",
			   "Divide each tensor by its largest eigenvalue",
			   1,
			   CreateObjectFunction<Normalize2TensorsCommand>::New());
  }
  
  Normalize2TensorsCommandFactory::~Normalize2TensorsCommandFactory()
  {
  }
  
  const char* 
  Normalize2TensorsCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  Normalize2TensorsCommandFactory::GetDescription(void) const
  {
    return "Divide each tensor by its largest eigenvalue";
  }
  
} // end namespace itk
