#include "itkResampleImageCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkResampleImageCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  ResampleImageCommandFactory::ResampleImageCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkResampleImageCommand",
			   "Resample an image given a linear transform and a reference image",
			   1,
			   CreateObjectFunction<ResampleImageCommand>::New());
  }
  
  ResampleImageCommandFactory::~ResampleImageCommandFactory()
  {
  }
  
  const char* 
  ResampleImageCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  ResampleImageCommandFactory::GetDescription(void) const
  {
    return "Resample an image given a linear transform and a reference image";
  }
  
} // end namespace itk
