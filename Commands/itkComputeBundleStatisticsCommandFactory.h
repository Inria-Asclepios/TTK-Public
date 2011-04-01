/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: https://scm.gforge.inria.fr/svn/ttk/trunk/Commands/itkComputeBundleStatisticsCommandFactory.h $
  Language:  C++
  Date:      $Date: 2010-12-22 11:25:59 +0100 (mer., 22 déc. 2010) $
  Version:   $Revision: 124 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_ComputeBundleStatisticsCommandFactory_h_
#define _itk_ComputeBundleStatisticsCommandFactory_h_

#include "itkObjectFactoryBase.h"

namespace itk
{
  
  class ComputeBundleStatisticsCommandFactory : public ObjectFactoryBase
  {
    
  public:
    typedef ComputeBundleStatisticsCommandFactory Self;
    typedef ObjectFactoryBase        Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Class methods used to interface with the registered factories. */
    virtual const char* GetITKSourceVersion(void) const;
    virtual const char* GetDescription(void) const;
    
    /** Method for class instantiation. */
    itkFactorylessNewMacro(Self);
    static ComputeBundleStatisticsCommandFactory* FactoryNew() { return new ComputeBundleStatisticsCommandFactory;}
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(ComputeBundleStatisticsCommandFactory, ObjectFactoryBase);
    
    /** Register one factory of this type  */
    static void RegisterOneFactory(void)
    {
      ComputeBundleStatisticsCommandFactory::Pointer CSFFactory = ComputeBundleStatisticsCommandFactory::New();
      ObjectFactoryBase::RegisterFactory( CSFFactory );
    }
    
		
  protected:
    ComputeBundleStatisticsCommandFactory();
    ~ComputeBundleStatisticsCommandFactory();
    
  private:
    ComputeBundleStatisticsCommandFactory(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
