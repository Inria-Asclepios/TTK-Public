#include "itkNPTRemoverCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkNPTRemoverCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  NPTRemoverCommandFactory::NPTRemoverCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkNPTRemoverCommand",
			   "remove any Non-Postive Tensors (NPT)",
			   1,
			   CreateObjectFunction<NPTRemoverCommand>::New());
  }
  
  NPTRemoverCommandFactory::~NPTRemoverCommandFactory()
  {
  }
  
  const char* 
  NPTRemoverCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  NPTRemoverCommandFactory::GetDescription(void) const
  {
    return "Removes any non-postive tensors";
  }
  
} // end namespace itk
