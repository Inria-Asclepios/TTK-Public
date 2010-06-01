#ifndef _itk_WarpImageCommandFactory_h_
#define _itk_WarpImageCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class WarpImageCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef WarpImageCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static WarpImageCommandFactory* FactoryNew() { return new WarpImageCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(WarpImageCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      WarpImageCommandFactory::Pointer CSFFactory = WarpImageCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    WarpImageCommandFactory();
    ~WarpImageCommandFactory();
    
  private:
    WarpImageCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
