#ifndef _itk_TensorGaussianFilteringCommandFactory_h_
#define _itk_TensorGaussianFilteringCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class TensorGaussianFilteringCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef TensorGaussianFilteringCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static TensorGaussianFilteringCommandFactory* FactoryNew() { return new TensorGaussianFilteringCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(TensorGaussianFilteringCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      TensorGaussianFilteringCommandFactory::Pointer CSFFactory = TensorGaussianFilteringCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    TensorGaussianFilteringCommandFactory();
    ~TensorGaussianFilteringCommandFactory();
    
  private:
    TensorGaussianFilteringCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
