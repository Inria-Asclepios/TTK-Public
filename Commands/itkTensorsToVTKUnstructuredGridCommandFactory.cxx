#include "itkTensorsToVTKUnstructuredGridCommandFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkTensorsToVTKUnstructuredGridCommand.h"
#include "itkVersion.h"

namespace itk
{
  
  TensorsToVTKUnstructuredGridCommandFactory::TensorsToVTKUnstructuredGridCommandFactory()
  {
    this->RegisterOverride("itkCommandObjectBase",
			   "itkTensorsToVTKUnstructuredGridCommand",
			   "Convert tensor images into a vtk unstructured grid",
			   1,
			   CreateObjectFunction<TensorsToVTKUnstructuredGridCommand>::New());
  }
  
  TensorsToVTKUnstructuredGridCommandFactory::~TensorsToVTKUnstructuredGridCommandFactory()
  {
  }
  
  const char* 
  TensorsToVTKUnstructuredGridCommandFactory::GetITKSourceVersion(void) const
  {
    return ITK_SOURCE_VERSION;
  }
  
  const char* 
  TensorsToVTKUnstructuredGridCommandFactory::GetDescription(void) const
  {
    return "Convert tensor images into a vtk unstructured grid";
  }
  
} // end namespace itk
