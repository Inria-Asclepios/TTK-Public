#ifndef _itk_LogEuclideanDTIEstimatorTensorFunction_txx_
#define _itk_LogEuclideanDTIEstimatorTensorFunction_txx_

#include <itkVector.h>
#include "itkLogEuclideanDTIEstimatorTensorFunction.h"


namespace itk
{

  static double BesselArray[12] = {1.0, 0.5, 0.125, 0.125, 0.1953125, 0.40625,
                                   1.0478515625, 3.21875, 11.46646118164, 46.478515625,
                                   211.276149749755, 1064.678222656250};

  template<class TImage>
  LogEuclideanDTIEstimatorTensorFunction<TImage>
  ::LogEuclideanDTIEstimatorTensorFunction()
  {    
    
    // Set the radius
    RadiusType r;
    r.Fill(1);
    this->SetRadius(r);

    m_NumberOfIterations = 0;

    m_C = 1.0;    
    m_TimeStep = 1.0;
    m_TimeStepB0 = 1.0;
    
    m_Lambda =   0.0;
    m_Energy =   0.0;
    m_B0Energy = 0.0;
    m_Kappa =    1.0;
    m_OldEnergy = 9999.0;
    m_OldB0Energy = 9999.0;
    m_Gradient = NULL;
    
    // new
    m_B0 = ScalarImageType::New();

    m_EstimatorType = 0;
            
  }
  
  
  template<class TImage>
  void
  LogEuclideanDTIEstimatorTensorFunction<TImage>
  ::InitializeIteration()
  {    
    
    if(m_Gradient.IsNull())
	throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Gradient is not set.");

    if(m_NumberOfIterations == 0)
    {
      // Allocate the output B0:
      typename ScalarImageType::RegionType region = m_DTI[0]->GetLargestPossibleRegion();
      m_B0->SetRegions(region);
      m_B0->SetSpacing ( m_DTI[0]->GetSpacing() );
      m_B0->SetOrigin ( m_DTI[0]->GetOrigin() );      
      m_B0->Allocate();

      // copy the buffer:
      typedef ImageRegionIterator<ScalarImageType> IteratorType;
      IteratorType itB0 (m_B0,m_B0->GetLargestPossibleRegion());
      IteratorType itDTI (m_DTI[0],m_DTI[0]->GetLargestPossibleRegion());
      while(!itDTI.IsAtEnd())
      {
        itB0.Set (itDTI.Get());
        m_B0Energy += itB0.Value()*itB0.Value();        
        ++itDTI;
        ++itB0;
      }
      
            
    }
    
    m_NumberOfIterations++;
    

    std::cout << "Global Energy: " << m_Energy << std::endl;
    //ScalarType diff = ( m_Energy-m_OldEnergy ) / m_OldEnergy;
    ScalarType diff = m_Energy-m_OldEnergy ;
    std::cout << "Diff: " << diff << std::endl;
    std::cout << "Global B0 Energy: " << m_B0Energy << std::endl;
    ScalarType diffB0 = m_B0Energy - m_OldB0Energy;
    std::cout << "Diff B0: " << diffB0 << std::endl;
    m_OldEnergy = m_Energy;
    m_OldB0Energy = m_B0Energy;
    m_Energy = 0.0;
    m_B0Energy = 0.0;
  }
  
  
  template<class TImage>
  typename LogEuclideanDTIEstimatorTensorFunction<TImage>::PixelType
  LogEuclideanDTIEstimatorTensorFunction<TImage>
  ::ComputeUpdate(const NeighborhoodType &it, void * gd,
                const FloatOffsetType& itkNotUsed(offset))
  {
    
    PixelType L = it.GetCenterPixel();
    IndexType index = it.GetIndex();


    PixelType out = PixelType ( 0.0 );
    unsigned int n = m_GradientList.size();
    
    // get the spacing
    typename NeighborhoodType::ImageType::ConstPointer image = it.GetImagePointer();
    SpacingType spacing;    
    for(unsigned int i=0; i<ImageDimension; i++)
      spacing[i] = image->GetSpacing()[i];
    // ... and size of the image
    SizeType size = m_Gradient->GetLargestPossibleRegion().GetSize();
    /*
    int total = 1;
    for( unsigned int i=0;i<ImageDimension;i++)
      total *= size[i];
    */
    
        

    ScalarType b0 = m_B0->GetPixel(index);

    GlobalDataStruct* data = static_cast<GlobalDataStruct*>(gd);
    
    // A simple thresholding is used to remove the background from the useful data
    if(b0>m_BST)
      {

	// first term: the data attach
        PixelType Data( 0.0 );
        
	for(unsigned int i=0; i<n; i++)
        {
          
          // trace(< exp(L) | gi giT >):
          PixelType G (m_GradientList[i]); // tensor product
          ScalarType trace = G.ScalarProductWith ( L.Exp() );
	  
          // si = exp( -< exp(L) | gi giT > )
          ScalarType si = 0.0; //b0*exp(-1.0*trace);
          
          // resi = si - siobs
          ScalarType resi = 0.0;
          
                    
          // Differential of the exponential
          PixelType diffExp = L.DifferentialExp (G);

	  //std::cout << L << " " << G << " " << diffExp << std::endl;
	  //getchar();

          
          ScalarType bi = m_DTI[i+1]->GetPixel(index);

          switch( m_EstimationType )
          {
                            
              case 0:
                
                si = b0/bi;
                resi = log ( si ) - trace;
                Data -= diffExp * this->MEstimator(resi);
                break;
                
                
              case 1:
                
                //si = b0*exp (-trace);
                //resi = bi - si;
                si = exp (-trace);
                resi = bi/b0 - si;
                Data += diffExp * this->MEstimator(resi) * si;                  
                break;
                
                /*
              case 2:
                
                si = (b0*b0 - 2*m_Variances[0])*exp (-2.0*trace);
                resi = bi*bi - 2.0*SIGMA2 - si;
                Data += diffExp * this->MEstimator (resi) * si;
                break;
                
              case 3:
                
                si = ( b0 - m_Variances[0]/(2.0*b0) )*exp(-trace);
                resi = bi - SIGMA2/(2.0*bi) - si;
                Data += diffExp * this->MEstimator (resi) *si;
                break;
                */
              case 2:
                
                //si = ( b0 )*exp(-trace);
                //resi = si - this->BesselRatio (bi*si/SIGMA2)*bi;

                si = exp(-trace);
                resi = si - this->BesselRatio (bi*b0*si/m_Variances[i+1])*bi/b0;		

                Data -= diffExp*resi*si;
                break;
                
                
              default:
                throw itk::ExceptionObject (__FILE__,__LINE__,"Error: Estimation type not recognized.");
                break;
          }
          
        }

        
        // normalization
        Data *=  -1.0 / static_cast<ScalarType>(n);

	PixelType Total = Data;
         
	// second term: anisotropic diffusion

	if( m_Lambda > 0.0 )
	{
	  PixelType Lap( static_cast<ScalarType>( 0.0 ) );
	  
	  for( unsigned int i=0; i<ImageDimension; i++ )
	  {
	    PixelType Ln = it.GetNext (i);
	    PixelType Lmn = it.GetPrevious (i);
	    
	    bool isNzero = Ln.IsZero();
	    bool isMNzero = Lmn.IsZero();
	    
	    if( !isNzero || !isMNzero )
	    {
	      if(isNzero && !isMNzero) // neuman conditions
		Ln = Lmn;
	      if(isMNzero && !isNzero) // neuman conditions
		Lmn = Ln;
	      
	      IndexType indexi = index;           
	      IndexType indexmi = index;
	      
	      indexi[i] = indexi[i] + 1;
	      indexmi[i] = indexmi[i] - 1;
	      
	      // bound check + clamping
	      if( indexmi[i] < 0 )
		indexmi[i] = 0;
	      if( indexi[i] > int(size[i])-1 )
		indexi[i] = int(size[i])-1;
	      
	      
	      Lap += ( (Ln - L) * this->C1 ( m_Gradient->GetPixel(indexi) ) -
		       (L - Lmn) * this->C1 ( m_Gradient->GetPixel(indexmi) ) )
		/(spacing[i]*spacing[i]);
	      
	    }
	    
	  }
	  
	  Total += Lap * m_Lambda;
	  /*std::cout << Data;
	    std::cout << Reg * m_Lambda << std::endl;
	    getchar();*/
	}
	
	out = Total;
        
	// calculate an energy criterion
        data->Energy +=  Total.GetNorm(); // / (ScalarType)(total);
        
      } // end if thresholding
    
    
    return out;
    
    
  }



  template<class TImage>
  typename LogEuclideanDTIEstimatorTensorFunction<TImage>::ScalarType
  LogEuclideanDTIEstimatorTensorFunction<TImage>
  ::BesselRatio ( const ScalarType &x )
  {
    ScalarType res = 0;

    if (x>10)
    {
      ScalarType X = x;
      res = 1.0;
      for( unsigned int i=1; i<12; i++ )
      {
        res -= BesselArray[i]/X;
        X *= x;
      }
    }
    else
    {
      ScalarType v = 0.5*x;
      ScalarType v2 = v*v;
      
      ScalarType I0=1.0;
      ScalarType A0=1.0;
      ScalarType I1=v;
      ScalarType A1=v;

      ScalarType EPS=1e-12;
      
      unsigned int N=30;
      for( unsigned int k=1; k<N; k++ )
      {
        A0 *= v2/(static_cast<ScalarType>(k*k));
        A1 *= v2/(static_cast<ScalarType>(k*k+k));

        if( A0<EPS && A1<EPS)
          break;

        I1 += A1;
        I0 += A0;
        
      }

      res = I1/I0;
      
    }


    return res;
        
  }
  
  
  template<class TImage>
  typename LogEuclideanDTIEstimatorTensorFunction<TImage>::ScalarType
  LogEuclideanDTIEstimatorTensorFunction<TImage>
  ::C1(const ScalarType &val)
  {

    //return exp(-val*val/(m_Kappa*m_Kappa) );

    return 1/( 1 + val*val/(m_Kappa*m_Kappa) );
    
    //return static_cast<ScalarType>( 1.0/sqrt(1 + val*val/(m_Kappa*m_Kappa)) );
  }


  template<class TImage>
  typename LogEuclideanDTIEstimatorTensorFunction<TImage>::ScalarType
  LogEuclideanDTIEstimatorTensorFunction<TImage>
  ::MEstimator (const ScalarType &val)
  {
    
    ScalarType res = 0.0;    
    ScalarType C2 = 0.0;
    ScalarType quotient = 0.0;
      
    switch(m_EstimatorType)
    {            
        case 0:// Least square estimator

          res = val;
          
          break;
          
        case 1: // McLure's estimator
          
          C2 = m_C*m_C;
          quotient = val*val + C2;    
          res = val*C2/(quotient*quotient);
          
          break;

        case 2: // Huber's function


          if ( fabs (val) <= m_C )
            res = val;
          else
            res = m_C*val / fabs (val);
          

          break;
          
          
        default:

          throw itk::ExceptionObject(__FILE__,__LINE__,"Error: M-Estimator not specified.");
                    
          break;          
    }
    
    return res;
    
  }


  template<class TImage>
  typename LogEuclideanDTIEstimatorTensorFunction<TImage>::ScalarType
  LogEuclideanDTIEstimatorTensorFunction<TImage>
  ::PHI (const ScalarType &val)
  {

    ScalarType res = 0.0;

    switch(m_EstimatorType)
    {

        case 0: // Least square
          
          res = 0.5*val*val;
          
          break;


        case 1: // Mc Lure

          res = 0.5*val*val/(val*val + m_C*m_C);

          break;

        case 2: // Huber's function

          if ( fabs (val) <= m_C )
            res = 0.5*val*val;
          else
            res = m_C*( fabs (val) - 0.5*m_C );

          break;
          
          
        default:

          throw itk::ExceptionObject(__FILE__,__LINE__,"Error: M-Estimator not specified.");
          
          break;
                
    }    

    
    return res;
    
  }
  
  


} // end of namespace mips

#endif
