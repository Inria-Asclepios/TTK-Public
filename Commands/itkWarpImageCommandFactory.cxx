#include "itkWarpImageCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkWarpImageCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  WarpImageCommandFactory::WarpImageCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkWarpImageCommand",
			   "Warp an image give a deformation field",
			   1,
			   CreateObjectFunction<WarpImageCommand>::New());
  }
  
  WarpImageCommandFactory::~WarpImageCommandFactory()
  {
  }
  
  const char* 
  WarpImageCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  WarpImageCommandFactory::GetDescription(void) const
  {
    return "Warp an image give a deformation field";
  }
  
} // end namespace itk
