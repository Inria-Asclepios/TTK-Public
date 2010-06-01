#ifndef _itk_SyntheticTensorFieldGeneratorCommandFactory_h_
#define _itk_SyntheticTensorFieldGeneratorCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class SyntheticTensorFieldGeneratorCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef SyntheticTensorFieldGeneratorCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static SyntheticTensorFieldGeneratorCommandFactory* FactoryNew() { return new SyntheticTensorFieldGeneratorCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(SyntheticTensorFieldGeneratorCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      SyntheticTensorFieldGeneratorCommandFactory::Pointer CSFFactory = SyntheticTensorFieldGeneratorCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    SyntheticTensorFieldGeneratorCommandFactory();
    ~SyntheticTensorFieldGeneratorCommandFactory();
    
  private:
    SyntheticTensorFieldGeneratorCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
