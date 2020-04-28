/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: https://scm.gforge.inria.fr/svn/ttk/trunk/Commands/itkAddRicianNoiseToTensorsCommandFactory.h $
  Language:  C++
  Date:      $Date: 2010-12-22 10:25:59 +0000 (Wed, 22 Dec 2010) $
  Version:   $Revision: 124 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_AddRicianNoiseToImageCommandFactory_h_
#define _itk_AddRicianNoiseToImageCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class AddRicianNoiseToImageCommandFactory : public ObjectFactoryBase
  {
    
  public:
    using Self         = AddRicianNoiseToImageCommandFactory;
    using Superclass   = ObjectFactoryBase;
    using Pointer      = SmartPointer<Self>;
    using ConstPointer = SmartPointer<const Self>;
    
    /** Class methods used to interface with the registered factories. */
    const char* GetITKSourceVersion() const override;
    const char* GetDescription() const override;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static AddRicianNoiseToImageCommandFactory* FactoryNew() { return new AddRicianNoiseToImageCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(AddRicianNoiseToImageCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory()
    {
      AddRicianNoiseToImageCommandFactory::Pointer CSFFactory = AddRicianNoiseToImageCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    AddRicianNoiseToImageCommandFactory();
    ~AddRicianNoiseToImageCommandFactory() override;
    
  private:
    AddRicianNoiseToImageCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
