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
    typedef NDImageToTensorImageCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static NDImageToTensorImageCommandFactory* FactoryNew() { return new NDImageToTensorImageCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(NDImageToTensorImageCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      NDImageToTensorImageCommandFactory::Pointer CSFFactory = NDImageToTensorImageCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
     NDImageToTensorImageCommandFactory();
    ~NDImageToTensorImageCommandFactory();
    
  private:
    NDImageToTensorImageCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
