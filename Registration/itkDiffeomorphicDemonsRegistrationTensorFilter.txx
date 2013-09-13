#ifndef __itkDiffeomorphicDemonsRegistrationTensorFilter_txx
#define __itkDiffeomorphicDemonsRegistrationTensorFilter_txx

#include "itkDiffeomorphicDemonsRegistrationTensorFilter.h"
#include "itkDivideByConstantImageFilter.h"

#include "itkSmoothingRecursiveGaussianImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include <float.h>

namespace itk
{

/*
 * Default constructor
 */
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
DiffeomorphicDemonsRegistrationTensorFilter
<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::DiffeomorphicDemonsRegistrationTensorFilter()
{

  typename DemonsRegistrationFunctionType::Pointer drfp = DemonsRegistrationFunctionType::New();

  this->SetDifferenceFunction( static_cast<FiniteDifferenceFunctionType *>(
      drfp.GetPointer() ) );

  m_Multiplier = MultiplyByConstantType::New();
  m_Multiplier->InPlaceOn();

  m_Exponentiator = FieldExponentiatorType::New();

  m_Warper = VectorWarperType::New();
  FieldInterpolatorPointer VectorInterpolator = FieldInterpolatorType::New();
  m_Warper->SetInterpolator(VectorInterpolator);

  m_Adder = AdderType::New();
  m_Adder->InPlaceOn();
}


/** Checks whether the DifferenceFunction is of type
 * ESMDemonsRegistrationTensorFunction.*/
template < class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision >
typename DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage, TMovingImage,
TDisplacementField, TSolverPrecision>::DemonsRegistrationFunctionType*
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage, TMovingImage, TDisplacementField, TSolverPrecision>::DownCastDifferenceFunctionType()
{
  DemonsRegistrationFunctionType *drfp =
    dynamic_cast<DemonsRegistrationFunctionType *> (this->GetDifferenceFunction().GetPointer());

    if ( !drfp )
      {
        itkExceptionMacro      ( << "Could not cast difference function to " <<
            "ESMDemonsRegistrationTensorFunction" );
      }

    return drfp;
}


/** Checks whether the DifferenceFunction is of type
 * ESMDemonsRegistrationTensorFunction.*/
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
const typename DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::DemonsRegistrationFunctionType*
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::DownCastDifferenceFunctionType() const
{
  const DemonsRegistrationFunctionType *drfp =
    dynamic_cast<const DemonsRegistrationFunctionType *>(this->GetDifferenceFunction().GetPointer());

    if( !drfp )
      {
        itkExceptionMacro( << "Could not cast difference function to " <<
            "ESMDemonsRegistrationTensorFunction" );
      }

    return drfp;
}


/** Set the function state values before each iteration */
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
void
DiffeomorphicDemonsRegistrationTensorFilter
<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::InitializeIteration()
{
  // update variables in the equation object
  DemonsRegistrationFunctionType *f = this->DownCastDifferenceFunctionType();
  f->SetDisplacementField( this->GetDisplacementField() );

  // call the superclass  implementation ( initializes f )
  Superclass::InitializeIteration();
}


/** Get the metric value from the difference function */
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
double
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::GetMetric() const
{
  const DemonsRegistrationFunctionType *f = this->DownCastDifferenceFunctionType();
  return f->GetMetric();
}


/** Get the metric value from the difference function */
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
unsigned long
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::GetNumberOfPixelsProcessed() const
{
  const DemonsRegistrationFunctionType *f = this->DownCastDifferenceFunctionType();
  return f->GetNumberOfPixelsProcessed();
}


/** Get the threshold below which the absolute difference of
 * intensity yields a match. */
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
double
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::GetIntensityDifferenceThreshold() const
{
  const DemonsRegistrationFunctionType *f = this->DownCastDifferenceFunctionType();
  return f->GetIntensityDifferenceThreshold();
}


/** Set the threshold below which the absolute difference of
 * intensity yields a match. */
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
void
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::SetIntensityDifferenceThreshold(double threshold)
{
  DemonsRegistrationFunctionType *f = this->DownCastDifferenceFunctionType();
  f->SetIntensityDifferenceThreshold(threshold);
}


/** Get the maximum update step length. */
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
double
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::GetMaximumUpdateStepLength() const
{
  const DemonsRegistrationFunctionType *f = this->DownCastDifferenceFunctionType();
  return f->GetMaximumUpdateStepLength();
}


/** Set the maximum update step length. */
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
void
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::SetMaximumUpdateStepLength(double threshold)
{
  DemonsRegistrationFunctionType *f = this->DownCastDifferenceFunctionType();
  f->SetMaximumUpdateStepLength(threshold);
}


/** Get the metric value from the difference function. */
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
const double &
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::GetRMSChange() const
{
  const DemonsRegistrationFunctionType *f = this->DownCastDifferenceFunctionType();
  return f->GetRMSChange();
}


/** Get types of available image forces */
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
typename DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::GradientType
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::GetUseGradientType() const
{
  const DemonsRegistrationFunctionType *f = this->DownCastDifferenceFunctionType();
  return f->GetUseGradientType();
}


/** Set types of available image forces */
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
void
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::SetUseGradientType(GradientType gtype)
{
  DemonsRegistrationFunctionType *f = this->DownCastDifferenceFunctionType();
  f->SetUseGradientType(gtype);
}


/** Get types of rotation we use to optimize the registration wrt tensor
 * reorientation */
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
typename DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::RotationType
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::GetUseRotationType() const
{
  const DemonsRegistrationFunctionType *f = this->DownCastDifferenceFunctionType();
  return f->GetUseRotationType();
}


/** Set types of rotation we use to optimize the registration wrt tensor
 * reorientation */
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
void
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::SetUseRotationType(RotationType rtype)
{
  DemonsRegistrationFunctionType *f = this->DownCastDifferenceFunctionType();
  f->SetUseRotationType(rtype);
}

/** This method allocates storage in m_UpdateBuffer.*/
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
void
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::AllocateUpdateBuffer()
{
  // The update buffer looks just like the output.
  DisplacementFieldPointer output = this->GetOutput();
  DisplacementFieldPointer upbuf = this->GetUpdateBuffer();

  upbuf->SetLargestPossibleRegion(output->GetLargestPossibleRegion());
  upbuf->SetRequestedRegion(output->GetRequestedRegion());
  upbuf->SetBufferedRegion(output->GetBufferedRegion());
  upbuf->SetSpacing(output->GetSpacing());
  upbuf->SetOrigin(output->GetOrigin());
  upbuf->SetDirection(output->GetDirection());
  upbuf->Allocate();
}

/** Solve update if necessary and then apply it. */
template <class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision>
void
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TDisplacementField,TSolverPrecision>
::ApplyUpdate(TimeStepType dt)
{
  DemonsRegistrationFunctionType *drfp = this->DownCastDifferenceFunctionType();

  // need to solve update first if we're using any kind of rotation
  if (this->GetUseRotationType() != DemonsRegistrationFunctionType::NoRotation)
    {
      this->GetUpdateBuffer()->Modified();
      drfp->SolveUpdate(this->GetUpdateBuffer());
    }

  // If we smooth the update buffer before applying it, then the are
  // approximating a viscuous problem as opposed to an elastic problem
  if ( this->GetSmoothUpdateField() )
    {
      this->SmoothUpdateField();
    }

  // use time step if necessary
  if ( fabs(dt - 1.0) > 1.0e-4 )
    {
      std::cout<<"Using timestep: "<<dt<<std::endl;
      m_Multiplier->SetConstant( dt );
      m_Multiplier->SetInput( this->GetUpdateBuffer() );
      m_Multiplier->GraftOutput( this->GetUpdateBuffer() );
      m_Multiplier->Update();

      // graft output back to this->GetUpdateBuffer()
      this->GetUpdateBuffer()->Graft( m_Multiplier->GetOutput() );
    }

  // compute the exponential
  m_Exponentiator->SetInput( this->GetUpdateBuffer() );
  const double imposedMaxUpStep = this->GetMaximumUpdateStepLength();
  if ( imposedMaxUpStep > 0.0 )
    {
      // max(norm(Phi))/2^N < 0.25*pixelspacing
      const double numiterfloat = 2.0 + vcl_log(imposedMaxUpStep)/vnl_math::ln2;
      unsigned int numiter = 0;
      if ( numiterfloat > 0.0 )
        {
          numiter = static_cast<unsigned int>( 1.0 + numiterfloat );
        }

      m_Exponentiator->AutomaticNumberOfIterationsOff();
      m_Exponentiator->SetMaximumNumberOfIterations( numiter );
    }
  else
    {
      m_Exponentiator->AutomaticNumberOfIterationsOn();
      m_Exponentiator->SetMaximumNumberOfIterations( 2000u ); // just a high value
    }

  // compose the vector fields
  m_Warper->SetOutputSpacing( this->GetUpdateBuffer()->GetSpacing() );
  m_Warper->SetOutputOrigin( this->GetUpdateBuffer()->GetOrigin() );
  m_Warper->SetOutputDirection( this->GetUpdateBuffer()->GetDirection());

  m_Warper->SetInput( this->GetOutput() );
  m_Warper->SetDisplacementField( m_Exponentiator->GetOutput() );

  m_Adder->SetInput1( m_Warper->GetOutput() );
  m_Adder->SetInput2( m_Exponentiator->GetOutput() );

  m_Adder->GetOutput()->SetRequestedRegion( this->GetOutput()->GetRequestedRegion() );
  m_Adder->Update();

  // Region passing stuff
  this->GraftOutput( m_Adder->GetOutput() );

  this->SetRMSChange( drfp->GetRMSChange() );

  // Smooth the deformation field
  if ( this->GetSmoothDisplacementField() )
    {
      this->SmoothDisplacementField();
    }
}


template < class TFixedImage, class TMovingImage, class TDisplacementField, class TSolverPrecision >
void
DiffeomorphicDemonsRegistrationTensorFilter<TFixedImage, TMovingImage, TDisplacementField, TSolverPrecision>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Intensity difference threshold: "
  << this->GetIntensityDifferenceThreshold() << std::endl;
}

} // end namespace itk

#endif
