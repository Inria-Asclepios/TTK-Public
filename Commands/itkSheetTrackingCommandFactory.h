#pragma once

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class SheetTrackingCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef SheetTrackingCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static SheetTrackingCommandFactory* FactoryNew() { return new SheetTrackingCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(SheetTrackingCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      SheetTrackingCommandFactory::Pointer CSFFactory = SheetTrackingCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    SheetTrackingCommandFactory();
    ~SheetTrackingCommandFactory();
    
  private:
    SheetTrackingCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}
