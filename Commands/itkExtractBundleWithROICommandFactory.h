#ifndef _itk_ExtractBundleWithROICommandFactory_h_
#define _itk_ExtractBundleWithROICommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class ExtractBundleWithROICommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef ExtractBundleWithROICommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static ExtractBundleWithROICommandFactory* FactoryNew() { return new ExtractBundleWithROICommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(ExtractBundleWithROICommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      ExtractBundleWithROICommandFactory::Pointer CSFFactory = ExtractBundleWithROICommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    ExtractBundleWithROICommandFactory();
    ~ExtractBundleWithROICommandFactory();
    
  private:
    ExtractBundleWithROICommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
