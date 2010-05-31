#include "itkTensorToScalarCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkTensorToScalarCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  TensorToScalarCommandFactory::TensorToScalarCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkTensorToScalarCommand",
			   "Extract scalars (such as FA) from tensors",
			   1,
			   CreateObjectFunction<TensorToScalarCommand>::New());
  }
  
  TensorToScalarCommandFactory::~TensorToScalarCommandFactory()
  {
  }
  
  const char* 
  TensorToScalarCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  TensorToScalarCommandFactory::GetDescription(void) const
  {
    return "Extract scalars (such as FA) from tensors";
  }
  
} // end namespace itk
