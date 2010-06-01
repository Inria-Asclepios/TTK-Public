#ifndef _itk_DTIEstimatorWithBFGSCommandFactory_h_
#define _itk_DTIEstimatorWithBFGSCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class DTIEstimatorWithBFGSCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef DTIEstimatorWithBFGSCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static DTIEstimatorWithBFGSCommandFactory* FactoryNew() { return new DTIEstimatorWithBFGSCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(DTIEstimatorWithBFGSCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      DTIEstimatorWithBFGSCommandFactory::Pointer CSFFactory = DTIEstimatorWithBFGSCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    DTIEstimatorWithBFGSCommandFactory();
    ~DTIEstimatorWithBFGSCommandFactory();
    
  private:
    DTIEstimatorWithBFGSCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
