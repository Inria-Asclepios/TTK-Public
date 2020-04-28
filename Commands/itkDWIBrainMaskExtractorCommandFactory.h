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
#ifndef _itk_DWIBrainMaskExtractorCommandFactory_h_
#define _itk_DWIBrainMaskExtractorCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class DWIBrainMaskExtractorCommandFactory : public ObjectFactoryBase
  {
    
  public:
    using Self         = DWIBrainMaskExtractorCommandFactory;
    using Superclass   = ObjectFactoryBase;
    using Pointer      = SmartPointer<Self>;
    using ConstPointer = SmartPointer<const Self>;
    
    /** Class methods used to interface with the registered factories. */
    const char* GetITKSourceVersion() const override;
    const char* GetDescription() const override;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static DWIBrainMaskExtractorCommandFactory* FactoryNew() { return new DWIBrainMaskExtractorCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(DWIBrainMaskExtractorCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory()
    {
      DWIBrainMaskExtractorCommandFactory::Pointer CSFFactory = DWIBrainMaskExtractorCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    DWIBrainMaskExtractorCommandFactory();
    ~DWIBrainMaskExtractorCommandFactory() override;
    
  private:
    DWIBrainMaskExtractorCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
