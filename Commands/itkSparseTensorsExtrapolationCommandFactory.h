#ifndef _itk_SparseTensorsExtrapolationCommandFactory_h_
#define _itk_SparseTensorsExtrapolationCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class SparseTensorsExtrapolationCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef SparseTensorsExtrapolationCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static SparseTensorsExtrapolationCommandFactory* FactoryNew() { return new SparseTensorsExtrapolationCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(SparseTensorsExtrapolationCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      SparseTensorsExtrapolationCommandFactory::Pointer CSFFactory = SparseTensorsExtrapolationCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    SparseTensorsExtrapolationCommandFactory();
    ~SparseTensorsExtrapolationCommandFactory();
    
  private:
    SparseTensorsExtrapolationCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
