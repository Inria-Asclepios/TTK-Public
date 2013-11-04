/*=========================================================================

 Program:   Insight Segmentation & Registration Toolkit
 Module:    $RCSfile: itkESMDemonsRegistrationTensorFunction.h,v $
 Language:  C++
 Date:      $Date$
 Version:   $Revision$

 Copyright (c) Insight Software Consortium. All rights reserved.
 See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

#ifndef __itkESMDemonsRegistrationTensorFunction_h
#define __itkESMDemonsRegistrationTensorFunction_h

#include "itkPDEDeformableRegistrationFunction.h"
#include "itkPoint.h"
#include "itkWarpTensorImageFilter.h"
#include "itkWarpTensorImageWOReorientationFilter.h"
#include "itkMatrix.h"
#include "itkCentralDifferenceTensorImageFunction.h"
#include "itkWarpJacobianFilter.h"
#include "gmm/gmm.h"

#include <vector>

namespace itk
{

/**
 * \class ESMDemonsRegistrationTensorFunction
 *
 * \brief Fast implementation of the symmetric demons registration force for
 * tensors.
 *
 * This class provides a substantially faster implementation of the
 * symmetric demons registration force. Speed is improved by keeping
 * a deformed copy of the moving image for gradient evaluation.
 *
 * Symmetric forces simply means using the mean of the gradient
 * of the fixed image and the gradient of the warped moving
 * image.
 *
 * Note that this class also enables the use of fixed, warped moving images
 * forces by using a call to SetUseGradientType.
 *
 * The moving image should not be saturated, so we use
 * NumericTraits<MovingPixelType>::Max() as a special value.
 *
 * This class was contributed by Thomas Yeo and adapted by Andrew Sweet, INRIA.
 *
 * \sa ESMDemonsRegistrationFunction
 * \ingroup FiniteDifferenceFunctions
 *
 */
template < class TFixedImage, class TMovingImage, class TDeformationField, class TSolverPrecision >
class ITK_EXPORT ESMDemonsRegistrationTensorFunction : public
PDEDeformableRegistrationFunction<TFixedImage, TMovingImage, TDeformationField>
{
public:
  /** Standard class typedefs. */
  typedef ESMDemonsRegistrationTensorFunction Self;
  typedef PDEDeformableRegistrationFunction<TFixedImage, TMovingImage,
  TDeformationField> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro( ESMDemonsRegistrationTensorFunction,
      PDEDeformableRegistrationFunction );

  /** MovingImage image type. */
  typedef typename Superclass::MovingImageType MovingImageType;
  typedef typename Superclass::MovingImagePointer MovingImagePointer;
  typedef typename Superclass::MovingImageType::PixelType MovingImagePixelType;

  /** FixedImage image type. */
  typedef typename Superclass::FixedImageType FixedImageType;
  typedef typename Superclass::FixedImagePointer FixedImagePointer;
  typedef typename Superclass::FixedImageType::PixelType FixedImagePixelType;
  typedef typename FixedImageType::IndexType IndexType;
  typedef typename FixedImageType::SizeType SizeType;
  typedef typename FixedImageType::SpacingType SpacingType;
  typedef typename FixedImageType::DirectionType  DirectionType;

  /** Deformation field type. */
  typedef typename Superclass::DeformationFieldType DeformationFieldType;
  typedef typename Superclass::DeformationFieldTypePointer
  DeformationFieldTypePointer;
  typedef typename DeformationFieldType::PixelType DeformationFieldVectorType;
  typedef typename DeformationFieldVectorType::ValueType DeformationFieldRealType;
  typedef Matrix<DeformationFieldRealType, Superclass::ImageDimension, Superclass::ImageDimension> DeformationFieldMatrixType;
  typedef vnl_matrix_fixed<DeformationFieldRealType, Superclass::ImageDimension, Superclass::ImageDimension> DeformationFieldMatrixInternalType;
  typedef vnl_vector<DeformationFieldRealType> DeformationFieldVectorInternalType;
  typedef Tensor<DeformationFieldRealType, Superclass::ImageDimension> DeformationFieldTensorType;

  /** Inherit some enums from the superclass. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  /** Inherit some enums from the superclass. */
  typedef typename Superclass::PixelType PixelType;
  typedef typename Superclass::RadiusType RadiusType;
  typedef typename Superclass::NeighborhoodType NeighborhoodType;
  typedef typename Superclass::FloatOffsetType FloatOffsetType;
  typedef typename Superclass::TimeStepType TimeStepType;

  typedef typename FixedImageType::PointType PointType;

  /** Warper type */
  typedef WarpTensorImageFilter<MovingImageType, MovingImageType,
  DeformationFieldType> WarperType;
  typedef typename WarperType::Pointer WarperPointer;

  /** Warper type */
  typedef WarpTensorImageWOReorientationFilter<MovingImageType,
  MovingImageType, DeformationFieldType> WarperWOReorientationType;
  typedef typename WarperWOReorientationType::Pointer
  WarperWOReorientationPointer;

  /** Gradient calculator types. */
  typedef CentralDifferenceTensorImageFunction<FixedImageType>
  GradientCalculatorType;
  typedef typename GradientCalculatorType::Pointer
  GradientCalculatorPointer;

  /** Tensor value and gradient types */
  typedef typename MovingImageType::PixelType TensorType;
  typedef typename TensorType::RealType TensorRealType;
  typedef typename TensorType::MatrixType TensorMatrixType;
  typedef typename TensorType::MatrixType::InternalMatrixType TensorMatrixInternalType;
  //  typedef vnl_matrix<TensorRealType> TensorMatrixInternalType;
  typedef Matrix<TensorRealType,TensorType::Dimension*(TensorType::Dimension+1)/2, Superclass::ImageDimension> TensorGradientType;

  typedef typename itk::Image<long, FixedImageType::ImageDimension> LongImageType;

  /** GMM typedefs for writing/storing system. */
  typedef typename itk::Vector<TSolverPrecision, Superclass::ImageDimension> SolutionVectorType;
  typedef typename std::vector<TSolverPrecision> SolverVectorType;
  typedef typename gmm::row_matrix< typename gmm::wsvector<TSolverPrecision> >
  SolverWriteMatrixType;

  /** GMM typedefs for solving system (read only sparse matrix). */
  typedef typename gmm::csr_matrix<TSolverPrecision> SolverReadMatrixType;

  /** This class uses a constant timestep of 1. */
  virtual TimeStepType ComputeGlobalTimeStep(void * itkNotUsed(GlobalData))
  const
  { return m_TimeStep;}

  /** Return a pointer to a global data structure that is passed to
   * this object from the solver at each calculation.  */
  virtual void *GetGlobalDataPointer() const
  {
    GlobalDataStruct *global = new GlobalDataStruct();
    global->m_SumOfSquaredDifference = 0.0;
    global->m_NumberOfPixelsProcessed = 0L;
    global->m_SumOfSquaredChange = 0;
    global->m_NormalizedSumOfSquaredDifference = 0.0;
    return global;
  }

  /** Release memory for global data structure. */
  virtual void ReleaseGlobalDataPointer( void *GlobalData ) const;

  /** Set the object's state before each iteration. */
  virtual void InitializeIteration();

  /** This method is called by a finite difference solver image filter at
   * each pixel that does not lie on a data set boundary. When considering
   * reorientation this just stores the system to be solved to find the update
   * (and returns a zero update). */
  virtual PixelType ComputeUpdate(const NeighborhoodType &neighborhood,
      void *globalData, const FloatOffsetType &offset = FloatOffsetType(0.0));

  /** This method actually solves the system to find an update */
  virtual void SolveUpdate(DeformationFieldTypePointer);

  /** Set/Get the minimum allowable tensor variance */
  virtual void SetTensorMinimumVariance(double tensorMinimumVariance)
  {m_TensorMinimumVariance = tensorMinimumVariance;}
  virtual double GetTensorMinimumVariance() const
  {return m_TensorMinimumVariance;}

  /** Get the metric value. The metric value is the mean square difference
   * in intensity between the fixed image and transforming moving image
   * computed over the the overlapping region between the two images. */
  virtual double GetMetric() const
  { return m_Metric; }

  /** Get the number of pixels processed (the number in the overlap region). */
  virtual unsigned long GetNumberOfPixelsProcessed() const
  { return m_NumberOfPixelsProcessed; }

  /** Get the rms change in the deformation field. */
  virtual const double &GetRMSChange() const
  { return m_RMSChange; }

  /** Set/Get the threshold below which the absolute difference of
   * intensity yields a match. When the intensities match between a
   * moving and fixed image pixel, the update vector (for that
   * iteration) will be the zero vector. Default is 0.001. */
  virtual void SetIntensityDifferenceThreshold(double intensityDifferenceThreshold)
  {m_IntensityDifferenceThreshold = intensityDifferenceThreshold;}
  virtual double GetIntensityDifferenceThreshold() const
  {return m_IntensityDifferenceThreshold;}

  /** Set/Get the maximum update step length. In Thirion this is 0.5.
   *  Setting it to 0 implies no restriction (beware of numerical
   *  instability in this case. */
  virtual void SetMaximumUpdateStepLength(double maximumUpdateStepLength){
    this->m_MaximumUpdateStepLength = maximumUpdateStepLength;};
  virtual double GetMaximumUpdateStepLength() const {
    return this->m_MaximumUpdateStepLength;};

  /** Type of available image forces */
  enum GradientType
  {
    Symmetric=0,
    Fixed,
    Moving
  };

  /** The types of rotation we use to optimize the registration wrt tensor
   * reorientation */
  enum RotationType
  {
    Rotation=0,
    NoRotation,
    ApproxRotation
  };

  /** Set/Get the type of used image forces */
  virtual void SetUseGradientType( GradientType gradientType )
  { m_UseGradientType = gradientType;}
  virtual GradientType GetUseGradientType() const
  { return m_UseGradientType;}

  /** Set/Get the type of used rotation for reorientation. */
  virtual void SetUseRotationType( RotationType rotationType )
  { m_UseRotationType = rotationType;}
  virtual RotationType GetUseRotationType() const
  { return m_UseRotationType;}

  /** Set/Get whether or not we precondition the solver. */
  virtual void SetPreconditionSolver( bool preconditionSolver )
  { m_PreconditionSolver = preconditionSolver;}
  virtual bool GetPreconditionSolver() const
  { return m_PreconditionSolver;}

protected:
  ESMDemonsRegistrationTensorFunction();
  ~ESMDemonsRegistrationTensorFunction()
  {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** A global data type for this class of equation. Used to store
   * iterators for the fixed image. */
  struct GlobalDataStruct
  {
    double m_SumOfSquaredDifference;
    unsigned long m_NumberOfPixelsProcessed;
    double m_SumOfSquaredChange;
    double m_NormalizedSumOfSquaredDifference;
  };

private:
  ESMDemonsRegistrationTensorFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Cache fixed image information. */
  SpacingType m_FixedImageSpacing;
  PointType m_FixedImageOrigin;
  DirectionType m_FixedImageDirection;
  double m_Normalizer;

  /** Function to compute derivatives of the fixed image. */
  GradientCalculatorPointer m_FixedImageGradientCalculator;

  /** Type of image force to use. */
  GradientType m_UseGradientType;

  /** Type of rotation for tensor reorientation to use. */
  RotationType m_UseRotationType;

  /** Filter to warp moving image for fast gradient computation. */
  WarperPointer m_MovingImageWarper;
  WarperWOReorientationPointer m_MovingImageWOReorientationWarper;

  /* WarpJacobianImage */
  typedef Image<DeformationFieldMatrixType, Superclass::ImageDimension> JacobianImageType;
  typedef typename JacobianImageType::Pointer JacobianImagePointer;
  typedef WarpJacobianFilter<DeformationFieldType, JacobianImageType> JacobianFilterType;

  /** The global timestep. */
  TimeStepType m_TimeStep;

  /** Threshold below which the denominator term is considered zero. */
  double m_DenominatorThreshold;

  /** Threshold below which two intensity value are assumed to match. */
  double m_IntensityDifferenceThreshold;

  /** Maximum update step length in pixels (default is 0.5 as in Thirion). */
  double m_MaximumUpdateStepLength;

  /** minimum allowable variance of tensor */
  double m_TensorMinimumVariance;

  /** The metric value is the mean square difference in intensity between
   * the fixed image and transforming moving image computed over the
   * the overlapping region between the two images. */
  mutable double m_Metric;

  /** The sum of square difference in intensity between the fixed image and
   *  transforming moving image computed over the overlapping region between
   *  the two images. */
  mutable double m_SumOfSquaredDifference;

  /** The number of pixels processed (i.e. the number in the overlap region).*/
  mutable unsigned long m_NumberOfPixelsProcessed;

  /** The root mean square change in the deformation field from one iteration
   * to the next. */
  mutable double m_RMSChange;

  /** The sum of square change in the deformation field from one iteration
   * to the next. */
  mutable double m_SumOfSquaredChange;

  /** Mutex lock to protect modification to metric. */
  mutable SimpleFastMutexLock m_MetricCalculationLock;
  mutable SimpleFastMutexLock m_GradLock;
  mutable SimpleFastMutexLock m_ResidualLock;
  mutable SimpleFastMutexLock m_UpdateFieldLock;

  /** Stores index of foreground voxels or -1 for background. */
  typename LongImageType::Pointer m_ForegroundRef;

  /** b in Ax - b system to solve */
  SolverVectorType *m_Residual;

  /** A in Ax - b system to solve */
  SolverWriteMatrixType *m_Grad;

  /** Jacobian of moving image */
  JacobianImagePointer m_Jacobian;

  /** Whether or not to precondition system before solver. */
  bool m_PreconditionSolver;

  /** Compute the moving image gradient by hand. */
  TensorGradientType ComputeMovingGradient(const TensorType, const IndexType, const IndexType, const IndexType);

  /** Element wise power of a tensor. */
  TensorType ElementWisePowerTensor(const TensorType, double);

  /** Element wise multiplication of a tensor by a another. */
  TensorType ElementWiseMultiplyTensor(const TensorType, const TensorType);

  /** Element wise division of a tensor by a another. */
  TensorType ElementWiseDivideTensor(const TensorType, const TensorType);

  /** Inserts 3 single tensors into the matrix that represents the gradient of
   * a single tensor */
  TensorGradientType InsertTensorsInTensorGradient(const TensorType, const TensorType, const TensorType);

  /** Perform reorientation on a tensor gradient. */
  TensorGradientType ReorientateMovingGradient(const DeformationFieldMatrixType, const TensorGradientType);

  /** Insert a single tensor into the residual in the system to solve */
  void InsertTensorIntoResidual(const TensorType, unsigned long);

  /** Insert a single tensor gradient into the sparse matrix in the system to
   * solve */
  void InsertTensorGradientIntoGradient(const TensorGradientType, unsigned long, unsigned long);

  /** Creates the 'cross product' operating matrix of a vector. */
  DeformationFieldMatrixType CreateCrossProductMatrix(const DeformationFieldVectorType);
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkESMDemonsRegistrationTensorFunction.txx"
#endif

#endif
