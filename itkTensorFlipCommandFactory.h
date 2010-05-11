#ifndef _itk_TensorFlipCommandFactory_h_
#define _itk_TensorFlipCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class TensorFlipCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef TensorFlipCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static TensorFlipCommandFactory* FactoryNew() { return new TensorFlipCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(TensorFlipCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      TensorFlipCommandFactory::Pointer CSFFactory = TensorFlipCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    TensorFlipCommandFactory();
    ~TensorFlipCommandFactory();
    
  private:
    TensorFlipCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
