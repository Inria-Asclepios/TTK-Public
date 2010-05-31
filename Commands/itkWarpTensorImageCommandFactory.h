#ifndef _itk_WarpTensorImageCommandFactory_h_
#define _itk_WarpTensorImageCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class WarpTensorImageCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef WarpTensorImageCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static WarpTensorImageCommandFactory* FactoryNew() { return new WarpTensorImageCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(WarpTensorImageCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      WarpTensorImageCommandFactory::Pointer CSFFactory = WarpTensorImageCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    WarpTensorImageCommandFactory();
    ~WarpTensorImageCommandFactory();
    
  private:
    WarpTensorImageCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
