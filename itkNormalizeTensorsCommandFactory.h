#ifndef _itk_NormalizeTensorsCommandFactory_h_
#define _itk_NormalizeTensorsCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class NormalizeTensorsCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef NormalizeTensorsCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static NormalizeTensorsCommandFactory* FactoryNew() { return new NormalizeTensorsCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(NormalizeTensorsCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      NormalizeTensorsCommandFactory::Pointer CSFFactory = NormalizeTensorsCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    NormalizeTensorsCommandFactory();
    ~NormalizeTensorsCommandFactory();
    
  private:
    NormalizeTensorsCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
