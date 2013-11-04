/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: https://scm.gforge.inria.fr/svn/ttk/trunk/Commands/itkComputeBundleStatisticsCommand.h $
  Language:  C++
  Date:      $Date: 2010-12-22 11:25:59 +0100 (mer., 22 déc. 2010) $
  Version:   $Revision: 124 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_ComputeBundleStatisticsCommand_h_
#define _itk_ComputeBundleStatisticsCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class ComputeBundleStatisticsCommand : public CommandObjectBase
  {
    
  public:
		
    typedef ComputeBundleStatisticsCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(ComputeBundleStatisticsCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "bundle_stat"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    ComputeBundleStatisticsCommand();
    ~ComputeBundleStatisticsCommand();
    
  private:
    ComputeBundleStatisticsCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
