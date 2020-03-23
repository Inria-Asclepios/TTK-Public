#pragma once

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class LogEuclideanDTIEstimatorCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef LogEuclideanDTIEstimatorCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static LogEuclideanDTIEstimatorCommandFactory* FactoryNew() { return new LogEuclideanDTIEstimatorCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(LogEuclideanDTIEstimatorCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      LogEuclideanDTIEstimatorCommandFactory::Pointer CSFFactory = LogEuclideanDTIEstimatorCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    LogEuclideanDTIEstimatorCommandFactory();
    ~LogEuclideanDTIEstimatorCommandFactory();
    
  private:
    LogEuclideanDTIEstimatorCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}
