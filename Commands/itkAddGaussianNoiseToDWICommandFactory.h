#pragma once

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class AddGaussianNoiseToDWICommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef AddGaussianNoiseToDWICommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static AddGaussianNoiseToDWICommandFactory* FactoryNew() { return new AddGaussianNoiseToDWICommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(AddGaussianNoiseToDWICommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      AddGaussianNoiseToDWICommandFactory::Pointer CSFFactory = AddGaussianNoiseToDWICommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    AddGaussianNoiseToDWICommandFactory();
    ~AddGaussianNoiseToDWICommandFactory();
    
  private:
    AddGaussianNoiseToDWICommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}
