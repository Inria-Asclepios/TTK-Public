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
#ifndef _itk_Average4DImageCommandFactory_h_
#define _itk_Average4DImageCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class Average4DImageCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef Average4DImageCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static Average4DImageCommandFactory* FactoryNew() { return new Average4DImageCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(Average4DImageCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      Average4DImageCommandFactory::Pointer CSFFactory = Average4DImageCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    Average4DImageCommandFactory();
    ~Average4DImageCommandFactory();
    
  private:
    Average4DImageCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
