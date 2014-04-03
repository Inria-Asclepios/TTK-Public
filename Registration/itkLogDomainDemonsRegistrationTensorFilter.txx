#ifndef __itkLogDomainDemonsRegistrationTensorFilter_txx
#define __itkLogDomainDemonsRegistrationTensorFilter_txx

#include "itkLogDomainDemonsRegistrationTensorFilter.h"

namespace itk {

/** Default constructor. */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::LogDomainDemonsRegistrationTensorFilter()
{
	DemonsRegistrationFunctionPointer drfp = DemonsRegistrationFunctionType::New();

	this->SetDifferenceFunction( static_cast<FiniteDifferenceFunctionType *>(
			drfp.GetPointer() ) );

	m_Multiplier = MultiplyByConstantType::New();
	m_Multiplier->InPlaceOn();

	// bch handles everything (no need for exp, warping, adding, etc)
	m_BCHFilter = BCHFilterType::New();
	m_BCHFilter->InPlaceOn();

	// Set number of terms in the BCH approximation to default value
	m_BCHFilter->SetNumberOfApproximationTerms( 2 );
}


/** Checks whether the DifferenceFunction is of type
 * DemonsRegistrationFunction. */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
typename LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::DemonsRegistrationFunctionType*
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::DownCastDifferenceFunctionType()
{
	DemonsRegistrationFunctionType *drfp =
		dynamic_cast<DemonsRegistrationFunctionType *>(this->GetDifferenceFunction().GetPointer());

		if( !drfp )
		{
			itkExceptionMacro( << "Could not cast difference function to ESMDemonsRegistrationFunctionTensor" );
		}

		return drfp;
}


/** Checks whether the DifferenceFunction is of type
 *  DemonsRegistrationFunction. */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
const typename LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::DemonsRegistrationFunctionType*
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::DownCastDifferenceFunctionType() const
{
	const DemonsRegistrationFunctionType *drfp =
		dynamic_cast<const DemonsRegistrationFunctionType *>(this->GetDifferenceFunction().GetPointer());

		if( !drfp )
		{
			itkExceptionMacro( << "Could not cast difference function to ESMDemonsRegistrationFunctionTensor" );
		}

		return drfp;
}


/** Set the function state values before each iteration. */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::InitializeIteration()
{
	// update variables in the equation object
	DemonsRegistrationFunctionType *f = this->DownCastDifferenceFunctionType();
	f->SetDisplacementField( this->GetDisplacementField() );

	// call the superclass  implementation ( initializes f )
	Superclass::InitializeIteration();
}


/** Get the metric value from the difference function. */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
double
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetMetric() const
{
	const DemonsRegistrationFunctionType *drfp = this->DownCastDifferenceFunctionType();
	return drfp->GetMetric();
}


/** Get the number of pixels processed by the filter. */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
unsigned long
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetNumberOfPixelsProcessed() const
{
	const DemonsRegistrationFunctionType *drfp = this->DownCastDifferenceFunctionType();
	return drfp->GetNumberOfPixelsProcessed();
}


/** Get Intensity Difference Threshold. */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
double
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetIntensityDifferenceThreshold() const
{
	const DemonsRegistrationFunctionType *drfp = this->DownCastDifferenceFunctionType();
	return drfp->GetIntensityDifferenceThreshold();
}


/** Set Intensity Difference Threshold. */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::SetIntensityDifferenceThreshold(double threshold)
{
	DemonsRegistrationFunctionType *drfp = this->DownCastDifferenceFunctionType();
	drfp->SetIntensityDifferenceThreshold(threshold);
}


/** Set Maximum Update Step Length. */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::SetMaximumUpdateStepLength(double step)
{
	DemonsRegistrationFunctionType *drfp = this->DownCastDifferenceFunctionType();
	drfp->SetMaximumUpdateStepLength(step);
}


/** Get Maximum Update Step Length. */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
double
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetMaximumUpdateStepLength() const
{
	const DemonsRegistrationFunctionType *drfp = this->DownCastDifferenceFunctionType();
	return drfp->GetMaximumUpdateStepLength();
}


/** Set number of terms used in the BCH approximation. */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::SetNumberOfBCHApproximationTerms(unsigned int numterms)
{
	this->m_BCHFilter->SetNumberOfApproximationTerms(numterms);
}


// Get number of terms used in the BCH approximation
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
unsigned int
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetNumberOfBCHApproximationTerms() const
{
	return this->m_BCHFilter->GetNumberOfApproximationTerms();
}


/** Get the metric value from the difference function */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
const double &
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetRMSChange() const
{
	const DemonsRegistrationFunctionType *drfp = this->DownCastDifferenceFunctionType();
	return drfp->GetRMSChange();
}


/** Get gradient type */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
typename LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>::GradientType
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetUseGradientType() const
{
	const DemonsRegistrationFunctionType *drfp = this->DownCastDifferenceFunctionType();
	return drfp->GetUseGradientType();
}

/** Set gradient type */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::SetUseGradientType(GradientType gtype)
{
	DemonsRegistrationFunctionType *drfp = this->DownCastDifferenceFunctionType();
	drfp->SetUseGradientType(gtype);
}


/** Get types of rotation we use to optimize the registration wrt tensor
 * reorientation */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
typename LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::RotationType
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetUseRotationType() const
{
	const DemonsRegistrationFunctionType *drfp = this->DownCastDifferenceFunctionType();
	return drfp->GetUseRotationType();
}


/** Set types of rotation we use to optimize the registration wrt tensor
 * reorientation */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::SetUseRotationType(RotationType gtype)
{
	DemonsRegistrationFunctionType *drfp = this->DownCastDifferenceFunctionType();
	drfp->SetUseRotationType(gtype);
}


/** This method allocates storage in m_UpdateBuffer.*/
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
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
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::ApplyUpdate(const TimeStepType &dt)
{

  // need to solve update first if we're using any kind of rotation
	if (this->GetUseRotationType() != DemonsRegistrationFunctionType::NoRotation )
	  {
	    this->GetUpdateBuffer()->Modified();
	    DemonsRegistrationFunctionType *drfp = this->DownCastDifferenceFunctionType();
	    drfp->SolveUpdate(this->GetUpdateBuffer());
	  }

	// If we smooth the update buffer before applying it, then the are
	// approximating a viscuous problem as opposed to an elastic problem
	if ( this->GetSmoothUpdateField() )
	{
		this->SmoothUpdateField();
	}

	// Use time step if necessary. In many cases
	// the time step is one so this will be skipped
	if ( fabs(dt - 1.0)>1.0e-4 )
	{
		itkDebugMacro( "Using timestep: " << dt );
		m_Multiplier->SetConstant( dt );
		m_Multiplier->SetInput( this->GetUpdateBuffer() );
		m_Multiplier->GraftOutput( this->GetUpdateBuffer() );
		// in place update
		m_Multiplier->Update();
		// graft output back to this->GetUpdateBuffer()
		this->GetUpdateBuffer()->Graft( m_Multiplier->GetOutput() );
	}

	// Apply update by using BCH approximation
	m_BCHFilter->SetInput( 0, this->GetOutput() );
	m_BCHFilter->SetInput( 1, this->GetUpdateBuffer() );
	if ( m_BCHFilter->GetInPlace() )
	{
		m_BCHFilter->GraftOutput( this->GetOutput() );
	}
	else
	{
		// Work-around for http://www.itk.org/Bug/view.php?id=8672
		m_BCHFilter->GraftOutput( DisplacementFieldType::New() );
	}
	m_BCHFilter->GetOutput()->SetRequestedRegion( this->GetOutput()->GetRequestedRegion() );

	// Triggers in place update
	m_BCHFilter->Update();

	// Region passing stuff
	this->GraftOutput( m_BCHFilter->GetOutput() );

	// Smooth the velocity field
	if( this->GetSmoothVelocityField() )
	{
		this->SmoothVelocityField();
	}
}


template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
LogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::PrintSelf(std::ostream& os, Indent indent) const
{
	Superclass::PrintSelf( os, indent );

	os << indent << "Multiplier: " << m_Multiplier << std::endl;
	os << indent << "BCHFilter: " << m_BCHFilter << std::endl;
}


} // end namespace itk

#endif
