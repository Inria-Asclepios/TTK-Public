#pragma once

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class DTIEstimatorCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef DTIEstimatorCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static DTIEstimatorCommandFactory* FactoryNew() { return new DTIEstimatorCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(DTIEstimatorCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      DTIEstimatorCommandFactory::Pointer CSFFactory = DTIEstimatorCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    DTIEstimatorCommandFactory();
    ~DTIEstimatorCommandFactory();
    
  private:
    DTIEstimatorCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}
