#include "itkNormalizeTensorsCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkNormalizeTensorsCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  NormalizeTensorsCommandFactory::NormalizeTensorsCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkNormalizeTensorsCommand",
			   "Divide tensors by the largest eigenvalue of the field",
			   1,
			   CreateObjectFunction<NormalizeTensorsCommand>::New());
  }
  
  NormalizeTensorsCommandFactory::~NormalizeTensorsCommandFactory()
  {
  }
  
  const char* 
  NormalizeTensorsCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  NormalizeTensorsCommandFactory::GetDescription(void) const
  {
    return "Divide tensors by the largest eigenvalue of the field";
  }
  
} // end namespace itk
