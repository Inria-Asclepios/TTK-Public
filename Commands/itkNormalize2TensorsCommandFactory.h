#ifndef _itk_Normalize2TensorsCommandFactory_h_
#define _itk_Normalize2TensorsCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class Normalize2TensorsCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef Normalize2TensorsCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static Normalize2TensorsCommandFactory* FactoryNew() { return new Normalize2TensorsCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(Normalize2TensorsCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      Normalize2TensorsCommandFactory::Pointer CSFFactory = Normalize2TensorsCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    Normalize2TensorsCommandFactory();
    ~Normalize2TensorsCommandFactory();
    
  private:
    Normalize2TensorsCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
