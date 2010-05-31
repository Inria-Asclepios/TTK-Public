#ifndef _itk_ResampleTensorImage2CommandFactory_h_
#define _itk_ResampleTensorImage2CommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class ResampleTensorImage2CommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef ResampleTensorImage2CommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static ResampleTensorImage2CommandFactory* FactoryNew() { return new ResampleTensorImage2CommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(ResampleTensorImage2CommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      ResampleTensorImage2CommandFactory::Pointer CSFFactory = ResampleTensorImage2CommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    ResampleTensorImage2CommandFactory();
    ~ResampleTensorImage2CommandFactory();
    
  private:
    ResampleTensorImage2CommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
