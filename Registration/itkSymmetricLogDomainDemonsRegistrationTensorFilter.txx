#ifndef __itkSymmetricLogDomainDemonsRegistrationTensorFilter_txx
#define __itkSymmetricLogDomainDemonsRegistrationTensorFilter_txx

#include "itkSymmetricLogDomainDemonsRegistrationTensorFilter.h"

#include "itkOppositeImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkImageDuplicator.h"
#include "itkVelocityFieldBCHCompositionFilter.h"

namespace itk {

// Default constructor
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::SymmetricLogDomainDemonsRegistrationTensorFilter()
{
	DemonsRegistrationFunctionPointer drfpf = DemonsRegistrationFunctionType::New();
	this->SetDifferenceFunction( static_cast<FiniteDifferenceFunctionType *>(
			drfpf.GetPointer() ) );

	m_Multiplier = MultiplyByConstantType::New();
	m_Multiplier->InPlaceOn();

	m_Adder = AdderType::New();
	m_Adder->InPlaceOn();

	// Set number of terms in the BCH approximation to default value
	m_NumberOfBCHApproximationTerms = 2;
	m_BackwardUpdateBuffer = 0;

	// initialize the average metrics here
	m_RMSChange = 0;
	m_NumberOfPixelsProcessed = 0;
	m_Metric = 0;

	// only use one thread due to changes in threaded calculate change
	this->SetNumberOfThreads(1);
}


// Checks whether the DifferenceFunction is of type DemonsRegistrationFunction.
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
typename SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::DemonsRegistrationFunctionType*
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetForwardRegistrationFunctionType()
{
	DemonsRegistrationFunctionType *drfp =
		dynamic_cast<DemonsRegistrationFunctionType *>(this->GetDifferenceFunction().GetPointer());

		if( !drfp )
		{
			itkExceptionMacro( << "Could not cast difference function to SymmetricDemonsRegistrationFunction" );
		}

		return drfp;
}


// Checks whether the DifferenceFunction is of type DemonsRegistrationFunction.
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
const typename SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::DemonsRegistrationFunctionType*
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetForwardRegistrationFunctionType() const
{
	const DemonsRegistrationFunctionType *drfp =
		dynamic_cast<const DemonsRegistrationFunctionType *>(this->GetDifferenceFunction().GetPointer());

		if( !drfp )
		{
			itkExceptionMacro( << "Could not cast difference function to SymmetricDemonsRegistrationFunction" );
		}

		return drfp;
}


// Set the function state values before each iteration
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::InitializeIteration()
{
	// update variables in the equation object
	DemonsRegistrationFunctionType *f = this->GetForwardRegistrationFunctionType();
	f->SetDisplacementField( this->GetDisplacementField() );

	// call the superclass  implementation ( initializes f )
	Superclass::InitializeIteration();
}


// Initialize flags
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::Initialize()
{
	//std::cout<<"SymmetricLogDomainDemonsRegistrationTensorFilter::Initialize"<<std::endl;
	this->Superclass::Initialize();

	const FixedImageType * fixim = this->GetFixedImage();
	const MovingImageType * movim = this->GetMovingImage();

	if ( fixim==0 || movim==0 )
	{
		itkExceptionMacro( << "A fixed and a moving image are required" );
	}

	if ( fixim->GetLargestPossibleRegion() != movim->GetLargestPossibleRegion() )
	{
		itkExceptionMacro( << "Registering images that have diffent sizes is not supported yet." );
	}

	if ( (fixim->GetSpacing() - movim->GetSpacing()).GetNorm() > 1e-10 )
	{
		itkExceptionMacro( << "Registering images that have diffent spacing is not supported yet." );
	}

	if ( (fixim->GetOrigin() - movim->GetOrigin()).GetNorm() > 1e-10 )
	{
		itkExceptionMacro( << "Registering images that have diffent origins is not supported yet." );
	}
}


// Get the metric value from the difference function
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
double
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetMetric() const
{
	//	const DemonsRegistrationFunctionType *drfpf = this->GetForwardRegistrationFunctionType();
	//	return drfpf->GetMetric();
	return m_Metric;
}


/*
 * Get the number of pixels processed by the filter. I guess we take the averaged
 * (as this is used for normalizing stats and metrics?).
 */
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
unsigned long
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetNumberOfPixelsProcessed() const
{
	//	const DemonsRegistrationFunctionType *drfpf = this->GetForwardRegistrationFunctionType();
	//	return drfpf->GetNumberOfPixelsProcessed();
	return m_NumberOfPixelsProcessed;
}


// Get Intensity Difference Threshold
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
double
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetIntensityDifferenceThreshold() const
{
	const DemonsRegistrationFunctionType *drfpf = this->GetForwardRegistrationFunctionType();
	return drfpf->GetIntensityDifferenceThreshold();
}


// Set Intensity Difference Threshold
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::SetIntensityDifferenceThreshold(double threshold)
{
	DemonsRegistrationFunctionType *drfpf = this->GetForwardRegistrationFunctionType();
	drfpf->SetIntensityDifferenceThreshold(threshold);
}


// Set Maximum Update Step Length
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::SetMaximumUpdateStepLength(double step)
{
	DemonsRegistrationFunctionType *drfpf = this->GetForwardRegistrationFunctionType();
	drfpf->SetMaximumUpdateStepLength(step);
}


// Get Maximum Update Step Length
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
double
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetMaximumUpdateStepLength() const
{
	const DemonsRegistrationFunctionType *drfpf = this->GetForwardRegistrationFunctionType();
	return drfpf->GetMaximumUpdateStepLength();
}


// Get the metric value from the difference function
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
const double &
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetRMSChange() const
{
	//	const DemonsRegistrationFunctionType *drfpf = this->GetForwardRegistrationFunctionType();
	//	return drfpf->GetRMSChange();
	return m_RMSChange;
}


// Get gradient type
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
typename SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>::GradientType
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetUseGradientType() const
{
	const DemonsRegistrationFunctionType *drfpf = this->GetForwardRegistrationFunctionType();
	return drfpf->GetUseGradientType();
}


// Set gradient type
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::SetUseGradientType(GradientType gtype)
{
	DemonsRegistrationFunctionType *drfpf = this->GetForwardRegistrationFunctionType();
	drfpf->SetUseGradientType(gtype);
}

// Get rotation type
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
typename SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>::RotationType
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::GetUseRotationType() const
{
	const DemonsRegistrationFunctionType *drfpf = this->GetForwardRegistrationFunctionType();
	return drfpf->GetUseRotationType();
}


// Set rotation type
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::SetUseRotationType(RotationType rtype)
{
	DemonsRegistrationFunctionType *drfpf = this->GetForwardRegistrationFunctionType();
	drfpf->SetUseRotationType(rtype);
}


// Allocate storage in m_UpdateBuffer
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
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

	this->AllocateBackwardUpdateBuffer();
}


// Allocates storage in m_BackwardUpdateBuffer
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::AllocateBackwardUpdateBuffer()
{
	// The backward update buffer looks just like the output.
	VelocityFieldPointer output = this->GetOutput();

	if ( !m_BackwardUpdateBuffer )
		m_BackwardUpdateBuffer = VelocityFieldType::New();
	m_BackwardUpdateBuffer->SetOrigin(output->GetOrigin());
	m_BackwardUpdateBuffer->SetSpacing(output->GetSpacing());
	m_BackwardUpdateBuffer->SetDirection(output->GetDirection());
	m_BackwardUpdateBuffer->SetLargestPossibleRegion(output->GetLargestPossibleRegion());
	m_BackwardUpdateBuffer->SetRequestedRegion(output->GetRequestedRegion());
	m_BackwardUpdateBuffer->SetBufferedRegion(output->GetBufferedRegion());
	m_BackwardUpdateBuffer->Allocate();
}


// Smooth the backward update field using a separable Gaussian kernel
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::SmoothBackwardUpdateField()
{
	// The update buffer will be overwritten with new data.
	this->SmoothGivenField(this->GetBackwardUpdateBuffer(), this->GetUpdateFieldStandardDeviations());
}


// This iterates through the overlap region and computes the update
// (or contribution to the system) at each voxel
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
typename
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>::TimeStepType
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::ThreadedCalculateChange(const ThreadRegionType &regionToProcess, int)
{
	typedef typename VelocityFieldType::RegionType      RegionType;
	typedef typename VelocityFieldType::SizeType        SizeType;
	typedef typename VelocityFieldType::SizeValueType   SizeValueType;
	typedef typename VelocityFieldType::IndexType       IndexType;
	typedef typename VelocityFieldType::IndexValueType  IndexValueType;
	typedef typename FiniteDifferenceFunctionType::NeighborhoodType
	NeighborhoodIteratorType;
	typedef ImageRegionIterator<VelocityFieldType>      UpdateIteratorType;

	VelocityFieldPointer output = this->GetOutput();

	// COMPUTE FORWARDS SLE

	// Get the FiniteDifferenceFunction to use in calculations.
	typename FiniteDifferenceFunctionType::Pointer dff = this->GetDifferenceFunction();

	const SizeType  radius = dff->GetRadius();

	// Break the input into a series of regions.  The first region is free
	// of boundary conditions, the rest with boundary conditions.  We operate
	// on the output region because input has been copied to output.
	typedef NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<VelocityFieldType>
	FaceCalculatorType;

	typedef typename FaceCalculatorType::FaceListType FaceListType;

	FaceCalculatorType faceCalculator;

	FaceListType faceList = faceCalculator(output, regionToProcess, radius);
	typename FaceListType::iterator fIt = faceList.begin();

	// Ask the function object for a pointer to a data structure it
	// will use to manage any global values it needs.  We'll pass this
	// back to the function object at each calculation and then
	// again so that the function object can use it to determine a
	// time step for this iteration.
	void *globalDataf = dff->GetGlobalDataPointer();

	// Process the non-boundary region.
	NeighborhoodIteratorType nD(radius, output, *fIt);
	UpdateIteratorType       nUF(this->GetUpdateBuffer(),  *fIt);
	while( !nD.IsAtEnd() )
	{
		nUF.Value() = dff->ComputeUpdate(nD, globalDataf);
		++nD;
		++nUF;
	}

	// Process each of the boundary faces.
	NeighborhoodIteratorType bD;
	UpdateIteratorType   bUF;
	for (++fIt; fIt != faceList.end(); ++fIt)
	{
		bD = NeighborhoodIteratorType(radius, output, *fIt);
		bUF = UpdateIteratorType  (this->GetUpdateBuffer(), *fIt);
		while ( !bD.IsAtEnd() )
		{
			bUF.Value() = dff->ComputeUpdate(bD, globalDataf);

			++bD;
			++bUF;
		}
	}

	TimeStepType forwTimeStep = dff->ComputeGlobalTimeStep(globalDataf);
	dff->ReleaseGlobalDataPointer(globalDataf);

	// SOLVE THE FORWARDS SLE
	DemonsRegistrationFunctionType *drfpf = this->GetForwardRegistrationFunctionType();

	if (this->GetUseRotationType() != DemonsRegistrationFunctionType::NoRotation )
	  {
	    this->GetUpdateBuffer()->Modified();
	    drfpf->SolveUpdate(this->GetUpdateBuffer());
	  }

	double forwRMSChange = drfpf->GetRMSChange();
	double forwMetric = drfpf->GetMetric();
	unsigned long forwNumOfPixels = drfpf->GetNumberOfPixelsProcessed();

	// COMPUTE BACKWARDS SLE
	globalDataf = dff->GetGlobalDataPointer();
	drfpf->SetFixedImage( this->GetMovingImage() );
	drfpf->SetMovingImage( this->GetFixedImage() );
	drfpf->SetDisplacementField( this->GetInverseDisplacementField() );
	drfpf->InitializeIteration();

	// Process the non-boundary region.
	fIt = faceList.begin();
	nD = NeighborhoodIteratorType(radius, output, *fIt);
	UpdateIteratorType       nUB(this->GetBackwardUpdateBuffer(),  *fIt);
	while( !nD.IsAtEnd() )
	{
		nUB.Value() = dff->ComputeUpdate(nD, globalDataf);
		++nD;
		++nUB;
	}

	// Process each of the boundary faces.
	UpdateIteratorType   bUB;
	for (++fIt; fIt != faceList.end(); ++fIt)
	{
		bD = NeighborhoodIteratorType(radius, output, *fIt);
		bUB = UpdateIteratorType  (this->GetBackwardUpdateBuffer(), *fIt);
		while ( !bD.IsAtEnd() )
		{
			bUB.Value() = dff->ComputeUpdate(bD, globalDataf);

			++bD;
			++bUB;
		}
	}

	// Ask the finite difference function to compute the time step for
	// this iteration.  We give it the global data pointer to use, then
	// ask it to free the global data memory.
	TimeStepType backTimeStep = dff->ComputeGlobalTimeStep(globalDataf);
	dff->ReleaseGlobalDataPointer(globalDataf);

	// SOLVE THE BACKWARDS SLE
	if (this->GetUseRotationType() != DemonsRegistrationFunctionType::NoRotation )
	  {
	    this->GetBackwardUpdateBuffer()->Modified();
	    drfpf->SolveUpdate(this->GetBackwardUpdateBuffer());
	  }

	double backRMSChange = drfpf->GetRMSChange();
	double backMetric = drfpf->GetMetric();
	unsigned long backNumOfPixels = drfpf->GetNumberOfPixelsProcessed();

	// average the metrics from the forwards and backwards updates
	this->SetRMSChange( 0.5*( forwRMSChange + backRMSChange ));
	this->SetMetric(0.5*( forwMetric + backMetric ));
	this->SetNumberOfPixelsProcessed(( forwNumOfPixels + backNumOfPixels )/2);

	return 0.5*(forwTimeStep + backTimeStep);
}


// Get the metric value from the difference function
template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::ApplyUpdate(TimeStepType dt)
{
	// If we smooth the update buffer before applying it, then the are
	// approximating a viscuous problem as opposed to an elastic problem
	if ( this->GetSmoothUpdateField() )
	{
		this->SmoothUpdateField();
		this->SmoothBackwardUpdateField();
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

		m_Multiplier->SetInput( this->GetBackwardUpdateBuffer() );
		m_Multiplier->GraftOutput( this->GetBackwardUpdateBuffer() );
		// in place update
		m_Multiplier->Update();
		// graft output back to this->GetUpdateBuffer()
		this->GetBackwardUpdateBuffer()->Graft( m_Multiplier->GetOutput() );
	}

	// Apply update (declare the filters here as efficiency is not critical
	// with "high" order BCH approximations)
	typedef VelocityFieldBCHCompositionFilter<VelocityFieldType, VelocityFieldType>   BCHFilterType;

	typename BCHFilterType::Pointer bchfilter = BCHFilterType::New();
	bchfilter->SetNumberOfApproximationTerms( this->m_NumberOfBCHApproximationTerms );

	// First get Z( v, K_fluid * u_forward )
	bchfilter->SetInput( 0, this->GetOutput() );
	bchfilter->SetInput( 1, this->GetUpdateBuffer() );

	bchfilter->GetOutput()->SetRequestedRegion( this->GetOutput()->GetRequestedRegion() );
	bchfilter->Update();
	VelocityFieldPointer Zf = bchfilter->GetOutput();
	Zf->DisconnectPipeline();

	// Now get Z( -v, K_fluid * u_backward )
	typedef OppositeImageFilter<
	VelocityFieldType, VelocityFieldType>  OppositeFilterType;

	typename OppositeFilterType::Pointer oppositefilter = OppositeFilterType::New();
	oppositefilter->SetInput( this->GetOutput() );
	oppositefilter->InPlaceOn();

	bchfilter->SetInput( 0, oppositefilter->GetOutput() );
	bchfilter->SetInput( 1, this->GetBackwardUpdateBuffer() );

	bchfilter->GetOutput()->SetRequestedRegion( this->GetOutput()->GetRequestedRegion() );
	bchfilter->Update();
	VelocityFieldPointer Zb = bchfilter->GetOutput();
	Zb->DisconnectPipeline();

	// Finally get 0.5*( Z( v, K_fluid * u_forward ) - Z( -v, K_fluid * u_backward ) )
	typedef SubtractImageFilter<
	VelocityFieldType, VelocityFieldType, VelocityFieldType>  SubtracterType;

	typename SubtracterType::Pointer subtracter = SubtracterType::New();
	subtracter->SetInput( 0, Zf );
	subtracter->SetInput( 1, Zb );

	subtracter->GraftOutput( this->GetOutput() );

	m_Multiplier->SetConstant( 0.5 );
	m_Multiplier->SetInput( subtracter->GetOutput() );
	m_Multiplier->GraftOutput( this->GetOutput() );

	// Triggers in place update
	m_Multiplier->GetOutput()->SetRequestedRegion( this->GetOutput()->GetRequestedRegion() );
	m_Multiplier->Update();

	// Region passing stuff
	this->GraftOutput( m_Multiplier->GetOutput() );

	//just Smooth the velocity field
	if( this->GetSmoothVelocityField() )
	  {
	    this->SmoothVelocityField();
	  }
}


template <class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
void
SymmetricLogDomainDemonsRegistrationTensorFilter<TFixedImage,TMovingImage,TField,TSolverPrecision>
::PrintSelf(std::ostream& os, Indent indent) const
{
	Superclass::PrintSelf( os, indent );

	os << indent << "Intensity difference threshold: " << this->GetIntensityDifferenceThreshold() << std::endl;
	os << indent << "Multiplier: " << m_Multiplier << std::endl;
	os << indent << "Adder: " << m_Adder << std::endl;
	os << indent << "NumberOfBCHApproximationTerms: " << m_NumberOfBCHApproximationTerms << std::endl;
}


} // end namespace itk

#endif
