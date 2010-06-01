#ifndef _itk_ImageConverterCommandFactory_h_
#define _itk_ImageConverterCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class ImageConverterCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef ImageConverterCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static ImageConverterCommandFactory* FactoryNew() { return new ImageConverterCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(ImageConverterCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      ImageConverterCommandFactory::Pointer CSFFactory = ImageConverterCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    ImageConverterCommandFactory();
    ~ImageConverterCommandFactory();
    
  private:
    ImageConverterCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
