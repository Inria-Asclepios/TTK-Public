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
#ifndef _itk_Normalize2TensorsCommandFactory_h_
#define _itk_Normalize2TensorsCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class Normalize2TensorsCommandFactory : public ObjectFactoryBase
  {
    
  public:
    using Self         = Normalize2TensorsCommandFactory;
    using Superclass   = ObjectFactoryBase;
    using Pointer      = SmartPointer<Self>;
    using ConstPointer = SmartPointer<const Self>;
    
    /** Class methods used to interface with the registered factories. */
    const char* GetITKSourceVersion() const override;
    const char* GetDescription() const override;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static Normalize2TensorsCommandFactory* FactoryNew() { return new Normalize2TensorsCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(Normalize2TensorsCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory()
    {
      Normalize2TensorsCommandFactory::Pointer CSFFactory = Normalize2TensorsCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    Normalize2TensorsCommandFactory();
    ~Normalize2TensorsCommandFactory() override;
    
  private:
    Normalize2TensorsCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
