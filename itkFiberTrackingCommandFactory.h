#ifndef _itk_FiberTrackingCommandFactory_h_
#define _itk_FiberTrackingCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class FiberTrackingCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef FiberTrackingCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static FiberTrackingCommandFactory* FactoryNew() { return new FiberTrackingCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(FiberTrackingCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      FiberTrackingCommandFactory::Pointer CSFFactory = FiberTrackingCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    FiberTrackingCommandFactory();
    ~FiberTrackingCommandFactory();
    
  private:
    FiberTrackingCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
