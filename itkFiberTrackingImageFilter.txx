#ifndef _itk_FiberTrackingImageFilter_txx_
#define _itk_FiberTrackingImageFilter_txx_
#include "itkFiberTrackingImageFilter.h"

#include <itkContinuousIndex.h>
#include <itkZeroFluxNeumannBoundaryCondition.h>
#include <itkConstNeighborhoodIterator.h>
#include <itkImageRegionIterator.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkImageRegionConstIterator.h>
#include <itkImageRegionConstIteratorWithIndex.h>

#include "itkProgressReporter.h"

namespace itk
{
  
  template< class TInputImage, class TOutputImage>
  void
  FiberTrackingImageFilter< TInputImage, TOutputImage >
  ::GenerateInputRequestedRegion() throw (InvalidRequestedRegionError)
  {

    // call the superclass's implementation of this method
    Superclass::GenerateInputRequestedRegion();
    
    if ( !this->GetInput() )
    {
      return;
    }

    if( this->GetUseTriLinearInterpolation() && !m_LogTensorImage )
    {
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error: Tri-linear interpolation is on and no log tensor image is set.");      
    }
    
    
    // get pointers to the input and output
    typename InputImageType::Pointer  inputPtr  = 
      const_cast< TInputImage *>( this->GetInput() );
    
    // Request the entire input image
    typename InputImageType::RegionType inputRegion;
    inputRegion = inputPtr->GetLargestPossibleRegion();
    inputPtr->SetLargestPossibleRegion(inputRegion);
    inputPtr->SetRequestedRegion(inputRegion);
    
    return;

  }


  template< class TInputImage, class TOutputImage>
  void
  FiberTrackingImageFilter< TInputImage, TOutputImage >
  ::BeforeThreadedGenerateData()
  {

 
    if( this->GetUseTriLinearInterpolation() )
    {
      m_Interpolator = LinearInterpolatorType::New();
      //m_Interpolator->SetInputImage ( logFilter->GetOutput() );
      m_Interpolator->SetInputImage ( this->GetLogTensorImage() );
    }
    else
    {
      m_Interpolator = NNInterpolatorType::New();
      m_Interpolator->SetInputImage ( this->GetInput() );
    }

    m_FibersSeededImage = ImageType::New();
    m_FibersSeededImage->SetRegions (this->GetInput()->GetLargestPossibleRegion());
    m_FibersSeededImage->SetOrigin (this->GetInput()->GetOrigin());
    m_FibersSeededImage->SetSpacing (this->GetInput()->GetSpacing());
    m_FibersSeededImage->Allocate();


    // set the integration step to spacing/2 * m_TimeStep
    for( unsigned int i=0; i<ImageDimension; i++)
    {
      m_IntegrationStep[i] = this->GetInput()->GetSpacing()[0] * m_TimeStep / 2.0;
      std::cout << m_IntegrationStep[i] << " ";
    }
    std::cout << std::endl;
    
  }
  
  
  template< class TInputImage, class TOutputImage>
  void
  FiberTrackingImageFilter< TInputImage, TOutputImage >
  ::ThreadedGenerateData(const OutputImageRegionType & outputRegion,int threadId)
  {
      
    typedef ZeroFluxNeumannBoundaryCondition<TOutputImage>                   BoundaryConditionType;
    typedef ConstNeighborhoodIterator<InputImageType,BoundaryConditionType>  NIType;
    typedef ImageRegionConstIteratorWithIndex<InputImageType>                InputIteratorType;
    typedef ImageRegionIteratorWithIndex<OutputImageType>                    IteratorType;
    typedef ImageRegionIteratorWithIndex<ImageType>                          ImageIteratorType;
    

    //unsigned long numPixels = outputRegion.GetNumberOfPixels() / (unsigned long)( this->GetSampling() );
    //unsigned long step      = numPixels/1000;
    //unsigned long progress  = 0;

        
    InputIteratorType itIn (this->GetInput(),  this->GetInput()->GetLargestPossibleRegion());
    IteratorType      itOut(this->GetOutput(), outputRegion);
    ImageIteratorType itFiberSeeds (m_FibersSeededImage, outputRegion);
    
    ImageIteratorType itSeed;
    if( !m_SeedImage.IsNull() )
    {
      itSeed = ImageIteratorType (m_SeedImage, outputRegion);
    }
    
    typename InputImageType::ConstPointer image = this->GetInput();
    typename InputImageType::SizeType size = this->GetInput()->GetLargestPossibleRegion().GetSize();

    long numFibers = 0;
    long numSeeds = 0;

    ProgressReporter progressReporter (this, threadId, outputRegion.GetNumberOfPixels(), 1000);

    /*
      if(threadId==0)
      {
      this->UpdateProgress (0.0);
      }
    */

    while(!itOut.IsAtEnd())
    {
      
      /*
      if( this->GetAbortGenerateData() )
      {        
        throw itk::ProcessAborted(__FILE__,__LINE__);
      }
      */
      
      IndexType  index = itOut.GetIndex();
      ContinuousIndexType myCIndex;
      for( unsigned int i=0; i<ImageDimension; i++)
      {
	myCIndex[i] = static_cast<ScalarType>( index[i] );
	/*
	  if( (unsigned int)myCIndex[i] < size[i]-1 )
	  {
          myCIndex[i] = static_cast<ScalarType>( index[i] );// + 0.5;
	  }
	  else
	  {
	  myCIndex[i] = static_cast<ScalarType>( index[i] );
	  }
	*/
      }
      
      

      InputPixelType T =  image->GetPixel (index);

      
      OutputPixelType Fiber;
      
      bool seed = true;
      
      if( !m_SeedImage.IsNull() )
      {
        seed = itSeed.Get()>0.0?false:true;
      }
      
      if( T.IsFinite() && !T.IsZero() && T.GetFA()>=this->GetFAThreshold() && seed )
      {
        numSeeds++;
        
        VectorType vstart;        
        
        try
        {
          vstart = T.GetEigenvector(2);
        }
        catch (itk::ExceptionObject &e)
        {
          std::cerr << e;
          throw itk::ExceptionObject (__FILE__,__LINE__,"Error in FiberTrackingImageFilter::ThreadedGenerateData()");
        }

        ContinuousIndexType cindex = myCIndex;
        
        Fiber  = this->GetFiberInDirection (cindex, vstart, 1);
        vstart = vstart*(-1.0);
        
        cindex = myCIndex;
                
        OutputPixelType Fiber2 = this->GetFiberInDirection (cindex, vstart, 0);
        Fiber.MergeWith ( Fiber2 );
        
        if( Fiber.GetLength() < this->GetMinLength() || Fiber.GetLength()>this->GetMaxLength() )
        {
          Fiber.Clear();
        }
        else
        {
          itFiberSeeds.Set ( static_cast<ScalarType>(255.0) );
          numFibers++;
        }
      }

      if( !seed )
      {
        itFiberSeeds.Set ( static_cast<ScalarType>(255.0) );
      }


      progressReporter.CompletedPixel();

      
      itOut.Set (Fiber);
      ++itIn;
      ++itOut;
      //++progress;
      ++itFiberSeeds;

      if( !m_SeedImage.IsNull() )
      {
        ++itSeed;
      }

      /*
      if( threadId==0 )
      {
        if( step && (progress%step)==0 )
        {
          this->UpdateProgress ( (double)progress/double(numPixels) );
        }
      }
      */
      
      for( int i=0; i<this->GetSampling()-1; i++ )
      {
        if( itOut.IsAtEnd() )
        {
          break;
        }
        ++itIn;
        OutputPixelType NullFiber;
        itOut.Set (NullFiber);
        ++itOut;

	progressReporter.CompletedPixel();
      }
      
    }

    /*
    if( threadId==0 )
    {
      std::cout << "Total number of seeds: " << numSeeds << std::endl;
      std::cout << "Total number of non-null fibers: " << numFibers << std::endl;
    }
    */

    /*
    if( threadId==0 )
    {
      this->UpdateProgress (1.0);
    }
    */
    
  }


  template< class TInputImage, class TOutputImage>
  typename TOutputImage::PixelType
  FiberTrackingImageFilter<TInputImage, TOutputImage>::
  GetFiberInDirection (ContinuousIndexType cindex, VectorType vin, int firstPoint)
  {
    OutputPixelType FiberForward;

    typename InputImageType::ConstPointer image = this->GetInput();
    typename InputImageType::SizeType     dims  = image->GetLargestPossibleRegion().GetSize();
    PointType pos;
    
    image->TransformContinuousIndexToPhysicalPoint (cindex, pos);
    
    
    if( firstPoint )
    {
      FiberForward.AddPoint( this->GetAffineTransform()->TransformPoint (pos) );
    }

    while ( 1 )
    {
      InputPixelType T = this->InterpolateTensorAt ( cindex );
      
      if( T.IsZero() || T.GetFA()<m_FAThreshold2  )
      {
        break;
      }
      
      vin = this->GetOutputDirection (vin, T);
      pos = this->IntegrateWithInitialConditions( pos, vin, T );
      
      if( !image->TransformPhysicalPointToContinuousIndex (pos, cindex) )
      {
        break; // we get out of the image
      }
      
      FiberForward.AddPoint( this->GetAffineTransform()->TransformPoint (pos) );
      if( FiberForward.GetLength() > this->GetMaxLength() )
      {
        break; // too long fibers, might result from a loop
      }
      
    }

    return FiberForward;
    
  }
  

  /** This function does not check for boundary problems! */
  template< class TInputImage, class TOutputImage>
  typename TInputImage::PixelType
  FiberTrackingImageFilter<TInputImage, TOutputImage>::
  InterpolateTensorAt (const ContinuousIndexType& index_base)
  {

    TensorType T(static_cast<InputScalarType>(0.0));
    try
    {      
      T = m_Interpolator->EvaluateAtContinuousIndex(index_base);
      if( this->GetUseTriLinearInterpolation() && !T.IsZero() )
      {
        T = T.Exp();
	if (!T.IsFinite())
	{
	  T = static_cast<InputScalarType>(0.0);
	}
      }      
      return T;      
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject (__FILE__,__LINE__,"Error in tensor interpolation");
    }
    
  }



  template< class TInputImage, class TOutputImage>
  typename FiberTrackingImageFilter<TInputImage, TOutputImage>::VectorType
  FiberTrackingImageFilter<TInputImage, TOutputImage>::
  GetOutputDirection (const VectorType& vin, const TensorType& T)
  {
    // does not check if T is null or not

    // get the major eigenvector of the tensor
    VectorType v1;
    try
    {
      v1 = T.GetEigenvector (2);
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      throw itk::ExceptionObject (__FILE__,__LINE__,
                                  "Error in FiberTrackingImageFilter::ThreadedGenerateData()");
    }

    // in case: reorientate v1 to be in the same direction that vin:
    ScalarType way = v1*vin;
    if( way<0.0 )
    {
      v1 = v1*(-1.0);
    }

    // Diffusion term
    TensorType TT=T/T.GetEigenvalue (2);
    VectorType vout = TT*vin;

    //vout.Normalize();
    // linear coefficient
    double cl = T.GetCl();

    // new propagation vector
    vout = v1*cl + ( vin*( 1.0-m_Smoothness ) + vout*m_Smoothness ) * (1.0-cl);
    if ( !vout.GetNorm()==0.0 )
    {
      vout.Normalize();
    }

    return vout;
    
  }
  
  
  template< class TInputImage, class TOutputImage>
  typename FiberTrackingImageFilter<TInputImage, TOutputImage>::PointType
  FiberTrackingImageFilter<TInputImage, TOutputImage>::
  IntegrateWithInitialConditions (const PointType& pos, const VectorType& v, const TensorType& T)
  {
    switch( this->GetIntegrationMethod() )
    {
        case 0: // first order Euler integration
          return this->FirstOrderEulerIntegration(pos, v, T);
          break;
          
        case 1: // second order Runge-Kutta method
          return this->SecondOderRungeKuttaIntegration(pos, v, T);
          break;
          
        case 2: // fourth order Runge Kuttan method
          return this->FourthOderRungeKuttaIntegration(pos, v, T);
          break;
          
        default:
          throw ExceptionObject(__FILE__,__LINE__,"Error in FiberTrackingImageFilter::IntegrateWithInitialConditions(): Integration method not supported.");		
    }
  }
  


  template< class TInputImage, class TOutputImage>
  typename FiberTrackingImageFilter<TInputImage, TOutputImage>::PointType
  FiberTrackingImageFilter<TInputImage, TOutputImage>::
  FirstOrderEulerIntegration (const PointType& pos, const VectorType& v, const TensorType& T)
  {
    PointType res;
    for( unsigned int i=0; i<ImageDimension; i++)
    {
      res[i] = pos[i] + m_IntegrationStep[i]*v[i];
    }
    return res;
  }
  
  template< class TInputImage, class TOutputImage>
  typename FiberTrackingImageFilter<TInputImage, TOutputImage>::PointType
  FiberTrackingImageFilter<TInputImage, TOutputImage>::
  SecondOderRungeKuttaIntegration (const PointType& pos, const VectorType& v, const TensorType& T)
  {
    return pos;
  }
  
  template< class TInputImage, class TOutputImage>
  typename FiberTrackingImageFilter<TInputImage, TOutputImage>::PointType
  FiberTrackingImageFilter<TInputImage, TOutputImage>::
  FourthOderRungeKuttaIntegration (const PointType& pos, const VectorType& v, const TensorType& junk)
  {

    VectorType k1, k2, k3, k4;
    PointType p;
    TensorType T;
    ContinuousIndexType c;

    for( unsigned int i=0; i<ImageDimension; i++)
    {
      k1[i] = m_IntegrationStep[i]*v[i];
    }

    p = pos + 0.5*k1;
    
    bool isInside = this->GetInput()->TransformPhysicalPointToContinuousIndex(p, c);
    if( !isInside )
    {
      return p; // will stop
    }
    
    T = this->InterpolateTensorAt( c );
    if( T.IsZero() )
    {
      return p;
    }


    VectorType v_temp = this->GetOutputDirection( k1, T);
    for( unsigned int i=0; i<ImageDimension; i++)
    {
      k2[i] = m_IntegrationStep[i]*v_temp[i];
    }

    p = pos+0.5*k2;
    isInside = this->GetInput()->TransformPhysicalPointToContinuousIndex(p, c);
    if( !isInside )
    {
      return p; // will stop
    }
    T = this->InterpolateTensorAt( c );
    if( T.IsZero() )
    {
      return p;
    }

    v_temp = this->GetOutputDirection( k2, T);
    for( unsigned int i=0; i<ImageDimension; i++)
    {
      k3[i] = m_IntegrationStep[i]*v_temp[i];
    }

    p = pos + k3;
    isInside = this->GetInput()->TransformPhysicalPointToContinuousIndex(p, c);
    if( !isInside )
    {
      return p; // will stop
    }
    T = this->InterpolateTensorAt( c );
    if( T.IsZero() )
    {
      return p;
    }

    v_temp = this->GetOutputDirection( k3, T);
    for( unsigned int i=0; i<ImageDimension; i++)
    {
      k4[i] = m_IntegrationStep[i]*v_temp[i];
    }

    return pos+(k1+2.0*k2+2.0*k3+k4)/6.0;

  }
  

  template< class TInputImage, class TOutputImage>
  void 
  FiberTrackingImageFilter<TInputImage, TOutputImage>::
  PrintSelf ( std::ostream& os, Indent indent ) const
  {
    os << indent << "Smoothness: " << m_Smoothness << std::endl;
    os << indent << "Minimum length: " << m_MinLength << std::endl;
    os << indent << "Maximum length: " << m_MaxLength << std::endl;
    os << indent << "FA threshold: " << m_FAThreshold << std::endl;
    os << indent << "FA threshold 2: " << m_FAThreshold2 << std::endl;
    os << indent << "Time step: " << m_TimeStep << std::endl;
    os << indent << "Integration method: " << m_IntegrationMethod << std::endl;
    os << indent << "Use trilinear interpolation: " << m_UseTriLinearInterpolation << std::endl;
    os << indent << "Sampling: " << m_Sampling << std::endl;
    os << indent << "Affine transform: " << m_AffineTransform << std::endl;
    Superclass::PrintSelf(os,indent);
  }
  
  
} // end of namespace itk


#endif
