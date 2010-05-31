#include "itkTensorsToDWICommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkTensorsToDWICommand.h"
#include "itkVersion.h"

namespace itk
{
  
  TensorsToDWICommandFactory::TensorsToDWICommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkTensorsToDWICommand",
			   "Create DWI from tensors using the Stejskal & Tanner diffusion equation (and a list of gradients)",
			   1,
			   CreateObjectFunction<TensorsToDWICommand>::New());
  }
  
  TensorsToDWICommandFactory::~TensorsToDWICommandFactory()
  {
  }
  
  const char* 
  TensorsToDWICommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  TensorsToDWICommandFactory::GetDescription(void) const
  {
    return "Create DWI from tensors using the Stejskal & Tanner diffusion equation (and a list of gradients)";
  }
  
} // end namespace itk
