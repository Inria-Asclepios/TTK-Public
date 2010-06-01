#ifndef _itk_TensorsToVTKUnstructuredGridCommandFactory_h_
#define _itk_TensorsToVTKUnstructuredGridCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class TensorsToVTKUnstructuredGridCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef TensorsToVTKUnstructuredGridCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static TensorsToVTKUnstructuredGridCommandFactory* FactoryNew() { return new TensorsToVTKUnstructuredGridCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(TensorsToVTKUnstructuredGridCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      TensorsToVTKUnstructuredGridCommandFactory::Pointer CSFFactory = TensorsToVTKUnstructuredGridCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    TensorsToVTKUnstructuredGridCommandFactory();
    ~TensorsToVTKUnstructuredGridCommandFactory();
    
  private:
    TensorsToVTKUnstructuredGridCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
