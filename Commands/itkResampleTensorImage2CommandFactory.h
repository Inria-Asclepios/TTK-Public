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
#ifndef _itk_ResampleTensorImage2CommandFactory_h_
#define _itk_ResampleTensorImage2CommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class ResampleTensorImage2CommandFactory : public ObjectFactoryBase
  {
    
  public:
    using Self         = ResampleTensorImage2CommandFactory;
    using Superclass   = ObjectFactoryBase;
    using Pointer      = SmartPointer<Self>;
    using ConstPointer = SmartPointer<const Self>;
    
    /** Class methods used to interface with the registered factories. */
    const char* GetITKSourceVersion() const override;
    const char* GetDescription() const override;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static ResampleTensorImage2CommandFactory* FactoryNew() { return new ResampleTensorImage2CommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(ResampleTensorImage2CommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory()
    {
      ResampleTensorImage2CommandFactory::Pointer CSFFactory = ResampleTensorImage2CommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    ResampleTensorImage2CommandFactory();
    ~ResampleTensorImage2CommandFactory() override;
    
  private:
    ResampleTensorImage2CommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
