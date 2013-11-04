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
#ifndef _itk_Permute23TensorCoefficientsCommandFactory_h_
#define _itk_Permute23TensorCoefficientsCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class Permute23TensorCoefficientsCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef Permute23TensorCoefficientsCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static Permute23TensorCoefficientsCommandFactory* FactoryNew() { return new Permute23TensorCoefficientsCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(Permute23TensorCoefficientsCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      Permute23TensorCoefficientsCommandFactory::Pointer CSFFactory = Permute23TensorCoefficientsCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    Permute23TensorCoefficientsCommandFactory();
    ~Permute23TensorCoefficientsCommandFactory();
    
  private:
    Permute23TensorCoefficientsCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
