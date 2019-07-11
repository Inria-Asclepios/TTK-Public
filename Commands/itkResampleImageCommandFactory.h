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
#ifndef _itk_ResampleImageCommandFactory_h_
#define _itk_ResampleImageCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class ResampleImageCommandFactory : public ObjectFactoryBase
  {
    
  public:
    using Self         = ResampleImageCommandFactory;
    using Superclass   = ObjectFactoryBase;
    using Pointer      = SmartPointer<Self>;
    using ConstPointer = SmartPointer<const Self>;
    
    /** Class methods used to interface with the registered factories. */
    const char* GetITKSourceVersion() const override;
    const char* GetDescription() const override;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static ResampleImageCommandFactory* FactoryNew() { return new ResampleImageCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(ResampleImageCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory()
    {
      ResampleImageCommandFactory::Pointer CSFFactory = ResampleImageCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    ResampleImageCommandFactory();
    ~ResampleImageCommandFactory() override;
    
  private:
    ResampleImageCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
