#ifndef _itk_VTKFibersToITKGroupSpatialObjectCommand_h_
#define _itk_VTKFibersToITKGroupSpatialObjectCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class VTKFibersToITKGroupSpatialObjectCommand : public CommandObjectBase
  {
    
  public:
		
    typedef VTKFibersToITKGroupSpatialObjectCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(VTKFibersToITKGroupSpatialObjectCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "fiber_vtk_to_itk"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    VTKFibersToITKGroupSpatialObjectCommand();
    ~VTKFibersToITKGroupSpatialObjectCommand();
    
  private:
    VTKFibersToITKGroupSpatialObjectCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
