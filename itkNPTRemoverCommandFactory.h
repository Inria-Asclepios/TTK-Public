#ifndef _itk_NPTRemoverCommandFactory_h_
#define _itk_NPTRemoverCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class NPTRemoverCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef NPTRemoverCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static NPTRemoverCommandFactory* FactoryNew() { return new NPTRemoverCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(NPTRemoverCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      NPTRemoverCommandFactory::Pointer CSFFactory = NPTRemoverCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    NPTRemoverCommandFactory();
    ~NPTRemoverCommandFactory();
    
  private:
    NPTRemoverCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
