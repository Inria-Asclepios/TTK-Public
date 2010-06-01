#ifndef _itk_TensorsToVTKUnstructuredGridCommand_h_
#define _itk_TensorsToVTKUnstructuredGridCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class TensorsToVTKUnstructuredGridCommand : public CommandObjectBase
  {
    
  public:
		
    typedef TensorsToVTKUnstructuredGridCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(TensorsToVTKUnstructuredGridCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "tensor_to_unstructured_grid"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    TensorsToVTKUnstructuredGridCommand();
    ~TensorsToVTKUnstructuredGridCommand();
    
  private:
    TensorsToVTKUnstructuredGridCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
