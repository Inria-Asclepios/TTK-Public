#include "itkWarpTensorImageCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkWarpTensorImageCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  WarpTensorImageCommandFactory::WarpTensorImageCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkWarpTensorImageCommand",
			   "warp a tensor image",
			   1,
			   CreateObjectFunction<WarpTensorImageCommand>::New());
  }
  
  WarpTensorImageCommandFactory::~WarpTensorImageCommandFactory()
  {
  }
  
  const char* 
  WarpTensorImageCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  WarpTensorImageCommandFactory::GetDescription(void) const
  {
    return "Warp a tensor image given a deformation field";
  }
  
} // end namespace itk
