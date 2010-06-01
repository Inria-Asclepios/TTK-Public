#include "itkImageConverterCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkImageConverterCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  ImageConverterCommandFactory::ImageConverterCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkImageConverterCommand",
			   "Convert an image file into another format",
			   1,
			   CreateObjectFunction<ImageConverterCommand>::New());
  }
  
  ImageConverterCommandFactory::~ImageConverterCommandFactory()
  {
  }
  
  const char* 
  ImageConverterCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  ImageConverterCommandFactory::GetDescription(void) const
  {
    return "Convert an image file into another format";
  }
  
} // end namespace itk
