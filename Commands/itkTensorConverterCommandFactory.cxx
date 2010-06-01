#include "itkTensorConverterCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkTensorConverterCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  TensorConverterCommandFactory::TensorConverterCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkTensorConverterCommand",
			   "Convert a tensor image into another format",
			   1,
			   CreateObjectFunction<TensorConverterCommand>::New());
  }
  
  TensorConverterCommandFactory::~TensorConverterCommandFactory()
  {
  }
  
  const char* 
  TensorConverterCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  TensorConverterCommandFactory::GetDescription(void) const
  {
    return "Convert a tensor image into another format";
  }
  
} // end namespace itk
