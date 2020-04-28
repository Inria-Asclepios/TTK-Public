#pragma once

#include "itkCommandObjectBase.h"
#include "itkCommand.h"


#include <cstdio>
#include <cmath>

namespace itk {

  

  /**
     \class CommandIterationUpdate
     \brief Just a little class to handle iteration event observations
     
     \ingroup Command
     \author Nicolas Toussaint, INRIA
     
  */
  class CommandIterationUpdate : public itk::Command
  {
  public:
    typedef CommandIterationUpdate  Self;
    typedef itk::Command            Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::ProcessObject      ProcessObjectType;
    typedef ProcessObjectType::Pointer  ProcessObjectPointer;
    
    itkNewMacro( Self );
    
  protected:
    CommandIterationUpdate() {};
    ~CommandIterationUpdate() {};
    
    ProcessObjectPointer m_ProcessObject;
    unsigned int m_Done;
  public:
    
    void Execute(itk::Object *caller, const itk::EventObject & event)
    {
      Execute( (const itk::Object *)caller, event);
    }
    
    void Execute(const itk::Object * object, const itk::EventObject & event)
    {
      if( !(itk::ProgressEvent().CheckEvent( &event )) )
	return;
      
      const ProcessObjectType * filter = dynamic_cast< const ProcessObjectType * >( object );
      if (!filter)
	return;

      if (filter->GetProgress()==0.0)
      {
	std::cout << "|" << std::flush;
	m_Done = 0;
	return;
      }
      else if (filter->GetProgress() == 1.0)
      {
	std::cout << "|" << std::flush;
	m_Done = 100;
	return;
      }
      else
      {
	
	double percent = 100.0 * filter->GetProgress();
	
	if (std::floor (percent) >= m_Done)
	{
	  m_Done++;
	  std::cout<<"="<<std::flush;
	}
      }
    }
  };

  
  class FiberTrackingCommand : public CommandObjectBase
  {
    
  public:
		
    typedef FiberTrackingCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;

    typedef itk::Command CommandType;
    
    
    itkTypeMacro(FiberTrackingCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "tractography"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    FiberTrackingCommand();
    ~FiberTrackingCommand();
    
  private:
    FiberTrackingCommand(const Self&);
    void operator=(const Self&);

    CommandType::Pointer Command;
    
  };
  
}
