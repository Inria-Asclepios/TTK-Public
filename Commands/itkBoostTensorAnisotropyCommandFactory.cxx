#include "itkBoostTensorAnisotropyCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkBoostTensorAnisotropyCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  BoostTensorAnisotropyCommandFactory::BoostTensorAnisotropyCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkBoostTensorAnisotropyCommand",
			   "Boost the tensor anisotropy (FA)",
			   1,
			   CreateObjectFunction<BoostTensorAnisotropyCommand>::New());
  }
  
  BoostTensorAnisotropyCommandFactory::~BoostTensorAnisotropyCommandFactory()
  {
  }
  
  const char* 
  BoostTensorAnisotropyCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  BoostTensorAnisotropyCommandFactory::GetDescription(void) const
  {
    return "Boost the tensor anisotropy (FA)";
  }
  
} // end namespace itk
