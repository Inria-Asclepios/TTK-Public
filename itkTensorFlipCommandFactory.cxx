#include "itkTensorFlipCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkTensorFlipCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  TensorFlipCommandFactory::TensorFlipCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkTensorFlipCommand",
			   "flip tensors w.r.t. a given axis",
			   1,
			   CreateObjectFunction<TensorFlipCommand>::New());
  }
  
  TensorFlipCommandFactory::~TensorFlipCommandFactory()
  {
  }
  
  const char* 
  TensorFlipCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  TensorFlipCommandFactory::GetDescription(void) const
  {
    return "Flip tensors w.r.t. a given axis";
  }
  
} // end namespace itk
