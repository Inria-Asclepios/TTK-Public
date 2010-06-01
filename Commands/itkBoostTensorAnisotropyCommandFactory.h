#ifndef _itk_BoostTensorAnisotropyCommandFactory_h_
#define _itk_BoostTensorAnisotropyCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class BoostTensorAnisotropyCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef BoostTensorAnisotropyCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static BoostTensorAnisotropyCommandFactory* FactoryNew() { return new BoostTensorAnisotropyCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(BoostTensorAnisotropyCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      BoostTensorAnisotropyCommandFactory::Pointer CSFFactory = BoostTensorAnisotropyCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    BoostTensorAnisotropyCommandFactory();
    ~BoostTensorAnisotropyCommandFactory();
    
  private:
    BoostTensorAnisotropyCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
