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
#ifndef __itkAnisotropicDiffusionTensorFunction_txx
#define __itkAnisotropicDiffusionTensorFunction_txx

#include "itkAnisotropicDiffusionTensorFunction.h"

namespace itk
{

  template<class TImage>
  AnisotropicDiffusionTensorFunction<TImage>
  ::AnisotropicDiffusionTensorFunction()
  {
    
    // Set the radius: important!
    RadiusType r;
    r.Fill(1);
    this->SetRadius(r);
    
   
    m_TimeStep  = 1.0/pow (2.0,static_cast<double>(ImageDimension));
    m_Kappa     = 1.0;
    m_Lambda    = 1.0;
    m_Energy    = 0.0;
    m_OldEnergy = 9999.9;
    
    m_Gradient = NULL;
    m_Attach   = NULL;
    
  }
  

  template<class TImage>
  void
  AnisotropicDiffusionTensorFunction<TImage>
  ::InitializeIteration()
  {
    if(m_Gradient.IsNull())
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Gradient is not set.");

    if(m_Attach.IsNull())
      throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Data attachment term is not set.");

    //itkDebugMacro( << "Global Energy: " << m_Energy );
    //ScalarType diff = m_Energy - m_OldEnergy;
    //itkDebugMacro( << "Diff: " << diff );
    m_OldEnergy = m_Energy;
    m_Energy = 0.0;        
  }

  
  template<class TImage>
  typename AnisotropicDiffusionTensorFunction<TImage>::PixelType
  AnisotropicDiffusionTensorFunction<TImage>
  ::ComputeUpdate(const NeighborhoodType &it, void * gd,
                  const FloatOffsetType& itkNotUsed(offset))
  {
    
    // Get the spacing...
    typename NeighborhoodType::ImageType::ConstPointer image = it.GetImagePointer();    
    SpacingType spacing;    
    for(unsigned int i=0; i<ImageDimension; i++)
      spacing[i] = image->GetSpacing()[i];
    // ... and size of the image
    SizeType size = image->GetLargestPossibleRegion().GetSize();
    
    // The global data structure is used to pass global fields to threads.
    GlobalDataStruct* data = static_cast<GlobalDataStruct*>(gd);
    
    // Get the current pixel tensor and index
    PixelType L = it.GetCenterPixel();
    IndexType index = it.GetIndex();

    // The output pixel is initialized to the input pixel
    PixelType out = PixelType ( 0.0 );
    

    // Well, one does not process null matrices, as they compose the background of a tensor
    // image.
    
    if( !L.IsZero() )
      {
	
	PixelType laplacian( static_cast<ScalarType>(0.0) );
	//PixelType gradient( static_cast<ScalarType>(0.0) );

        for(unsigned int i=0;i<ImageDimension;i++)
        {
          // directional derivatives in the direction of axis i
          PixelType Ln = it.GetNext (i);
          PixelType Lmn = it.GetPrevious (i);

          bool isNzero = Ln.IsZero();
          bool isMNzero = Lmn.IsZero();

           if( !isNzero || !isMNzero )
           {             
             // neuman conditions
             if(isNzero && !isMNzero) // 
               Ln = Lmn;
             if(isMNzero && !isNzero) // neuman conditions
               Lmn = Ln;
           }

           
           // gradient selon i du champ de tenseurs
           //PixelType gradI =  ( Ln - Lmn ) * 0.5/ spacing[i];
           
           // laplacien selon i
           //PixelType LapI = ( Ln - L * 2.0 + Lmn ) / ( spacing[i]*spacing[i] ) ;

           // gradient selon i de C
           IndexType indexi = index;           
           IndexType indexmi = index;

           indexi[i] = indexi[i] + 1;
           indexmi[i] = indexmi[i] - 1;

           // bound check + clamping
           if( indexmi[i] < 0 )
             indexmi[i] = 0;
           if( indexi[i] > int(size[i])-1 )
             indexi[i] = int(size[i])-1;

           
           //ScalarType gradCI = ( this->C1( m_Gradient->GetPixel(indexi)  ) -
           //                      this->C1( m_Gradient->GetPixel(indexmi) ) ) * 0.5 / spacing[i];


           // Gerig's method (works better):
           laplacian += ((Ln - L) * this->C1 ( m_Gradient->GetPixel(indexi) )
                         - ( L - Lmn) * this->C1 ( m_Gradient->GetPixel(indexmi) ) )
             /(spacing[i]*spacing[i]);
                      
           
           // update the gradient
           //gradient += gradI * gradCI;
           // update le laplacien
           //laplacian += LapI;
          
        }
                
        
	//ScalarType normG = m_Gradient->GetPixel(index);
	PixelType total = laplacian;// * this->C1( normG ) + gradient;

	// a data attach term
	PixelType attach = m_Attach->GetPixel(index) - L;

        // the output pixel
	out = attach * m_Lambda + total;
	
	
      }
    
    // calculate an energy criterion
    ScalarType normG = m_Gradient->GetPixel(index);
    data->Energy += 0.5* log( 1+ normG*normG/(m_Kappa*m_Kappa) );
    //data->Energy += this->C1 (normG);
    //data->Energy += L.GetSquaredNorm();
    
        
    return out;
    
  }
  
  
  template<class TImage>
  typename AnisotropicDiffusionTensorFunction<TImage>::ScalarType
  AnisotropicDiffusionTensorFunction<TImage>
  ::C1(ScalarType n)
  {

    //return exp( -n*n/(m_Kappa*m_Kappa) );

    return 1/( 1 + n*n/(m_Kappa*m_Kappa) );
        
    //return 1.0/sqrt(1.0 + n*n/( m_Kappa*m_Kappa ));
  }

} // end of namespace

#endif
