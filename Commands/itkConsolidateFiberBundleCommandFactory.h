/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL:$
  Language:  C++
  Date:      $Date:$
  Version:   $Revision:$

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_ConsolidateFiberBundleCommandFactory_h_
#define _itk_ConsolidateFiberBundleCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class ConsolidateFiberBundleCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef ConsolidateFiberBundleCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static ConsolidateFiberBundleCommandFactory* FactoryNew() { return new ConsolidateFiberBundleCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(ConsolidateFiberBundleCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      ConsolidateFiberBundleCommandFactory::Pointer CSFFactory = ConsolidateFiberBundleCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    ConsolidateFiberBundleCommandFactory();
    ~ConsolidateFiberBundleCommandFactory();
    
  private:
    ConsolidateFiberBundleCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
