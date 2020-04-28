#pragma once

#include "itkDTIEstimatorWithBFGSTensorImageFilter.h"

#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIterator.h>

#include <math.h>

namespace itk
{


  static double BesselArray[12] = {1.0, 0.5, 0.125, 0.125, 0.1953125, 0.40625,
                                   1.0478515625, 3.21875, 11.46646118164, 46.478515625,
                                   211.276149749755, 1064.678222656250};
  

  template<class TInputImage, class TOutputImage>
  void
  DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>
  ::BeforeThreadedGenerateData()
  {
  
    if( m_GradientList.size()+1 != this->GetNumberOfIndexedInputs() )
      throw itk::ExceptionObject(__FILE__,__LINE__,
                                 "Error: The lengths of the gradient list and number of inputs does not match.");

  }

  template<class TInputImage, class TOutputImage>
  typename DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>::ScalarType
  DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>
  ::Simulator(const DataListType& data,
              const OutputPixelType& L, OutputPixelType& GRAD)
  {

    ScalarType E = static_cast<ScalarType>(0.0);
    GRAD = OutputPixelType(static_cast<ScalarType>(0.0));

    ScalarType resi=0;
    unsigned int n = m_GradientList.size();
    OutputPixelType EL = L.Exp();
    ScalarType trace = static_cast<ScalarType>(0.0);    
    OutputPixelType Gi;
    OutputPixelType diffExp;

    SizeType size;    
    if( m_Initialization )
      size = m_Initialization->GetLargestPossibleRegion().GetSize();
                    
    switch( m_EstimationType )
    {

        case 0:

          for (unsigned int i=0;i<n;i++)
          {

            Gi = OutputPixelType (m_GradientList[i]);
            trace = EL.ScalarProductWith (Gi);            
            resi = log( data[0]/data[i+1] ) - trace;

            E += 0.5*resi*resi;

            diffExp = L.DifferentialExp (Gi);
            GRAD += diffExp*resi*(-1.0);

          }
          
          break;


        case 1:

          for (unsigned int i=0;i<n;i++)
          {
            Gi = OutputPixelType (m_GradientList[i]);
            trace = EL.ScalarProductWith (Gi);
            
            resi = data[i+1] - data[0] * exp( -1.0 * trace );

            E += 0.5*resi*resi;
                        

            diffExp = L.DifferentialExp (Gi);
            GRAD += diffExp * resi * data[0] * exp(-1.0*trace);
            
          }
          

          break;

        case 2:

          for (unsigned int i=0;i<n;i++)
          {
            Gi = OutputPixelType (m_GradientList[i]);
            trace = 2.0 * ( EL.ScalarProductWith (Gi) );

            
            resi = data[i+1]*data[i+1] - 2.0*m_Variances[i+1] - ( data[0]*data[0] - 2.0*m_Variances[0] )
              * exp( -1.0*trace );

            //E += 0.5*resi*resi;
            E += this->Huber (resi);

            // gradient
            diffExp = L.DifferentialExp (Gi);
            
            GRAD += diffExp * this->DerHuber (resi) * 2.0
              * ( data[0]*data[0] - 2.0*m_Variances[0] ) * exp(-1.0*trace);
            
          }

          break;


        case 3:
      
      for (unsigned int i=0;i<n;i++)
        {
          Gi = OutputPixelType (m_GradientList[i]);
          trace = EL.ScalarProductWith (Gi);
          
          resi = data[i+1] - m_Variances[i+1]/(2.0*data[i+1]) - ( data[0] -
                                                                  m_Variances[0]/( 2.0*data[0] ) )* exp( -1.0 * trace );
          
          E += this->Huber(resi);
          
          
          diffExp = L.DifferentialExp (Gi);
          GRAD += diffExp * this->DerHuber(resi) * ( data[0] - m_Variances[0]/( 2.0*data[0] ) )* exp(-1.0*trace);
          
        }
      
      break;
      

        case 4:

          for (unsigned int i=0;i<n;i++)
          {
            Gi = OutputPixelType (m_GradientList[i]);
            trace = EL.ScalarProductWith (Gi);

            ScalarType vari = m_Variances[i+1];            
            ScalarType OSi = data[i+1];
            //ScalarType Si = ( data[0] - m_Variances[0]/(2.0*data[0]) )*exp(-1.0 * trace);
            ScalarType Si = data[0] * exp(-1.0 * trace);


            E += -log ( OSi/vari ) + ( OSi*OSi + Si*Si )/(2.0*vari)
              - log( this->Bessel(OSi*Si/vari) );

            resi = Si/vari - this->BesselRatio(Si*OSi/vari) * OSi/vari;


            diffExp = L.DifferentialExp (Gi);
            GRAD -= diffExp * resi * Si;
            
          }
          

          break;
          

    default:
      
      throw itk::ExceptionObject(__FILE__,__LINE__,
                                 "Error: Estimation mode not recognized.");
    }
    
    
    E /= static_cast<ScalarType>(n);    
    GRAD /= static_cast<ScalarType>(n);
    
    return E;       
    
  }
  

  template<class TInputImage, class TOutputImage>
  typename DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>::ScalarType
  DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>
  ::Huber ( const ScalarType &res )
  {
    
    ScalarType h = 0.0;

    switch( m_MEstimatorType )
    {

        case 0:
          h = 0.5*res*res;
          break;

        case 1:
          if ( fabs (res) <= m_Kappa )
            h = 0.5*res*res;
          else
            h = m_Kappa*(fabs (res) - 0.5*m_Kappa);
          break;

        case 2:
          h = m_Kappa*m_Kappa*(fabs (res)/m_Kappa - log (1.0 + fabs (res)/m_Kappa));
          break;
          
          
        default:
          throw itk::ExceptionObject (__FILE__,__LINE__,"Error: MEstimator not recognized.");
    }
    
    return h;
  }

  
  template<class TInputImage, class TOutputImage>
  typename DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>::ScalarType
  DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>
  ::DerHuber ( const ScalarType &res )
  {
    ScalarType h = 0.0;

    switch(m_MEstimatorType)
    {

        case 0:
          h = res;
          break;

        case 1:
    
          if ( fabs (res) <= m_Kappa )
            h = res;
          else
            h = m_Kappa*( res / fabs (res) );
          break;

        case 2:
          h = res/(1 + fabs (res)/m_Kappa);
          break;
          
        default:
          throw itk::ExceptionObject (__FILE__,__LINE__,"Error: MEstimator not recognized");

    }
    
    return h;
    
  }


  template<class TInputImage, class TOutputImage>
  typename DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>::ScalarType
  DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>
  ::Bessel ( const ScalarType &x )
  {

    ScalarType I0 = 1.0;

    ScalarType v = 0.5*x;
    ScalarType v2 = v*v;
    
    ScalarType A0=1.0;
    
    ScalarType EPS=1e-12;
    
    unsigned int N=30;    
    for( unsigned int k=1; k<N; k++ )
    {
      A0 *= v2/(static_cast<ScalarType>(k*k));
      
      if( A0<EPS )
        break;

      I0 += A0;
    }

    return I0;
        
  }
  


  template<class TInputImage, class TOutputImage>
  typename DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>::ScalarType
  DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>
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
  
  
  
  template<class TInputImage, class TOutputImage>
  typename DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>::OutputPixelType
  DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>
  ::Optimizor(const OutputPixelType& L, const OutputPixelType& DIR,
              const DataListType& data)
  {    

    //std::cout << "LinearResearchWolfe()" << std::endl;    
    ScalarType dt = LinearResearchWolfe(L, DIR, data);
    //ScalarType dt = LinearResearch(L, DIR, data);
    //std::cout << dt << std::endl;
    
    //std::cout << "LinearResearchWolfe() Done." << std::endl;    

    return L + DIR*dt;
        
  }


  template<class TInputImage, class TOutputImage>
  typename DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>::ScalarType
  DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>
  ::LinearResearchWolfe(const OutputPixelType& L, const OutputPixelType& DIR,
                        const DataListType& data)
  {

    
    ScalarType t = m_Tinit;
    ScalarType EPS = 1e-12;

    OutputPixelType junk;
    ScalarType Et = Simulator (data, L+DIR*t, junk);
    
    while( /*isnan ( Et ) &*/ t>EPS )
    {      
      t *= 0.5;
      Et = Simulator (data, L+DIR*t, junk);
    }        
    
    ScalarType tg = 0.0;
    ScalarType td = 999999999.9;
    bool t_OK = false;

    
    int nite = 0;
      
    while( !t_OK )
    {

      if ( (td - tg) < EPS )
        t_OK=true;
      
      

      if ( this->IsTooBig (t, L, DIR, data) )
        td = t;
      else
      {
        if ( this->IsTooSmall(t, L, DIR, data) )
          tg = t;
        else
          t_OK = true;
      }


      if( !t_OK )
        t = this->ExterpInterp (tg,td);

      nite++;
      
    }
    

    return t;    
    
  }



  template<class TInputImage, class TOutputImage>
  typename DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>::ScalarType
  DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>
  ::LinearResearch(const OutputPixelType& L, const OutputPixelType& DIR,
                        const DataListType& data)
  {

    
    ScalarType t = 2.0*m_Tinit;
    ScalarType oldt = t;
    ScalarType EPS = 1e-12;
        
    OutputPixelType junk;
    ScalarType Et = Simulator (data, L+DIR*t, junk);
    ScalarType E = 2*Et+1;
    
    while( E >= Et /*|| isnan (Et)*/ )
    {
      oldt = t;
      t /= 2.0;

      if( fabs (oldt -t) < EPS )
        break;
      
      E = Et;
      Et = Simulator (data, L+DIR*t, junk);      
    }

    //std::cout << t << std::endl;
    
    
    return t;    
    
  }

  

  template<class TInputImage, class TOutputImage>
  bool
  DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>
  ::IsTooBig(const ScalarType& t, const OutputPixelType& L, const OutputPixelType &DIR,
             const DataListType& data)
  {
    bool res = false;

    OutputPixelType GRAD;    
    ScalarType Q = Simulator (data, L + DIR*t, GRAD);
    OutputPixelType GRAD0;
    ScalarType Q0 = Simulator (data, L, GRAD0);

    ScalarType Q0PRIME = GRAD0.ScalarProductWith (DIR);

    if ( Q > Q0 + 0.1*t*Q0PRIME )
      res = true;

    return res;    
    
  }


  template<class TInputImage, class TOutputImage>
  bool
  DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>
  ::IsTooSmall(const ScalarType& t, const OutputPixelType& L, const OutputPixelType &DIR,
               const DataListType& data)
  {

    bool res = false;

    OutputPixelType GRAD;    
    //ScalarType Q = Simulator (data, L + DIR*t, GRAD);
    this->Simulator (data, L + DIR*t, GRAD);
    OutputPixelType GRAD0;
    //ScalarType Q0 = Simulator (data, L, GRAD0);
    this->Simulator (data, L, GRAD0);

    ScalarType QPRIME = GRAD.ScalarProductWith (DIR);
    ScalarType Q0PRIME = GRAD0.ScalarProductWith (DIR);
    
    if( /*(Q <= Q0 + 0.1*t*Q0PRIME) &&*/ (QPRIME < 0.9*Q0PRIME) )
      res = true;

    return res;
    
  }


  template<class TInputImage, class TOutputImage>
  typename DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>::ScalarType
  DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>
  ::ExterpInterp(const ScalarType& tg, const ScalarType& td)
  {

    ScalarType t = 0.0;
    
    if ( td == 999999999.9 )
      t = 10*tg;
    else
      t = 0.5*( td + tg );

    return t;
    
  }
  
  
  template<class TInputImage, class TOutputImage>
  typename DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>::InternalMatrixType
  DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>
  ::BFGS(const OutputPixelType& Lt, const OutputPixelType& Ltt,
         const OutputPixelType& Gt, const OutputPixelType& Gtt,
         const InternalMatrixType& Wt)
  {

    OutputPixelType S = Ltt - Lt;
    OutputPixelType Y = Gtt - Gt;

    // W1 = s*y'
    InternalMatrixType W1 (6,6,0.0);
    // W2 = s*s'
    InternalMatrixType W2 (6,6,0.0);
    // W3 = y*y'
    InternalMatrixType W3 (6,6,0.0);
    
    for (unsigned int nl=0;nl<6;nl++)
    {
      for (unsigned int nc=0;nc<6;nc++)
      {
        W1(nl,nc) = S.GetNthComponentAsVector (nl) * Y.GetNthComponentAsVector (nc);
        W2(nl,nc) = S.GetNthComponentAsVector (nl) * S.GetNthComponentAsVector (nc);
        W3(nl,nc) = Y.GetNthComponentAsVector (nl) * Y.GetNthComponentAsVector (nc);
      }
    }


    // scalar product between W3 and Wt:
    ScalarType b = 0.0;
    
    for (unsigned int nl=0;nl<6;nl++)
    {
      for (unsigned int nc=0;nc<6;nc++)
        b += W3(nl,nc)*Wt(nl,nc);      
    }
        
    ScalarType a = S.ScalarProductWith (Y);

    InternalMatrixType Wtt = Wt - (W1*Wt + Wt*W1.transpose())/a + (1.0 + b/a)*W2/a;

    
    return Wtt;

  }


  
  template<class TInputImage, class TOutputImage>
  void
  DTIEstimatorWithBFGSTensorImageFilter<TInputImage,TOutputImage>
  ::DynamicThreadedGenerateData(const OutputImageRegionType &outputRegionForThread)
  {

    typedef ImageRegionIterator<OutputImageType>      IteratorOutputType;
    typedef ImageRegionConstIterator<InputImageType>  IteratorInputType;
    
    unsigned int n = this->GetNumberOfIndexedInputs();
    
    IteratorOutputType itOut(this->GetOutput(), outputRegionForThread);
    
    // create a list of iterators for each input
    std::vector<IteratorInputType> ListOfInputIterators;
    for(unsigned int i=0;i<n;i++)
    {
      IteratorInputType it(this->GetInput(i),outputRegionForThread);
      ListOfInputIterators.push_back(it);
    }
    
    
    while(!itOut.IsAtEnd())
    {

      OutputPixelType out( static_cast<ScalarType>( 0.0 ) );      
      InputPixelType b0 = ListOfInputIterators[0].Get();
      
      // threshold b0
      if(b0>m_BST)
      {

        unsigned int NMAX = 100;
        ScalarType EPS = 1e-6;
                        
        // fill up data
        DataListType data;
        for (unsigned int i=0;i<n;i++)
          data.push_back( ListOfInputIterators[i].Get() );


        /*
        if ( index[0]==0 && index[1]==41 && index[2]==5 )
        {
          for(unsigned int i=0;i<n;i++)
            std::cout << data[i] << " ";
          std::cout << std::endl;
          }*/
        

        
        // init:
        OutputPixelType Lt;        
        if( m_Initialization.IsNull() )
          Lt = OutputPixelType( static_cast<ScalarType>( 0.0 ) );
        else
          Lt = m_Initialization->GetPixel ( ListOfInputIterators[0].GetIndex() );

        InternalMatrixType Wt (6,6);
        Wt.set_identity();


        OutputPixelType GRADt;
        ScalarType Et = this->Simulator(data, Lt, GRADt);
        

	/*
        if ( isnan ( Et ) )
          Lt = OutputPixelType( static_cast<ScalarType>( 0.0 ) );
	*/
        
        unsigned int nite = 0;
        ScalarType Energy = 9999999;

        
        while( (nite<NMAX) && (Energy > EPS) )
        {


          VectorType D( 0.0 );
          OutputPixelType DIR;
          
          switch( m_GradientDescentType )
          {

              case 0:

                DIR = GRADt*(-1.0);
                
                break;


              case 1:
                
                for (unsigned int nl=0;nl<6;nl++)
                {
                  for (unsigned int nc=0;nc<6;nc++)
                    D[nl] += -Wt(nl,nc)*GRADt.GetNthComponentAsVector (nc);
                }
                
                for( unsigned int i=0;i<OutputPixelType::NDegreesOfFreedom; i++)
                  DIR.SetNthComponentAsVector(i, D[i]);
                
                break;
                
                
              default:
                throw itk::ExceptionObject (__FILE__,__LINE__,"Error: Gradient descent type not recognized.");
                
          }
          
          // Optimisation avec la nouvelle direction de descente:
          OutputPixelType Ltt = this->Optimizor(Lt, DIR, data);

          // nouveau gradient
          OutputPixelType GRADtt;
          ScalarType Ett = this->Simulator (data, Ltt, GRADtt);

          // nouvelle matrice BFGS
          Wt = this->BFGS(Lt,Ltt,GRADt,GRADtt,Wt);

          GRADt = GRADtt;

          //Energy = (Lt - Ltt).GetNorm();/*Et-Ett;*/ //GRADt.GetNorm();
          Energy = GRADt.GetNorm();
		  
/*
		  if ( isnan (Energy) )          
          {
            break;            
          }
  */        
          Lt = Ltt;
          Et = Ett;
   
          nite++;          
          
        }
        
        out = Lt;        
        
      }
      
      
      itOut.Set(out);
      ++itOut;
      for(unsigned int i=0;i<n;i++)
        ++(ListOfInputIterators[i]);
    }
    
    
  }
  

} // end of namespace

