#ifndef _itk_TensorNormalizedGaussianInterpolationCommandFactory_h_
#define _itk_TensorNormalizedGaussianInterpolationCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class TensorNormalizedGaussianInterpolationCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef TensorNormalizedGaussianInterpolationCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static TensorNormalizedGaussianInterpolationCommandFactory* FactoryNew() { return new TensorNormalizedGaussianInterpolationCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(TensorNormalizedGaussianInterpolationCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      TensorNormalizedGaussianInterpolationCommandFactory::Pointer CSFFactory = TensorNormalizedGaussianInterpolationCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    TensorNormalizedGaussianInterpolationCommandFactory();
    ~TensorNormalizedGaussianInterpolationCommandFactory();
    
  private:
    TensorNormalizedGaussianInterpolationCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
