#pragma once

#include "itkFiberTrackingImageFilter.h"

#include <itkContinuousIndex.h>
#include <itkZeroFluxNeumannBoundaryCondition.h>
#include <itkConstNeighborhoodIterator.h>
#include <itkImageRegionIterator.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkImageRegionConstIterator.h>
#include <itkImageRegionConstIteratorWithIndex.h>

namespace itk
{

template< class TInputImage, class TOutputImage>
FiberTrackingImageFilter< TInputImage, TOutputImage >
::FiberTrackingImageFilter()
{
    m_Sampling     = 1;
    m_Smoothness   = 0.0;
    m_MinLength    = 10.0;
    m_MaxLength    = 200.0;
    m_FAThreshold  = 0.3;
    m_FAThreshold2 = 0.01;
    m_TimeStep     = 1.0;
    m_OutputFiberSampling = 0.0;
    m_UseTriLinearInterpolation = true;
    m_AffineTransform = AffineTransformType::New();
    m_AffineTransform->SetIdentity();
    m_InternalAffineTransform = AffineTensorTransformType::New();
    m_Interpolator = NULL;
    m_IntegrationMethod = 0;
    m_SeedImage = 0;
    m_FibersSeededImage = 0;
    m_TransformTensorWithImageDirection = 0;
    m_TransformTensorWithPDD = 1;

    for( unsigned int i=0; i<ImageDimension; i++)
    {
        m_IntegrationStep[i] = 1.0;
    }

    this->SetNumberOfRequiredInputs(2);
}

template< class TInputImage, class TOutputImage>
void
FiberTrackingImageFilter< TInputImage, TOutputImage >
::SetLogTensorImage(const InputImageType *logImage)
{
    // Process object is not const-correct so the const casting is required.
    this->SetNthInput(1, const_cast<InputImageType *>( logImage ));
}

template< class TInputImage, class TOutputImage>
void
FiberTrackingImageFilter< TInputImage, TOutputImage >
::GenerateInputRequestedRegion()
{
    // call the superclass's implementation of this method
    Superclass::GenerateInputRequestedRegion();
    
    if ( !this->GetInput() )
    {
        return;
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
        typename InputImageType::ConstPointer logTensorImage
                = dynamic_cast<const InputImageType*>(ProcessObject::GetInput(1));
        typename LinearInterpolatorType::Pointer interpolator = LinearInterpolatorType::New();
        interpolator->SetInputImage ( logTensorImage );
        m_Interpolator = interpolator;

    }
    else
    {
        typename NNInterpolatorType::Pointer interpolator = NNInterpolatorType::New();
        interpolator->SetInputImage ( this->GetInput() );
        m_Interpolator = interpolator;
    }

    m_FibersSeededImage = ImageType::New();
    m_FibersSeededImage->SetRegions   (this->GetInput()->GetLargestPossibleRegion());
    m_FibersSeededImage->SetOrigin    (this->GetInput()->GetOrigin());
    m_FibersSeededImage->SetSpacing   (this->GetInput()->GetSpacing());
    m_FibersSeededImage->SetDirection (this->GetInput()->GetDirection());
    
    m_FibersSeededImage->Allocate();

    typename InputImageType::DirectionType direction ( this->GetInput()->GetDirection().GetInverse() );
    m_InternalAffineTransform->SetMatrix ( direction );
    
    // set the integration step to spacing/2 * m_TimeStep
    // std::cout<<"time steps : ";
    for( unsigned int i=0; i<ImageDimension; i++)
    {
        m_IntegrationStep[i] = this->GetInput()->GetSpacing()[0] * m_TimeStep / 2.0;
        // std::cout << m_IntegrationStep[i] << " ";
    }
    // std::cout << std::endl;
}

template< class TInputImage, class TOutputImage>
void
FiberTrackingImageFilter< TInputImage, TOutputImage >
::DynamicThreadedGenerateData(const OutputImageRegionType & outputRegion)
{
    typedef ZeroFluxNeumannBoundaryCondition<TOutputImage>                   BoundaryConditionType;
    typedef ConstNeighborhoodIterator<InputImageType,BoundaryConditionType>  NIType;
    typedef ImageRegionConstIteratorWithIndex<InputImageType>                InputIteratorType;
    typedef ImageRegionIteratorWithIndex<OutputImageType>                    IteratorType;
    typedef ImageRegionIteratorWithIndex<ImageType>                          ImageIteratorType;
    
    InputIteratorType itIn  (this->GetInput(),  outputRegion);
    IteratorType      itOut (this->GetOutput(), outputRegion);
    ImageIteratorType itFiberSeeds (m_FibersSeededImage, outputRegion);
    
    ImageIteratorType itSeed;
    if( !m_SeedImage.IsNull() )
    {
        itSeed = ImageIteratorType (m_SeedImage, outputRegion);
    }

    typename InputImageType::ConstPointer image = this->GetInput();
    
    long numFibers = 0;
    long numSeeds = 0;

    while(!itOut.IsAtEnd())
    {
        IndexType  index = itOut.GetIndex();
        ContinuousIndexType myCIndex;
        for( unsigned int i=0; i<ImageDimension; i++)
        {
            myCIndex[i] = static_cast<ScalarType>( index[i] );
        }

        InputPixelType T =  itIn.Get(); // image->GetPixel (index);

        /////////////////// CAUTION ///////////////////

        // The tensors are stored in image coordinates.
        // we have to transform them to global coordinates
        // in order to recover the true position/direction to
        // integrate from.

        /////////////////// CAUTION ///////////////////
        if (m_TransformTensorWithImageDirection)
        {
            if (m_TransformTensorWithPDD)
                T = m_InternalAffineTransform->TransformTensorWithPPD (T);
            else
                T = m_InternalAffineTransform->TransformTensorWithFS (T);
        }

        OutputPixelType Fiber;

        bool seed = true;

        if( !m_SeedImage.IsNull() )
        {
            seed = itSeed.Get()>0.0?false:true;
        }

        if( T.IsFinite() && !T.IsZero() && T.GetFA()>=this->GetFAThreshold() && seed )
        {
            numSeeds++;

            VectorType vstart = T.GetEigenvector(2);

            ContinuousIndexType cindex = myCIndex;

            this->GetFiberInDirection (cindex, vstart, Fiber, 1);
            vstart = vstart*(-1.0);

            cindex = myCIndex;

            OutputPixelType Fiber2;
            this->GetFiberInDirection (cindex, vstart, Fiber2, 0);
            Fiber.MergeWith ( Fiber2 );

            if( Fiber.GetLength() < this->GetMinLength() || Fiber.GetLength()>this->GetMaxLength() )
            {
                Fiber.Clear();
            }
            else
            {
                itFiberSeeds.Set ( static_cast<unsigned char>(255.0) );
                numFibers++;
            }
        }

        if( !seed )
        {
            itFiberSeeds.Set ( static_cast<unsigned char>(255.0) );
        }

        itOut.Set (Fiber);
        ++itIn;
        ++itOut;
        ++itFiberSeeds;

        if( !m_SeedImage.IsNull() )
        {
            ++itSeed;
        }

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
        }
    }
}


template< class TInputImage, class TOutputImage>
void
FiberTrackingImageFilter<TInputImage, TOutputImage>::
GetFiberInDirection (ContinuousIndexType cindex, VectorType vin, OutputPixelType &FiberForward, int firstPoint)
{
    typename InputImageType::ConstPointer image = this->GetInput();
    
    PointType pos;
    PointType prevPos;
    
    image->TransformContinuousIndexToPhysicalPoint (cindex, pos);
    prevPos = pos;
    
    if( firstPoint )
    {
        FiberPointType fPoint;
        fPoint.Point = m_AffineTransform->TransformPoint (pos);
        fPoint.Tensor = this->InterpolateTensorAt( cindex );
        FiberForward.AddPoint( fPoint );
    }

    InputPixelType T = this->InterpolateTensorAt ( cindex );

    if( T.IsZero() || T.GetFA()<m_FAThreshold2  )
    {
        return;
    }
    
    while ( 1 )
    {
        vin = this->GetOutputDirection ( vin, T );
        pos = this->IntegrateWithInitialConditions( pos, vin, T );

        if( !image->TransformPhysicalPointToContinuousIndex (pos, cindex) )
        {
            break; // we get out of the image
        }

        T = this->InterpolateTensorAt( cindex );

        if( T.IsZero() || T.GetFA()<m_FAThreshold2  )
        {
            break;
        }

        FiberPointType fPoint;
        if (prevPos.EuclideanDistanceTo (pos)>m_OutputFiberSampling)
        {
            fPoint.Point  = this->GetAffineTransform()->TransformPoint (pos);
            fPoint.Tensor = T;
            FiberForward.AddPoint( fPoint );

            if( FiberForward.GetLength() > this->GetMaxLength() )
            {
                break; // too long fibers, might result from a loop
            }
            prevPos = fPoint.Point;
        }
    }
}


template< class TInputImage, class TOutputImage>
typename TInputImage::PixelType
FiberTrackingImageFilter<TInputImage, TOutputImage>::
InterpolateTensorAt (const ContinuousIndexType& index_base) const
{
    TensorType T(static_cast<InputScalarType>(0.0));

    if ( !m_Interpolator->IsInsideBuffer ( index_base ))
        return T;

    T = m_Interpolator->EvaluateAtContinuousIndex(index_base);
    if( m_UseTriLinearInterpolation && !T.IsZero() )
    {
        T = T.Exp();
        if (!T.IsFinite())
        {
            T = static_cast<InputScalarType>(0.0);
        }
    }
    
    /////////////////// CAUTION ///////////////////
    
    // The tensors are stored in image coordinates.
    // we have to transform them to global coordinates
    // in order to recover the true position/direction to
    // integrate from.
    
    /////////////////// CAUTION ///////////////////

    if (m_TransformTensorWithImageDirection)
    {
        if (m_TransformTensorWithPDD)
            T = m_InternalAffineTransform->TransformTensorWithPPD (T);
        else
            T = m_InternalAffineTransform->TransformTensorWithFS (T);
    }

    return T;
}



template< class TInputImage, class TOutputImage>
typename FiberTrackingImageFilter<TInputImage, TOutputImage>::VectorType
FiberTrackingImageFilter<TInputImage, TOutputImage>::
GetOutputDirection (const VectorType& vin, const TensorType& T) const
{
    // does not check if T is null or not

    // get the major eigenvector of the tensor
    VectorType v1 = T.GetEigenvector (2);

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
    if ( vout.GetNorm()!=0.0 )
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
    switch( m_IntegrationMethod )
    {
        case 0: // first order Euler integration
            return this->FirstOrderEulerIntegration(pos, v, T);
            break;

        case 1: // second order Runge-Kutta method
            return this->SecondOrderRungeKuttaIntegration(pos, v, T);
            break;

        case 2: // fourth order Runge Kuttan method
            return this->FourthOrderRungeKuttaIntegration(pos, v, T);
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
SecondOrderRungeKuttaIntegration (const PointType& pos, const VectorType& v, const TensorType& T)
{
    return this->FirstOrderEulerIntegration (pos, v, T);
}

template< class TInputImage, class TOutputImage>
typename FiberTrackingImageFilter<TInputImage, TOutputImage>::PointType
FiberTrackingImageFilter<TInputImage, TOutputImage>::
FourthOrderRungeKuttaIntegration (const PointType& pos, const VectorType& v, const TensorType& junk)
{
    VectorType k1, k2, k3, k4;
    PointType p;
    TensorType T;
    ContinuousIndexType c;

    for( unsigned int i=0; i<ImageDimension; i++)
    {
        k1[i] = m_IntegrationStep[i]*v[i];
    }

    p = pos + k1 * 0.5;

    typename InputImageType::ConstPointer input = this->GetInput();
    
    if( !input->TransformPhysicalPointToContinuousIndex(p, c) )
    {
        return p; // will stop
    }
    
    T = this->InterpolateTensorAt( c );
    if( T.IsZero() )
    {
        return p;
    }
    
    VectorType v_temp = this->GetOutputDirection( k1, T );
    for( unsigned int i=0; i<ImageDimension; i++)
    {
        k2[i] = m_IntegrationStep[i]*v_temp[i];
    }

    p = pos + k2 * 0.5;
    if( !input->TransformPhysicalPointToContinuousIndex(p, c) )
    {
        return p; // will stop
    }
    
    T = this->InterpolateTensorAt( c );
    if( T.IsZero() )
    {
        return p;
    }

    v_temp = this->GetOutputDirection( k2, T );
    for( unsigned int i=0; i<ImageDimension; i++)
    {
        k3[i] = m_IntegrationStep[i]*v_temp[i];
    }

    p = pos + k3;
    if( !input->TransformPhysicalPointToContinuousIndex(p, c) )
    {
        return p; // will stop
    }
    
    T = this->InterpolateTensorAt( c );
    if( T.IsZero() )
    {
        return p;
    }

    v_temp = this->GetOutputDirection( k3, T );
    for( unsigned int i=0; i<ImageDimension; i++)
    {
        k4[i] = m_IntegrationStep[i]*v_temp[i];
    }

    return pos + (k1 + k2*2.0 + k3*2.0 + k4)/6.0;
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
    os << indent << "Transform tensors with image direction: "<<m_TransformTensorWithImageDirection << std::endl;
    os << indent << "Type of tensor transformation used (PPD:1 / FS:0) "<<m_TransformTensorWithPDD << std::endl;
    
    Superclass::PrintSelf(os,indent);
}


} // end of namespace itk
