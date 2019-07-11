/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: $
  Language:  C++
  Date:      $Date: 2010-08-17 09:58:00 +0200 (mar., 17 août 2010) $
  Version:   $Revision: 97 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_NDImageToTensorImageCommandFactory_h_
#define _itk_NDImageToTensorImageCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class NDImageToTensorImageCommandFactory : public ObjectFactoryBase
  {
    
  public:
    using Self         = NDImageToTensorImageCommandFactory;
    using Superclass   = ObjectFactoryBase;
    using Pointer      = SmartPointer<Self>;
    using ConstPointer = SmartPointer<const Self>;
    
    /** Class methods used to interface with the registered factories. */
    const char* GetITKSourceVersion() const override;
    const char* GetDescription() const override;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static NDImageToTensorImageCommandFactory* FactoryNew() { return new NDImageToTensorImageCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(NDImageToTensorImageCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory()
    {
      NDImageToTensorImageCommandFactory::Pointer CSFFactory = NDImageToTensorImageCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
     NDImageToTensorImageCommandFactory();
    ~NDImageToTensorImageCommandFactory() override;
    
  private:
    NDImageToTensorImageCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
