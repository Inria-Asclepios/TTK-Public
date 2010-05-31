#include "itkWarpFibersCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkWarpFibersCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  WarpFibersCommandFactory::WarpFibersCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkWarpFibersCommand",
			   "Warp fibers w.r.t. a linear matrix or a deformation field",
			   1,
			   CreateObjectFunction<WarpFibersCommand>::New());
  }
  
  WarpFibersCommandFactory::~WarpFibersCommandFactory()
  {
  }
  
  const char* 
  WarpFibersCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  WarpFibersCommandFactory::GetDescription(void) const
  {
    return "Warp fibers w.r.t. a linear matrix or a deformation field";
  }
  
} // end namespace itk
