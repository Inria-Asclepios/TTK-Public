#ifndef _itk_TensorsToDWICommandFactory_h_
#define _itk_TensorsToDWICommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class TensorsToDWICommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef TensorsToDWICommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static TensorsToDWICommandFactory* FactoryNew() { return new TensorsToDWICommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(TensorsToDWICommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      TensorsToDWICommandFactory::Pointer CSFFactory = TensorsToDWICommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    TensorsToDWICommandFactory();
    ~TensorsToDWICommandFactory();
    
  private:
    TensorsToDWICommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
