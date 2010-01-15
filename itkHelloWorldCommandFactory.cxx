#include "itkHelloWorldCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkHelloWorldCommand.h"
#include "itkVersion.h"

namespace itk
{
	
	HelloWorldCommandFactory::HelloWorldCommandFactory()
	{
		this->RegisterOverride("itkCommandObjectBase",
							   "itkHelloWorldCommand",
							   "Hello World Command",
							   1,
							   CreateObjectFunction<HelloWorldCommand>::New());
	}
	
	HelloWorldCommandFactory::~HelloWorldCommandFactory()
	{
	}
	
	const char* 
	HelloWorldCommandFactory::GetITKSourceVersion(void) const
	{
		return ITK_SOURCE_VERSION;
	}
	
	const char* 
	HelloWorldCommandFactory::GetDescription(void) const
	{
		return "Pouet pouet pouet";
	}
	
} // end namespace itk