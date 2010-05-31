#ifndef _itk_WarpFibersCommandFactory_h_
#define _itk_WarpFibersCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class WarpFibersCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef WarpFibersCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static WarpFibersCommandFactory* FactoryNew() { return new WarpFibersCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(WarpFibersCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      WarpFibersCommandFactory::Pointer CSFFactory = WarpFibersCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    WarpFibersCommandFactory();
    ~WarpFibersCommandFactory();
    
  private:
    WarpFibersCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
