#ifndef _itk_HelloWorldCommand_h_
#define _itk_HelloWorldCommand_h_


#include "itkCommandObjectBase.h"

/**
 
 */

namespace itk
{
	
	class HelloWorldCommand : public CommandObjectBase
	{
	public:
		typedef HelloWorldCommand        Self;
		typedef CommandObjectBase            Superclass;
		typedef SmartPointer<Self>       Pointer;
		typedef SmartPointer<const Self> ConstPointer;
		
		itkNewMacro (HelloWorldCommand);
		itkTypeMacro (HelloWorldCommand, CommandObjectBase);
		
		virtual const char *GetCommandName (void);
		virtual const char *GetShortDescription (void) const;
		virtual const char *GetLongDescription (void) const;
		
		virtual int Execute (int nargs, const char *args[]);
		
	protected:
		HelloWorldCommand();
		~HelloWorldCommand();
		
		
	private:
		HelloWorldCommand (const Self&);
		void operator=(const Self&);
		
		
	};
	
	
} // end of namespace


#endif
