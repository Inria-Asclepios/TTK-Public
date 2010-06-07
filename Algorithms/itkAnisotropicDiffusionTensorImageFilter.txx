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
#ifndef _itk_AnisotropicDiffusionTensorImageFilter_txx_
#define _itk_AnisotropicDiffusionTensorImageFilter_txx_

#include "itkAnisotropicDiffusionTensorImageFilter.h"

#include <iostream>
#include <fstream>
#include <string>
  
namespace itk
{
  

  template< class TInputImage, class TOutputImage >
  bool
  AnisotropicDiffusionTensorImageFilter< TInputImage, TOutputImage >
  ::Halt()
  {
//     double nite = this->GetNumberOfIterations();
//     double lambda = m_Function->GetLambda();
//     double timestep = m_Function->GetTimeStep();
//     double kappa = m_Function->GetKappa();
//     char temp[512];
//     sprintf (temp,"Convergence_N%fL%fT%fK%f.csv", (float)nite,(float)lambda,(float)timestep,(float)kappa);
//     std::string filename = temp;
    
//     std::ofstream buffer (filename.c_str(), std::ofstream::app);
//     if( !buffer.fail() )
//     {
//       buffer << (int)this->GetRMSChange();
//       buffer << "\n";
      
//     }
//     buffer.close();
    
    
    itkDebugMacro(  << "Delta Energy: " << this->GetRMSChange() );
    
    if (this->GetNumberOfIterations() != 0)
      {
	this->UpdateProgress( static_cast<float>( this->GetElapsedIterations() ) /
			      static_cast<float>( this->GetNumberOfIterations() ) );
      }
    
    if (this->GetElapsedIterations() >= this->GetNumberOfIterations())
      {
        this->UpdateProgress(1.0);
	return true;
      }
    else if ( this->GetElapsedIterations() == 0)
      {
	return false; 
      }
    else if ( this->GetRMSChange() <= this->GetMaximumRMSError() )
      {
        this->UpdateProgress(1.0);
	return true;
      }
    else
      { 
	return false; 
      }
    
  }

  template< class TInputImage, class TOutputImage >
  const double
  AnisotropicDiffusionTensorImageFilter< TInputImage, TOutputImage >
  ::GetRMSChange()
  { 
    return m_Function->GetRMSChange();
  }



  template< class TInputImage, class TOutputImage >
  void
  AnisotropicDiffusionTensorImageFilter<TInputImage,TOutputImage>
  ::InitializeIteration()
  {

    this->UpdateProgress( (double)(this->GetElapsedIterations())/(double)(this->GetNumberOfIterations()) );
    
    itkDebugMacro( << "############ Iteration No " << this->GetElapsedIterations() << " ############" );
    if( this->GetElapsedIterations() == 0 )
    {      
      m_GradientFilter->SetInput(this->GetInput());
      m_Function->SetAttach ( this->GetInput() );      
    }    
    else
    {
      this->GetOutput()->Modified();
      m_GradientFilter->SetInput(this->GetOutput());
    }    


    if( this->GetAbortGenerateData() )
    {
      throw itk::ProcessAborted(__FILE__,__LINE__);
    }
    
      
    itkDebugMacro( << "Updating the gradient...");
    try
    {
      m_GradientFilter->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject(__FILE__,
                                 __LINE__,
                                 "Error in AnisotropicDiffusionTensorImageFilter::InitializeIteration()");
    }
    itkDebugMacro( << "Done." );
    
    m_Function->SetGradient(m_GradientFilter->GetOutput());
    m_Function->InitializeIteration();

  }
  

} // end of namespace

#endif
