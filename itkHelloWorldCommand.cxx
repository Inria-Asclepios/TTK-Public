#include "itkHelloWorldCommand.h"

namespace itk
{

	HelloWorldCommand::HelloWorldCommand()
	{}
	
	HelloWorldCommand::~HelloWorldCommand()
	{}
	
	const char *HelloWorldCommand::GetCommandName()
	{
		return "HelloWorld";
	}
	
	const char *HelloWorldCommand::GetShortDescription() const
	{
		return "This is a short description";
	}
	
	const char *HelloWorldCommand::GetLongDescription() const
	{
		return "This is a long description";
	}
	
	int HelloWorldCommand::Execute (int nargs, const char *args[])
	{
		std::cout << "Hello World!" << std::endl;
		return EXIT_SUCCESS;
	}
	
}
