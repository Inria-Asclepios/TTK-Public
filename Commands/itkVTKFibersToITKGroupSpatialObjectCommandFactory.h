#ifndef _itk_VTKFibersToITKGroupSpatialObjectCommandFactory_h_
#define _itk_VTKFibersToITKGroupSpatialObjectCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class VTKFibersToITKGroupSpatialObjectCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef VTKFibersToITKGroupSpatialObjectCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static VTKFibersToITKGroupSpatialObjectCommandFactory* FactoryNew() { return new VTKFibersToITKGroupSpatialObjectCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(VTKFibersToITKGroupSpatialObjectCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      VTKFibersToITKGroupSpatialObjectCommandFactory::Pointer CSFFactory = VTKFibersToITKGroupSpatialObjectCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    VTKFibersToITKGroupSpatialObjectCommandFactory();
    ~VTKFibersToITKGroupSpatialObjectCommandFactory();
    
  private:
    VTKFibersToITKGroupSpatialObjectCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
