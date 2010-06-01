#include "itkApplyMaskToImageCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkApplyMaskToImageCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  ApplyMaskToImageCommandFactory::ApplyMaskToImageCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkApplyMaskToImageCommand",
			   "Apply a mask to a (tensor) image",
			   1,
			   CreateObjectFunction<ApplyMaskToImageCommand>::New());
  }
  
  ApplyMaskToImageCommandFactory::~ApplyMaskToImageCommandFactory()
  {
  }
  
  const char* 
  ApplyMaskToImageCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  ApplyMaskToImageCommandFactory::GetDescription(void) const
  {
    return "Apply a mask to a (tensor) image";
  }
  
} // end namespace itk
