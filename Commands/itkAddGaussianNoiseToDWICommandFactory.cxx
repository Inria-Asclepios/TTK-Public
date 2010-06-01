#include "itkAddGaussianNoiseToDWICommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkAddGaussianNoiseToDWICommand.h"
#include "itkVersion.h"

namespace itk
{
  
  AddGaussianNoiseToDWICommandFactory::AddGaussianNoiseToDWICommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkAddGaussianNoiseToDWICommand",
			   "Generate DWI, add Gaussian noise on them, and re-estimate tensors",
			   1,
			   CreateObjectFunction<AddGaussianNoiseToDWICommand>::New());
  }
  
  AddGaussianNoiseToDWICommandFactory::~AddGaussianNoiseToDWICommandFactory()
  {
  }
  
  const char* 
  AddGaussianNoiseToDWICommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  AddGaussianNoiseToDWICommandFactory::GetDescription(void) const
  {
    return "Generate DWI, add Gaussian noise on them, and re-estimate tensors";
  }
  
} // end namespace itk
