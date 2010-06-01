#ifndef _itk_AverageStdCalculatorCommandFactory_h_
#define _itk_AverageStdCalculatorCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class AverageStdCalculatorCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef AverageStdCalculatorCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static AverageStdCalculatorCommandFactory* FactoryNew() { return new AverageStdCalculatorCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(AverageStdCalculatorCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      AverageStdCalculatorCommandFactory::Pointer CSFFactory = AverageStdCalculatorCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    AverageStdCalculatorCommandFactory();
    ~AverageStdCalculatorCommandFactory();
    
  private:
    AverageStdCalculatorCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
