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
#ifndef _itk_WarpImageCommandFactory_h_
#define _itk_WarpImageCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class WarpImageCommandFactory : public ObjectFactoryBase
  {
    
  public:
    using Self         = WarpImageCommandFactory;
    using Superclass   = ObjectFactoryBase;
    using Pointer      = SmartPointer<Self>;
    using ConstPointer = SmartPointer<const Self>;
    
    /** Class methods used to interface with the registered factories. */
    const char* GetITKSourceVersion() const override;
    const char* GetDescription() const override;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static WarpImageCommandFactory* FactoryNew() { return new WarpImageCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(WarpImageCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory()
    {
      WarpImageCommandFactory::Pointer CSFFactory = WarpImageCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    WarpImageCommandFactory();
    ~WarpImageCommandFactory() override;
    
  private:
    WarpImageCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
