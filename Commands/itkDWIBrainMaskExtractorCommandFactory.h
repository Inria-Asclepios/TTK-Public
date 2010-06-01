#ifndef _itk_DWIBrainMaskExtractorCommandFactory_h_
#define _itk_DWIBrainMaskExtractorCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class DWIBrainMaskExtractorCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef DWIBrainMaskExtractorCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static DWIBrainMaskExtractorCommandFactory* FactoryNew() { return new DWIBrainMaskExtractorCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(DWIBrainMaskExtractorCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      DWIBrainMaskExtractorCommandFactory::Pointer CSFFactory = DWIBrainMaskExtractorCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    DWIBrainMaskExtractorCommandFactory();
    ~DWIBrainMaskExtractorCommandFactory();
    
  private:
    DWIBrainMaskExtractorCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
