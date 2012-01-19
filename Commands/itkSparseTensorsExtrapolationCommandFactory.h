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
#ifndef _itk_SparseTensorsExtrapolationCommandFactory_h_
#define _itk_SparseTensorsExtrapolationCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class SparseTensorsExtrapolationCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef SparseTensorsExtrapolationCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static SparseTensorsExtrapolationCommandFactory* FactoryNew() { return new SparseTensorsExtrapolationCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(SparseTensorsExtrapolationCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      SparseTensorsExtrapolationCommandFactory::Pointer CSFFactory = SparseTensorsExtrapolationCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    SparseTensorsExtrapolationCommandFactory();
    ~SparseTensorsExtrapolationCommandFactory();
    
  private:
    SparseTensorsExtrapolationCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
