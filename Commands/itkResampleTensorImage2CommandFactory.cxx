#include "itkResampleTensorImage2CommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkResampleTensorImage2Command.h"
#include "itkVersion.h"

namespace itk
{
  
  ResampleTensorImage2CommandFactory::ResampleTensorImage2CommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkResampleTensorImage2Command",
			   "Resample tensors w.r.t. a linear matrix and a domain extent",
			   1,
			   CreateObjectFunction<ResampleTensorImage2Command>::New());
  }
  
  ResampleTensorImage2CommandFactory::~ResampleTensorImage2CommandFactory()
  {
  }
  
  const char* 
  ResampleTensorImage2CommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  ResampleTensorImage2CommandFactory::GetDescription(void) const
  {
    return "Resample tensors w.r.t. a linear matrix and a domain extent";
  }
  
} // end namespace itk
