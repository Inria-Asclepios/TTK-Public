#ifndef __itkSymmetricLogDomainDemonsRegistrationTensorFilter_h
#define __itkSymmetricLogDomainDemonsRegistrationTensorFilter_h

#include "itkLogDomainDeformableRegistrationFilter.h"
#include "itkESMDemonsRegistrationTensorFunction.h"

#include "itkMultiplyByConstantImageFilter.h"

namespace itk {

/**\class SymmetricLogDomainDemonsRegistrationTensorFilter
   * \brief Deformably register two tensor images using a symmetric log-domain
   * diffeomorphic demons algorithm.
   *
   * See A Sweet and X Pennec. "Log-Domain Diffeomorphic Registration of
   * Diffusion Tensor Images". Workshop on Biomedical Image Registration 2010.
   *
   * This class was contributed by Andrew Sweet, INRIA, based on variations
   * to the LogDomainDemonsRegistrationTensorFilter and the
   * SymmetricLogDomainDemonsRegistrationFilter. The modification to the former
   * is to incorporate a symmetric or inverse consistency constraint on the
   * registration, by projecting the velocity fields. The modification to the
   * latter is to use tensor images instead of scalar images.
   *
   * SymmetricLogDomainDemonsRegistrationTensorFilter implements the symmetric
   * log-domain demons deformable algorithm that register two images by
   * computing the inverse consistent deformation field which will map a
   * moving image onto a fixed image.
   *
   * A deformation field is represented as a image whose pixel type is some
   * vector type with at least N elements, where N is the dimension of
   * the fixed image. The vector type must support element access via operator
   * []. It is assumed that the vector elements behave like floating point
   * scalars.
   *
   * This class is templated over the fixed image type, moving image type
   * and the deformation field type.
   *
   * The input fixed and moving images are set via methods SetFixedImage
   * and SetMovingImage respectively. An initial deformation field maybe set via
   * SetInitialDisplacementField or SetInput. If no initial field is set,
   * a zero field is used as the initial condition.
   *
   * The output deformation field can be obtained via methods GetOutput
   * or GetDisplacementField.
   *
   * This class make use of the finite difference solver hierarchy. Each update
   * for each iteration is computed in ESMDemonsRegistrationTensorFunction.
   *
   * \warning This filter assumes that the fixed image type, moving image type
   * and deformation field type all have the same number of dimensions.
   *
   * \sa LogDomainDemonsRegistrationTensorFilter
   * \sa ESMDemonsRegistrationTensorFunction
   * \ingroup LogDomainDeformableImageRegistration MultiThreaded
   * \author Andrew Sweet.
   */
template<class TFixedImage, class TMovingImage, class TField, class TSolverPrecision>
class ITK_EXPORT SymmetricLogDomainDemonsRegistrationTensorFilter :
public LogDomainDeformableRegistrationFilter<TFixedImage,TMovingImage,TField>
{
public:
	/** Standard class typedefs. */
	typedef SymmetricLogDomainDemonsRegistrationTensorFilter    Self;
	typedef LogDomainDeformableRegistrationFilter<TFixedImage,TMovingImage,TField>   Superclass;
	typedef SmartPointer<Self>                            Pointer;
	typedef SmartPointer<const Self>                      ConstPointer;

	/** Method for creation through the object factory. */
	itkNewMacro(Self);

	/** Run-time type information (and related methods) */
	itkTypeMacro( SymmetricLogDomainDemonsRegistrationTensorFilter,LogDomainDeformableRegistrationFilter );

	/** FixedImage image type. */
	typedef typename Superclass::FixedImageType           FixedImageType;
	typedef typename Superclass::FixedImagePointer        FixedImagePointer;

	/** MovingImage image type. */
	typedef typename Superclass::MovingImageType          MovingImageType;
	typedef typename Superclass::MovingImagePointer       MovingImagePointer;

	/** Velocity field type. */
	typedef TField                                        VelocityFieldType;
	typedef typename VelocityFieldType::Pointer           VelocityFieldPointer;

	/** Displacement field type. */
	typedef typename Superclass::DisplacementFieldType     DisplacementFieldType;
	typedef typename Superclass::DisplacementFieldPointer  DisplacementFieldPointer;

	/** Types inherited from the superclass */
	typedef typename Superclass::OutputImageType          OutputImageType;

	/** FiniteDifferenceFunction type. */
	typedef typename Superclass::FiniteDifferenceFunctionType  FiniteDifferenceFunctionType;

	/** Take timestep type from the FiniteDifferenceFunction. */
	typedef typename
	FiniteDifferenceFunctionType::TimeStepType          TimeStepType;

	/** DemonsRegistrationFilterFunction type. */
	typedef ESMDemonsRegistrationTensorFunction<FixedImageType,
	MovingImageType, DisplacementFieldType, TSolverPrecision> DemonsRegistrationFunctionType;
	typedef typename DemonsRegistrationFunctionType::Pointer          DemonsRegistrationFunctionPointer;

	/** Set/Get the metric value. The metric value is the mean square difference
	 * in intensity between the fixed image and transforming moving image
	 * computed over the the overlapping region between the two images.
	 * This value is calculated for the current iteration */
	virtual void SetMetric(double metric) {m_Metric = metric;}
	virtual double GetMetric() const;

	/** Set/Get the number of pixels processed (the number in the overlap region). */
	virtual void SetNumberOfPixelsProcessed(unsigned long numberOfPixels)
	  {
	    m_NumberOfPixelsProcessed = numberOfPixels;
	  }
	virtual unsigned long GetNumberOfPixelsProcessed() const;

	/** Set/Get the rms change in the deformation field. */
    itkSetMacro(RMSChange, double);
	virtual const double &GetRMSChange() const;

	/** Set/Get the type of used image forces */
	typedef typename DemonsRegistrationFunctionType::GradientType GradientType;
	virtual void SetUseGradientType( GradientType gtype );
	virtual GradientType GetUseGradientType() const;

	/** Set/Get the type of used rotation for reorientation. */
	typedef typename DemonsRegistrationFunctionType::RotationType RotationType;
	virtual void SetUseRotationType( RotationType gtype );
	virtual RotationType GetUseRotationType() const;

	/** Set/Get the threshold below which the absolute difference of
	 * intensity yields a match. When the intensities match between a
	 * moving and fixed image pixel, the update vector (for that
	 * iteration) will be the zero vector. Default is 0.001. */
	virtual void SetIntensityDifferenceThreshold(double);
	virtual double GetIntensityDifferenceThreshold() const;

	/** Set/Get the maximum length in terms of pixels of
	 *  the vectors in the update buffer. */
	virtual void SetMaximumUpdateStepLength(double);
	virtual double GetMaximumUpdateStepLength() const;

	/** Set/Get the number of terms used in the Baker-Campbell-Hausdorff approximation. */
	itkSetMacro( NumberOfBCHApproximationTerms, unsigned int );
	itkGetConstMacro( NumberOfBCHApproximationTerms, unsigned int );

	/** Set/Get whether or not we precondition the solver. */
	virtual void SetPreconditionSolver( bool preconditionSolver )
	{ this->GetForwardRegistrationFunctionType()->SetPreconditionSolver(preconditionSolver);}
	virtual bool GetPreconditionSolver() const
	{ return this->GetForwardRegistrationFunctionType()->GetPreconditionSolver();}
protected:
	SymmetricLogDomainDemonsRegistrationTensorFilter();
	~SymmetricLogDomainDemonsRegistrationTensorFilter() {}
	void PrintSelf(std::ostream& os, Indent indent) const;

	/** Initialize the state of filter and equation before each iteration. */
	virtual void InitializeIteration();

	/** This method is called before iterating the solution. */
	virtual void Initialize();

	/** This method allocates storage in m_UpdateBuffer.  It is called from
	 * FiniteDifferenceFilter::GenerateData(). */
	virtual void AllocateUpdateBuffer();

	/** Method to allow subclasses to get direct access to the backward update
	 * buffer */
	virtual VelocityFieldType* GetBackwardUpdateBuffer()
	{ return m_BackwardUpdateBuffer; }

	/** This method allocates storage in m_BackwardUpdateBuffer. */
	virtual void AllocateBackwardUpdateBuffer();

	/** Utility to smooth the BackwardUpdateBuffer using a Gaussian operator.
	 * The amount of smoothing is specified by the UpdateFieldStandardDeviations. */
	virtual void SmoothBackwardUpdateField();

	typedef typename VelocityFieldType::RegionType ThreadRegionType;
	/** Does the actual work of calculating change over a region supplied by
	 * the multithreading mechanism. */
	virtual TimeStepType ThreadedCalculateChange(
			const ThreadRegionType &regionToProcess, int threadId);

	/** Apply update. */
	virtual void ApplyUpdate(TimeStepType dt);

private:
	SymmetricLogDomainDemonsRegistrationTensorFilter(const Self&); //purposely not implemented
	void operator=(const Self&); //purposely not implemented

	/** Downcast the DifferenceFunction using a dynamic_cast to ensure that it is of the correct type.
	 * this method will throw an exception if the function is not of the expected type. */
	DemonsRegistrationFunctionType *  GetForwardRegistrationFunctionType();
	const DemonsRegistrationFunctionType *  GetForwardRegistrationFunctionType() const;

	/** Exp and composition typedefs */
	typedef MultiplyByConstantImageFilter<
	VelocityFieldType,
	TimeStepType, VelocityFieldType >                   MultiplyByConstantType;

	typedef AddImageFilter<
	VelocityFieldType, VelocityFieldType>                AdderType;

	typedef typename MultiplyByConstantType::Pointer      MultiplyByConstantPointer;
	typedef typename AdderType::Pointer                   AdderPointer;

	typename FiniteDifferenceFunctionType::Pointer        m_BackwardDifferenceFunction;

	MultiplyByConstantPointer                             m_Multiplier;
	AdderPointer                                          m_Adder;
	unsigned int                                          m_NumberOfBCHApproximationTerms;

	/** The buffer that holds the updates for an iteration of the algorithm. */
	VelocityFieldPointer                                  m_BackwardUpdateBuffer;

  /** We store average of forwards and backwards solvers for these metrics */
	double m_RMSChange;
	unsigned long m_NumberOfPixelsProcessed;
	double m_Metric;
};


} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSymmetricLogDomainDemonsRegistrationTensorFilter.txx"
#endif

#endif
