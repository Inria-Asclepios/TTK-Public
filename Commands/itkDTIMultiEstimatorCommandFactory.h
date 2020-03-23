#pragma once

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class DTIMultiEstimatorCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef DTIMultiEstimatorCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static DTIMultiEstimatorCommandFactory* FactoryNew() { return new DTIMultiEstimatorCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(DTIMultiEstimatorCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      DTIMultiEstimatorCommandFactory::Pointer CSFFactory = DTIMultiEstimatorCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    DTIMultiEstimatorCommandFactory();
    ~DTIMultiEstimatorCommandFactory();
    
  private:
    DTIMultiEstimatorCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}
