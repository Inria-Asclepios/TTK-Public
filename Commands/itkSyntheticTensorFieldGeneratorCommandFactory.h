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
#ifndef _itk_SyntheticTensorFieldGeneratorCommandFactory_h_
#define _itk_SyntheticTensorFieldGeneratorCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class SyntheticTensorFieldGeneratorCommandFactory : public ObjectFactoryBase
  {
    
  public:
    using Self         = SyntheticTensorFieldGeneratorCommandFactory;
    using Superclass   = ObjectFactoryBase;
    using Pointer      = SmartPointer<Self>;
    using ConstPointer = SmartPointer<const Self>;
    
    /** Class methods used to interface with the registered factories. */
    const char* GetITKSourceVersion() const override;
    const char* GetDescription() const override;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static SyntheticTensorFieldGeneratorCommandFactory* FactoryNew() { return new SyntheticTensorFieldGeneratorCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(SyntheticTensorFieldGeneratorCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory()
    {
      SyntheticTensorFieldGeneratorCommandFactory::Pointer CSFFactory = SyntheticTensorFieldGeneratorCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    SyntheticTensorFieldGeneratorCommandFactory();
    ~SyntheticTensorFieldGeneratorCommandFactory() override;
    
  private:
    SyntheticTensorFieldGeneratorCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
