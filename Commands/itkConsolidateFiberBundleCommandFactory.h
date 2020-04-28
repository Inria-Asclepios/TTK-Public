/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_ConsolidateFiberBundleCommandFactory_h_
#define _itk_ConsolidateFiberBundleCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class ConsolidateFiberBundleCommandFactory : public ObjectFactoryBase
  {
    
  public:
    using Self         = ConsolidateFiberBundleCommandFactory;
    using Superclass   = ObjectFactoryBase;
    using Pointer      = SmartPointer<Self>;
    using ConstPointer = SmartPointer<const Self>;
    
    /** Class methods used to interface with the registered factories. */
    const char* GetITKSourceVersion() const override;
    const char* GetDescription() const override;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static ConsolidateFiberBundleCommandFactory* FactoryNew() { return new ConsolidateFiberBundleCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(ConsolidateFiberBundleCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory()
    {
      ConsolidateFiberBundleCommandFactory::Pointer CSFFactory = ConsolidateFiberBundleCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    ConsolidateFiberBundleCommandFactory();
    ~ConsolidateFiberBundleCommandFactory() override;
    
  private:
    ConsolidateFiberBundleCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
