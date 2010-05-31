#ifndef _itk_AddGaussianNoiseToTensorsCommandFactory_h_
#define _itk_AddGaussianNoiseToTensorsCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class AddGaussianNoiseToTensorsCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef AddGaussianNoiseToTensorsCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static AddGaussianNoiseToTensorsCommandFactory* FactoryNew() { return new AddGaussianNoiseToTensorsCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(AddGaussianNoiseToTensorsCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      AddGaussianNoiseToTensorsCommandFactory::Pointer CSFFactory = AddGaussianNoiseToTensorsCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    AddGaussianNoiseToTensorsCommandFactory();
    ~AddGaussianNoiseToTensorsCommandFactory();
    
  private:
    AddGaussianNoiseToTensorsCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
