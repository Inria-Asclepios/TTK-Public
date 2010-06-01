#include "itkSyntheticTensorFieldGeneratorCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkSyntheticTensorFieldGeneratorCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  SyntheticTensorFieldGeneratorCommandFactory::SyntheticTensorFieldGeneratorCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkSyntheticTensorFieldGeneratorCommand",
			   "Generate a synthetic crossing or circle tensor field",
			   1,
			   CreateObjectFunction<SyntheticTensorFieldGeneratorCommand>::New());
  }
  
  SyntheticTensorFieldGeneratorCommandFactory::~SyntheticTensorFieldGeneratorCommandFactory()
  {
  }
  
  const char* 
  SyntheticTensorFieldGeneratorCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  SyntheticTensorFieldGeneratorCommandFactory::GetDescription(void) const
  {
    return "Generate a synthetic crossing or circle tensor field";
  }
  
} // end namespace itk
