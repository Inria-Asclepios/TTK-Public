#include "itkResampleTensorImageCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkResampleTensorImageCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  ResampleTensorImageCommandFactory::ResampleTensorImageCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkResampleTensorImageCommand",
			   "Resample tensors w.r.t. a linear matrix and a reference image",
			   1,
			   CreateObjectFunction<ResampleTensorImageCommand>::New());
  }
  
  ResampleTensorImageCommandFactory::~ResampleTensorImageCommandFactory()
  {
  }
  
  const char* 
  ResampleTensorImageCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  ResampleTensorImageCommandFactory::GetDescription(void) const
  {
    return "Resample tensors w.r.t. a linear matrix and a reference image";
  }
  
} // end namespace itk
