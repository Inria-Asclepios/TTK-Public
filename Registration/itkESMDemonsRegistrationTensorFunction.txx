/*=========================================================================

 Program:   Insight Segmentation & Registration Toolkit
 Module:    $RCSfile: itkESMDemonsRegistrationTensorFunction.txx,v $
 Language:  C++
 Date:      $Date$
 Version:   $Revision$

 Copyright (c) Insight Software Consortium. All rights reserved.
 See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

#ifndef __itkESMDemonsRegistrationTensorFunction_txx
#define __itkESMDemonsRegistrationTensorFunction_txx

#include "itkESMDemonsRegistrationTensorFunction.h"
#include "itkExceptionObject.h"
#include "vnl/vnl_math.h"
#include "vnl/vnl_vector.h"
#include "vnl/vnl_matrix.h"
#include "vnl/algo/vnl_cholesky.h"

#include <math.h>
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

namespace itk
{

/**
 * Casts the real type of an ITK vector.
 */
template < class FromVectorType, class ToVectorType >
ToVectorType
CastVector(FromVectorType fromVector)
{
  ToVectorType toVector;

  unsigned int toDims = ToVectorType::Dimension;
  typedef    typename ToVectorType::ValueType ToVectorRealType;

  unsigned int fromDims = FromVectorType::Dimension;

  if (fromDims != toDims)
    {
      std::cerr << "Numbers of dims do not match." << std::endl;
      exit(EXIT_FAILURE);
    }

  for (unsigned int i = 0; i < fromDims; i++)
    {
      toVector[i] = static_cast<ToVectorRealType>(fromVector[i]);
    }

  return toVector;
}

/**
 * Casts the real type of an ITK matrix.
 */
template < class FromMatrixType, class ToMatrixType >
ToMatrixType
CastMatrix(FromMatrixType fromMatrix)
{
  ToMatrixType toMatrix;

  unsigned int toRows = ToMatrixType::RowDimensions;
  unsigned int toColumns = ToMatrixType::ColumnDimensions;
  typedef typename ToMatrixType::ValueType ToMatrixRealType;

  unsigned int fromRows = FromMatrixType::RowDimensions;
  unsigned int fromColumns = FromMatrixType::ColumnDimensions;

  if (fromRows != toRows)
    {
      std::cerr << "Numbers of rows do not match." << std::endl;
      exit(EXIT_FAILURE);
    }

  if (fromColumns != toColumns)
    {
      std::cerr << "Numbers of columns do not match." << std::endl;
      exit(EXIT_FAILURE);
    }

  for (unsigned int i = 0; i < fromRows; i++)
    {
      for (unsigned int j = 0; j < fromColumns; j++)
        {
          toMatrix[i][j] = static_cast<ToMatrixRealType>(fromMatrix[i][j]);
        }
    }

  return toMatrix;
}

/*
 * Default constructor
 */
template < class TFixedImage, class TMovingImage, class TDeformationField,
class TSolverPrecision >
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::ESMDemonsRegistrationTensorFunction()
{
  if(ImageDimension != 3)
    {
      std::cerr << "Can only handle 3D tensor images." << std::endl;
      exit(EXIT_FAILURE);
    }

  RadiusType r;
  unsigned int j;
  for (j = 0; j < ImageDimension; j++)
    {
      r[j] = 0;
    }
  this->SetRadius(r);

  m_TimeStep = 1.0;
  m_DenominatorThreshold = 1e-9;
  m_IntensityDifferenceThreshold = 0.001;
  m_MaximumUpdateStepLength = 0.5;

  this->SetMovingImage(NULL);
  this->SetFixedImage(NULL);
  m_FixedImageSpacing.Fill(1.0);
  m_FixedImageOrigin.Fill(0.0);
  m_FixedImageDirection.SetIdentity();
  m_Normalizer = 0.0;
  m_FixedImageGradientCalculator = GradientCalculatorType::New();
  // Gradient orientation will be taken care of explicitely
  m_FixedImageGradientCalculator->UseImageDirectionOff();

  this->m_UseGradientType = Symmetric;
  this->m_UseRotationType = Rotation;

  m_MovingImageWarper = WarperType::New();
  m_MovingImageWarper->SetEdgePaddingValue(NumericTraits<
      typename MovingImageType::PixelType>::Zero);

  m_MovingImageWOReorientationWarper = WarperWOReorientationType::New();
  m_MovingImageWOReorientationWarper->SetEdgePaddingValue(NumericTraits<
      typename MovingImageType::PixelType>::Zero);

  m_Metric = NumericTraits<double>::max();
  m_SumOfSquaredDifference = 0.0;
  m_NumberOfPixelsProcessed = 0L;
  m_RMSChange = NumericTraits<double>::max();
  m_SumOfSquaredChange = 0.0;

  m_ForegroundRef = 0;
  m_Grad = 0;
  m_Residual = 0;
  m_Jacobian = 0;

  m_PreconditionSolver = false;

  m_TensorMinimumVariance = 0.001;
}

/*
 * Standard "PrintSelf" method.
 */
template < class TFixedImage, class TMovingImage, class TDeformationField,
class TSolverPrecision >
void
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::PrintSelf(std::ostream& os,
    Indent indent) const
    {
      Superclass::PrintSelf(os, indent);

      os << indent << "FixedImageGradientCalculator: ";
      os << m_FixedImageGradientCalculator.GetPointer() << std::endl;
      os << indent << "DenominatorThreshold: ";
      os << m_DenominatorThreshold << std::endl;
      os << indent << "IntensityDifferenceThreshold: ";
      os << m_IntensityDifferenceThreshold << std::endl;

      os << indent << "Metric: ";
      os << m_Metric << std::endl;
      os << indent << "SumOfSquaredDifference: ";
      os << m_SumOfSquaredDifference << std::endl;
      os << indent << "NumberOfPixelsProcessed: ";
      os << m_NumberOfPixelsProcessed << std::endl;
      os << indent << "RMSChange: ";
      os << m_RMSChange << std::endl;
      os << indent << "SumOfSquaredChange: ";
      os << m_SumOfSquaredChange << std::endl;
    }

/*
 * Set the function state values before each iteration
 */
template < class TFixedImage, class TMovingImage, class TDeformationField,
class TSolverPrecision >
void
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::InitializeIteration()
{
  if ( !this->GetFixedImage() )
    {
      itkExceptionMacro ( << "FixedImage not set" );
    }
  if ( !this->GetMovingImage() )
    {
      itkExceptionMacro ( << "MovingImage not set" );
    }

  // cache fixed image information
  m_FixedImageSpacing = this->GetFixedImage()->GetSpacing();
  m_FixedImageOrigin = this->GetFixedImage()->GetOrigin();
  m_FixedImageDirection  = this->GetFixedImage()->GetDirection();

  // compute the normalizer
  if ( m_MaximumUpdateStepLength > 0.0 )
    {
      m_Normalizer = 0.0;
      for( unsigned int k = 0; k < ImageDimension; k++ )
        {
          m_Normalizer += m_FixedImageSpacing[k] * m_FixedImageSpacing[k];
        }
      // note the muliplication by 2.0 (compared to scalar version)
      m_Normalizer *= 2.0*2.0*m_MaximumUpdateStepLength *
      m_MaximumUpdateStepLength / static_cast<double>( ImageDimension );
    }
  else
    {
      // set it to minus one to avoid rounding errors
      m_Normalizer = -1.0;
    }

  if(this->m_UseRotationType != Rotation)
    {
      m_MovingImageWarper->SetOutputSpacing(
          this->GetFixedImage()->GetSpacing() );
      m_MovingImageWarper->SetOutputOrigin(
          this->GetFixedImage()->GetOrigin() );
      m_MovingImageWarper->SetOutputDirection( this->GetFixedImage()->GetDirection() );
      m_MovingImageWarper->SetInput( this->GetMovingImage() );
      m_MovingImageWarper->SetDeformationField( this->GetDeformationField() );
      m_MovingImageWarper->GetOutput()->SetRequestedRegion(
          this->GetDeformationField()->GetRequestedRegion() );
      m_MovingImageWarper->Update();
    }
  else
    { //true rotation gradient requires special warper and Jacobian calculator
      m_MovingImageWOReorientationWarper->SetOutputSpacing(
          this->GetFixedImage()->GetSpacing() );
      m_MovingImageWOReorientationWarper->SetOutputOrigin(
          this->GetFixedImage()->GetOrigin() );
      m_MovingImageWOReorientationWarper->SetOutputDirection( this->GetFixedImage()->GetDirection() );
      m_MovingImageWOReorientationWarper->SetInput( this->GetMovingImage() );
      m_MovingImageWOReorientationWarper->SetDeformationField(
          this->GetDeformationField() );
      m_MovingImageWOReorientationWarper->GetOutput()->SetRequestedRegion(
          this->GetDeformationField()->GetRequestedRegion() );
      m_MovingImageWOReorientationWarper->Update();

      typename JacobianFilterType::Pointer jacobianFilter =
        JacobianFilterType::New();
      jacobianFilter->SetInput( this->GetDeformationField() );
      jacobianFilter->SetUseImageSpacing( true );

      try
      {
        jacobianFilter->Update();
      }
      catch(itk::ExceptionObject &e)
      {
        std::cerr << e;
        throw itk::ExceptionObject(__FILE__, __LINE__,
            "Error in WarpTensorImageFilter::BeforeThreadedGenerateData()");
      }

      // note that jacobian filter computes jacobian + identity
      m_Jacobian = jacobianFilter->GetOutput();
    }

  // setup gradient calculator
  m_FixedImageGradientCalculator->SetInputImage( this->GetFixedImage() );

  // initialize metric computation variables
  m_SumOfSquaredDifference = 0.0;
  m_NumberOfPixelsProcessed = 0L;
  m_SumOfSquaredChange = 0.0;

  if(m_UseRotationType != NoRotation)
    {
      // beginning of new multires level, new memory for foreground
      if( !m_ForegroundRef ||
          (this->m_ForegroundRef->GetLargestPossibleRegion().GetSize() !=
            this->GetFixedImage()->GetLargestPossibleRegion().GetSize()))
        {
          m_ForegroundRef = LongImageType::New();
          m_ForegroundRef->SetRegions(
              this->GetFixedImage()->GetLargestPossibleRegion());
          m_ForegroundRef->SetSpacing(this->GetFixedImage()->GetSpacing());
          m_ForegroundRef->SetOrigin(this->GetFixedImage()->GetOrigin());
	  m_ForegroundRef->SetDirection(this->GetFixedImage()->GetDirection());
          m_ForegroundRef->Allocate();
        }

      long count = 0;

      typedef typename itk::ImageRegionConstIterator<FixedImageType>
      FixedIteratorType;
      FixedIteratorType fixIt = FixedIteratorType(this->GetFixedImage(),
          this->GetFixedImage()->GetLargestPossibleRegion());

      typedef typename itk::ImageRegionConstIterator< MovingImageType>
      MovingIteratorType;
      MovingIteratorType movIt;

      // checks whether we require reorientation during warp
      if ( this->m_UseRotationType == Rotation )
        {
          movIt = MovingIteratorType(
              m_MovingImageWOReorientationWarper->GetOutput(),
              m_MovingImageWOReorientationWarper->GetOutput()->
              GetLargestPossibleRegion());
        }
      else
        {
          movIt = MovingIteratorType(m_MovingImageWarper->GetOutput(),
              m_MovingImageWarper->GetOutput()->GetLargestPossibleRegion());
        }

      typedef typename itk::ImageRegionIterator<LongImageType> IteratorType;
      IteratorType It = IteratorType(m_ForegroundRef,
          m_ForegroundRef->GetLargestPossibleRegion());

      for (It.GoToBegin(), fixIt.GoToBegin(), movIt.GoToBegin(); !It.IsAtEnd(); ++It, ++fixIt, ++movIt )
        {
          if ( fixIt.Get() == NumericTraits<typename FixedImageType::PixelType>::Zero &&
              movIt.Get() == NumericTraits<typename MovingImageType::PixelType>::Zero )
            {
              It.Set(-1);
            }
          else
            {
              It.Set(count);
              count++;
            }
        }

      // only store foreground voxels for memory reasons
      m_Residual = new SolverVectorType((6+3)*count, 0.0);
      m_Grad = new SolverWriteMatrixType((6+3)*count, 3*count);

    }
}

/*
 * Compute update at a non boundary neighbourhood. When considering
 * reorientation this just stores the system to be solved to find the update
 * (and returns a zero update).
 */
template <class TFixedImage, class TMovingImage, class TDeformationField, class TSolverPrecision>
typename ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage, TDeformationField, TSolverPrecision>::PixelType
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage, TDeformationField, TSolverPrecision>
::ComputeUpdate(const NeighborhoodType &it, void * gd,
    const FloatOffsetType& itkNotUsed(offset))
{
  GlobalDataStruct *globalData = (GlobalDataStruct *)gd;
  PixelType update;
  IndexType FirstIndex = this->GetFixedImage()->GetLargestPossibleRegion().GetIndex();
  IndexType LastIndex = this->GetFixedImage()->GetLargestPossibleRegion().GetIndex() +
  this->GetFixedImage()->GetLargestPossibleRegion().GetSize();

  const IndexType index = it.GetIndex();

  // Get fixed image related information
  // Note: no need to check the index is within
  // fixed image buffer. This is done by the external filter.
  FixedImagePixelType fixedPixValue = this->GetFixedImage()->GetPixel( index );
  bool zeroFixedImage = false;
  if ( fixedPixValue == NumericTraits<FixedImagePixelType>::Zero )
    {
      update.Fill( 0.0 );
      zeroFixedImage = true;
    }
  const TensorType fixedValue = static_cast<TensorType>(fixedPixValue);
  TensorGradientType fixedGradient = m_FixedImageGradientCalculator->EvaluateAtIndex( index );

  typedef Vector<double, TFixedImage::ImageDimension> VectorType;
  
  for (unsigned int vdim=0; vdim<TensorType::DegreesOfFreedom; vdim++)
  {
    VectorType v;
    for (unsigned int dim=0; dim<TFixedImage::ImageDimension; dim++)
    {
      v[dim] = fixedGradient (vdim, dim);     
    }
    this->GetFixedImage()->TransformLocalVectorToPhysicalVector (v, v);
    for (unsigned int dim=0; dim<TFixedImage::ImageDimension; dim++)
    {
      fixedGradient (vdim, dim) = v[dim];
    }
  }

  // Get moving image related information
  // check if the point was mapped outside of the moving image using
  // the "special value" NumericTraits<MovingPixelType>::Zero
  MovingImagePixelType movingPixValue;
  bool zeroMovingImage = false;
  if ( this->m_UseRotationType == Rotation )
    {
      movingPixValue = m_MovingImageWOReorientationWarper->GetOutput()->GetPixel( index );
    }
  else
    {
      movingPixValue = m_MovingImageWarper->GetOutput()->GetPixel( index );
    }

  if ( movingPixValue == NumericTraits<MovingImagePixelType>::Zero )
    {
      update.Fill( 0.0 );
      zeroMovingImage = true;
      if(zeroFixedImage)
        {
          return update;
        }
    }
  const TensorType movingValue = static_cast<TensorType>( movingPixValue );

  // jacobian of transformation
  DeformationFieldMatrixType J;
  if ( this->m_UseRotationType == Rotation )
    {
      J = m_Jacobian->GetPixel(index);
    }

  // rotation component of jacobian
  DeformationFieldMatrixType R;
  DeformationFieldMatrixType JJtsqrt;
  if ( m_UseRotationType == Rotation )
    {
      DeformationFieldTensorType JJt;
      JJt.SetVnlMatrix(J.GetVnlMatrix() * J.GetTranspose());

      JJtsqrt = DeformationFieldMatrixType(JJt.Sqrt().GetVnlMatrix());

      R = DeformationFieldMatrixType(JJtsqrt.GetInverse()*J.GetVnlMatrix());
    }

  // gradient of image we actually use depends on gradient type
  TensorGradientType usedGradient;

  if ( m_UseGradientType != Fixed )
    {
      TensorGradientType movingGradient = ComputeMovingGradient(movingValue, index, FirstIndex, LastIndex);

      // the gradient of the un-reorientated moving image is reorientated in this case
      if ( m_UseRotationType == Rotation )
        {
          movingGradient = ReorientateMovingGradient(R, movingGradient);
        }


      if (this->m_UseGradientType == Symmetric)
        {
          usedGradient = (fixedGradient + movingGradient) / 2.0;
        }
      else if (this->m_UseGradientType == Moving)
        {
          usedGradient = (movingGradient + movingGradient) / 2.0;
        }
      else
        {
          itkExceptionMacro(<<"Unknown gradient type");
        }
      
    }
  else if (this->m_UseGradientType==Fixed)
    {
      usedGradient = (fixedGradient + fixedGradient) / 2.0;
    }
  else
    {
      itkExceptionMacro(<<"Unknown gradient type");
    }

   for (unsigned int vdim=0; vdim<TensorType::DegreesOfFreedom; vdim++)
   {
     VectorType v;
     for (unsigned int dim=0; dim<TFixedImage::ImageDimension; dim++)
     {
       v[dim] = usedGradient (vdim, dim);     
     }
     this->GetFixedImage()->TransformLocalVectorToPhysicalVector (v, v); // warped moving image is defined in the fixed image coordinate system
     for (unsigned int dim=0; dim<TFixedImage::ImageDimension; dim++)
     {
       usedGradient (vdim, dim) = v[dim];
     }
   }
   
  // compute values of linear system due to this point
  // We avoid the mismatch in units between the two terms.
  // and avoid a large step using a normalization term.

  TensorType reorientedMovingValue = movingValue;

  if ( m_UseRotationType == Rotation )
    {
      // ApplyMatrix sets the tensor to be R*T*R' as we want
      reorientedMovingValue = reorientedMovingValue.ApplyMatrix(
          CastMatrix<DeformationFieldMatrixType, TensorMatrixType>(
              DeformationFieldMatrixType(R.GetTranspose())));
    }

  TensorType speedValue;
  if(m_UseRotationType == Rotation)
    {
      speedValue = fixedValue - reorientedMovingValue;
    }
  else
    {
      speedValue = fixedValue - movingValue;
    }

  TensorType sqSpeedValue = ElementWiseMultiplyTensor(speedValue, speedValue);
  TensorType absSpeedValue = ElementWisePowerTensor(sqSpeedValue, 0.5);

  // limit the variance
  TensorType varValue = static_cast<TensorType>(sqSpeedValue);
  for(unsigned int row = 0; row < TensorType::NDegreesOfFreedom; row++)
    {
      if(varValue.GetNthComponent(row) < m_TensorMinimumVariance)
        {
          varValue.SetNthComponent(row, m_TensorMinimumVariance);
        }
    }

  // when there's no rotation the Gauss-Newton Hessian is simply the
  // outer product of the gradient
  if(this->m_UseRotationType == NoRotation)
    {
      vnl_vector<double> tempUpdate(ImageDimension,0);

      const TensorGradientType varMatrix = InsertTensorsInTensorGradient(
          varValue, varValue, varValue);

      const TensorGradientType speedMat = InsertTensorsInTensorGradient(
          speedValue, speedValue, speedValue);

      const TensorGradientType trueGradient(element_quotient(element_product(
          usedGradient.GetVnlMatrix(), speedMat.GetVnlMatrix()),
          varMatrix.GetVnlMatrix()));

      for (unsigned i = 0; i < ImageDimension; i++)
        {
          for (unsigned int j = 0; j < 6; j++)
            {
              if(j==1 || j==3 || j==4)
                {
                  tempUpdate[i] += static_cast<double>(2.0 * trueGradient[j][i]);
                }
              else
                {
                  tempUpdate[i] += static_cast<double>(trueGradient[j][i]);
                }
            }
        }

      typedef Matrix<double, ImageDimension, ImageDimension> HessianType;
      typedef Vector<typename TensorGradientType::ValueType, TensorGradientType::ColumnDimensions> TensorGradientVectorType;

      HessianType hessian;

      for (unsigned int i = 0; i < 6; i++)
        {
          DeformationFieldVectorInternalType gradientRow =
            CastVector<TensorGradientVectorType, DeformationFieldVectorType>(
                TensorGradientVectorType(usedGradient[i])).GetVnlVector();

          if(i==1 || i==3 || i==4)
            {
              hessian += CastMatrix<DeformationFieldMatrixType, HessianType>(
                  DeformationFieldMatrixType(outer_product(gradientRow, gradientRow)))
                  / varMatrix(i,0) * 2.0;
            }
          else
            {
              hessian += CastMatrix<DeformationFieldMatrixType, HessianType>(
                  DeformationFieldMatrixType(outer_product(gradientRow, gradientRow)))
                  / varMatrix(i,0);
            }
        }

      // contribution from distance term
      HessianType augmentor;
      augmentor.SetIdentity();
      augmentor = augmentor/m_Normalizer;

      hessian += augmentor;

      if ( (speedValue.GetNorm() > m_IntensityDifferenceThreshold) )
        {
          // compute least-square update using cholesky
          vnl_cholesky choleskySolver(hessian.GetVnlMatrix());
          vnl_vector<double> augmentedUpdate;
          choleskySolver.solve(tempUpdate, &augmentedUpdate);

          for( unsigned int j = 0; j < ImageDimension; j++ )
            {
              update[j] = static_cast<DeformationFieldRealType>(augmentedUpdate[j]);
            }
        }
      else
        {
          update.Fill( 0.0 );
        }
    }
  else
    // when there's rotation, we can't form a local hessian so we store the
    // values at this pixel in the system and solve later
    {
      update.Fill(0.0);

      long position = m_ForegroundRef->GetPixel(index);

      if(position < 0)
        {
          std::cerr << "Position < 0: " << position << std::endl;
          exit(EXIT_FAILURE);
        }

      const TensorType stdValue = ElementWisePowerTensor(varValue, 0.5);

      const TensorGradientType stdMatrix = InsertTensorsInTensorGradient(
          stdValue, stdValue, stdValue);

      // put contribution from image gradient in gradient
      m_GradLock.Lock();
      TensorGradientType normalizedGradient(element_quotient(usedGradient.GetVnlMatrix(), stdMatrix.GetVnlMatrix()));
      InsertTensorGradientIntoGradient(normalizedGradient, position*6, position*3);

      // also put contribution from distance term in gradient
      const double normalizerInvSqrt = 1.0/sqrt(m_Normalizer);

      for (unsigned int i=0; i < 3; i++)
        {
          const unsigned long numCols = gmm::mat_ncols(*m_Grad);
          (*m_Grad)(2*numCols+3*position+i, 3*position+i) = normalizerInvSqrt;
        }
      m_GradLock.Unlock();

      // put contribution from image gradient in residual
      m_ResidualLock.Lock();
      const TensorType normalizedResidual = ElementWiseDivideTensor(speedValue, stdValue);
      InsertTensorIntoResidual(normalizedResidual, position*6);
      m_ResidualLock.Unlock();

      // for indexing and weighting neighborhood
      int neighborIndexShifts[2];
      neighborIndexShifts[0] = 1;
      neighborIndexShifts[1] = -1;

      DeformationFieldRealType neighborFactors[2];
      neighborFactors[0] = 1;
      neighborFactors[1] = -1;

      if(this->m_UseRotationType == ApproxRotation && this->m_UseGradientType != Fixed)
        {
          IndexType tmpIndex = index;

          // iterate through 6 neighborhood to find local values of
          TensorType fixed;
          TensorType moving;
          TensorType speed;
          TensorType variance;
          TensorType standardDeviation;

          TensorType derivs[3];
          TensorGradientType gradient;

          m_GradLock.Lock();
          for (unsigned int j=0; j < 3; j++) // xyz (nhood voxel dimensions) - image locations
            {
              for (unsigned int k = 0; k < 2; k++) // +ve, -ve (nhood movement)

                {
                  tmpIndex[j] += neighborIndexShifts[k];

                  if((index[j] == LastIndex[j]-1) || (index[j] == FirstIndex[j]))
                    {
                      fixed = NumericTraits<TensorType>::Zero;
                      moving = NumericTraits<TensorType>::Zero;
                      speed = NumericTraits<TensorType>::Zero;

                      variance = NumericTraits<TensorType>::Zero;
                      standardDeviation = NumericTraits<TensorType>::Zero;
                    }
                  else
                    {
                      fixed = this->GetFixedImage()->GetPixel( tmpIndex );
                      moving = m_MovingImageWarper->GetOutput()->GetPixel( tmpIndex );
                      speed = fixed - moving;

                      variance = ElementWisePowerTensor(speed, 2);
                      for(unsigned int row = 0; row < TensorType::NDegreesOfFreedom; row++)
                        {
                          if(variance.GetNthComponent(row) < m_TensorMinimumVariance)
                            {
                              variance.SetNthComponent(row, m_TensorMinimumVariance);
                            }
                        }
                      standardDeviation = ElementWisePowerTensor(variance, 0.5);
                    }

                  // take derivatives wrt directions
                  for (unsigned int i = 0; i < 3; i++) // uvw (xyz directions for gradient deriv) - warp directions
                    {
                      // why if fixed is zero, deriv is zero?
                      if( i == j || fixed == NumericTraits<TensorType>::Zero ||
                          moving == NumericTraits<TensorType>::Zero)
                        {
                          derivs[i] = NumericTraits<TensorType>::Zero;
                        }
                      else
                        {
                          TensorMatrixInternalType f(0.0);

                          double step = -0.5*neighborFactors[k] / m_FixedImageSpacing[j];

                          for (unsigned int l = 0; l < 3; l++)
                            {
                              f[j][l] = moving.GetComponent(i, l) * step;
                              f[i][l] = moving.GetComponent(j, l) * -step;
                            }

                          TensorType rotationGradient;
                          rotationGradient.SetVnlMatrix(f + f.transpose());
                          rotationGradient = rotationGradient/2;

                          derivs[i] = ElementWiseDivideTensor(rotationGradient,
                              standardDeviation);
                        }
                    }

                  if ((tmpIndex[j] < LastIndex[j]) && (tmpIndex[j] >= FirstIndex[j]))
                    {
                      long tmpPosition = m_ForegroundRef->GetPixel(tmpIndex);
                      if(tmpPosition >= 0)
                        {
                          gradient = InsertTensorsInTensorGradient(derivs[0],
                              derivs[1], derivs[2]);

                          InsertTensorGradientIntoGradient(gradient, 6*tmpPosition, 3*position);
                        }
                    }
                  tmpIndex[j] -= neighborIndexShifts[k];
                }
            }
          m_GradLock.Unlock();
        }

      else if(this->m_UseRotationType == Rotation && this->m_UseGradientType != Fixed)
        {
          IndexType tmpIndex = index;

          // notation is complicated - see the appendix of the DT-REFinD paper

          // precompute derivatives of rotation wrt Jacobian components
          DeformationFieldMatrixType dRdJ[3][3];

          // prefactor = r' (tr(S)I - S)^{-1} r
          DeformationFieldRealType traceS = 0;
          for (unsigned int row=0; row < TensorType::Dimension; row++)
            {
              traceS += JJtsqrt(row, row);
            }
          DeformationFieldMatrixType prefactor;
          prefactor.SetIdentity();
          prefactor = prefactor * traceS - JJtsqrt;
          prefactor = DeformationFieldMatrixType(prefactor.GetInverse());
          prefactor = DeformationFieldMatrixType(R.GetTranspose())*prefactor*R;

          DeformationFieldMatrixType Rt = DeformationFieldMatrixType(
              R.GetTranspose());

          for (unsigned int i = 0; i < 3; i++) // uvw
            {
              DeformationFieldVectorType xJ;
              xJ[0] = 0; xJ[1] = Rt(2, i); xJ[2] = -Rt(1, i);
              dRdJ[i][0] = R * -1.0 * CreateCrossProductMatrix(prefactor * xJ);

              DeformationFieldVectorType yJ;
              yJ[0] = -Rt(2, i); yJ[1] = 0; yJ[2] = Rt(0, i);
              dRdJ[i][1] = R * -1.0 * CreateCrossProductMatrix(prefactor * yJ);

              DeformationFieldVectorType zJ;
              zJ[0] = Rt(1, i); zJ[1] = -Rt(0, i); zJ[2] = 0;
              dRdJ[i][2] = R * -1.0 * CreateCrossProductMatrix(prefactor * zJ);
            }

          // precompute constant term in gradient
          DeformationFieldMatrixType M =
            CastMatrix<TensorMatrixType, DeformationFieldMatrixType>(
                TensorMatrixType(movingValue.GetVnlMatrix()));
          DeformationFieldMatrixType RtM = Rt * M;

          // now find and store local gradient contributions in neighborhood
          DeformationFieldMatrixType neighborJ;

          DeformationFieldMatrixType dR;
          DeformationFieldMatrixType RtMdR;
          TensorMatrixType gradMatrix;

          TensorType derivs[3];
          TensorGradientType gradient;

          m_GradLock.Lock();
          for (unsigned int j = 0; j < 3; j++) // xyz

            {
              for (unsigned int k = 0; k < 2; k++) // +ve, -ve

                {
                  tmpIndex[j] += neighborIndexShifts[k];

                  if ( ( (tmpIndex[j] < LastIndex[j] && k==0) ||
                      (tmpIndex[j] >= FirstIndex[j] && k==1) ) &&
                      !zeroFixedImage && !zeroMovingImage)
                    {
                      neighborJ = m_Jacobian->GetPixel(tmpIndex);

                      // take derivatives wrt directions
                      for (unsigned int i = 0; i < 3; i++) // xyz

                        {
                          dR.Fill(0.0);
                          for (unsigned int l = 0; l < 3; l++) // uvw

                            {
                              dR += dRdJ[l][j] * neighborJ(l, i);
                            }
                          dR = dR / (2.0 * neighborFactors[k] *
                              this->m_FixedImageSpacing[j]);

                          RtMdR = RtM * dR;
                          gradMatrix = CastMatrix<
                          DeformationFieldMatrixType, TensorMatrixType>(
                              DeformationFieldMatrixType(RtMdR.GetTranspose()
                                  + RtMdR.GetVnlMatrix()));

                          derivs[i].SetVnlMatrix(gradMatrix.GetVnlMatrix());
                          derivs[i] = ElementWiseDivideTensor(derivs[i], stdValue);
                        }

                      gradient = InsertTensorsInTensorGradient(derivs[0],
                          derivs[1], derivs[2]);

                      long tmpPosition = m_ForegroundRef->GetPixel(tmpIndex);
                      if(tmpPosition >= 0)
                        {
                          InsertTensorGradientIntoGradient(gradient, 6*position, 3*tmpPosition);
                        }
                    }
                  tmpIndex[j] -= neighborIndexShifts[k];
                }
            }
          m_GradLock.Unlock();
        }
      else
        {
          if(m_UseGradientType != Fixed && m_UseRotationType != NoRotation)
            {
              std::cout<<"Should be fixed gradient type or no rotation"<<std::endl;
              exit(EXIT_FAILURE);
            }
        }
    }

  // WARNING!! We compute the global data without taking into account the
  // current update step. There are several reasons for that:
  // If an exponential, a smoothing or any other operation is applied on the
  // update field, we cannot compute the newMappedCenterPoint here; and even
  // if we could, this would be an often unnecessary time-consuming task.
  if(!zeroFixedImage && !zeroMovingImage)
    {
      //only update pixels which are both non background
      if ( globalData )
        {
          globalData->m_SumOfSquaredDifference += speedValue.GetSquaredNorm();
          globalData->m_NumberOfPixelsProcessed += 1;
          globalData->m_SumOfSquaredChange += update.GetSquaredNorm();
        }
    }

  return update;
}

/**
 * Solves system and grafts result to pointer.
 */
template < class TFixedImage, class TMovingImage, class TDeformationField,
class TSolverPrecision >
void
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::SolveUpdate(
    DeformationFieldTypePointer updateField)
{

  // for timing
  time_t start, final;
  double diff;

  long numCols = gmm::mat_ncols(*m_Grad);
  long numRows = gmm::mat_nrows(*m_Grad);

  std::cout << "Matrix is " << numRows << "x" << numCols << std::endl;

  std::cout << "Setting up GMM system..." << std::flush;
  time (&start);

  gmm::iteration iter(0.005);
  iter.set_maxiter(100);

  SolverReadMatrixType* m_GradRead = new SolverReadMatrixType(numRows, numCols);
  gmm::clean(*m_Grad, 1E-8);
  gmm::copy(*m_Grad, *m_GradRead);
  delete m_Grad;

  SolverReadMatrixType* AtA = 0;
  SolverVectorType* Atb = 0;

  double rhsNorm, operatorNorm;

  if (GetPreconditionSolver())
    {
      // gmm/itl only allows preconditioning on general cgs, so we need to
      // form the normal equations manually
      SolverWriteMatrixType* writeAtA = new SolverWriteMatrixType(numCols, numCols);
      gmm::mult( gmm::transposed(*m_GradRead), *m_GradRead, *writeAtA);
      AtA = new SolverReadMatrixType(numCols, numCols);
      gmm::clean(*writeAtA, 1E-8);
      gmm::copy(*writeAtA, *AtA);

      delete writeAtA;

      Atb = new SolverVectorType(numCols, 0.0);
      gmm::mult( gmm::transposed(*m_GradRead), *m_Residual, *Atb);

      delete m_GradRead;
      delete m_Residual;

      rhsNorm = gmm::vect_norm2(*Atb);
      operatorNorm = gmm::mat_euclidean_norm(*AtA);
    }
  else
    {
      rhsNorm = gmm::vect_norm2(*m_Residual);
      operatorNorm = gmm::mat_euclidean_norm(*m_GradRead);
    }

  final = time(&final);
  diff = difftime(final, start);
  std::cout << "Done in " << diff << " seconds." << std::endl;

  // initialize solution with last update (not sure we should do this,
  // but not much empirical difference?)
  SolverVectorType* solution = new SolverVectorType(numCols, 0.0);
  typedef typename itk::ImageRegionConstIterator< LongImageType > LongIteratorType;
  LongIteratorType longIt = LongIteratorType(m_ForegroundRef,
      m_ForegroundRef->GetLargestPossibleRegion());

  typedef typename itk::ImageRegionIterator< DeformationFieldType > vfIteratorType;
  vfIteratorType vfIt = vfIteratorType(updateField, updateField->GetLargestPossibleRegion());

  DeformationFieldVectorType displacement;
  for(longIt.GoToBegin(), vfIt.GoToBegin(); !longIt.IsAtEnd(); ++longIt, ++vfIt)
    {
      if(longIt.Get() >= 0)
        {
          displacement = vfIt.Get();

          (*solution)[longIt.Get()*3 + 0] = displacement[0];
          (*solution)[longIt.Get()*3 + 1] = displacement[1];
          (*solution)[longIt.Get()*3 + 2] = displacement[2];

          vfIt.Set(displacement);
        }
    }

  // use conjugate gradients on the normal equations to solve the system
  std::cout << "Solving GMM system..." << std::flush;
  time (&start);

  if (GetPreconditionSolver())
    {
      // gmm only allows preconditioning on general cgs, so we need to
      // form the normal equations manually
      gmm::identity_matrix I;
      gmm::diagonal_precond<SolverReadMatrixType> P(*AtA);
      gmm::cg(*AtA, *solution, *Atb, I, P, iter);

//      gmm::identity_matrix P;
//      gmm::bicgstab(*AtA, *solution, *Atb, P, iter);
      delete AtA;
      delete Atb;

    }
  else
    {
      // gmm allows unpreconditioned cgs on the normal equations,
      // without needing to form them
      gmm::least_squares_cg(*m_GradRead, *solution, *m_Residual, iter);

      delete m_GradRead;
      delete m_Residual;
    }

  final = time(&final);
  diff = difftime(final, start);
  std::cout << "Done in " << diff << " seconds." << std::endl;

  std::cout << "Iter: "<< iter.get_iteration() <<", ";
  std::cout << "Res: "<< iter.get_res() <<", ";
  std::cout << "Rhsn: "<< rhsNorm <<", ";
  std::cout << "An: " << operatorNorm << ", ";
  std::cout << "Fraction: " << iter.get_res()/rhsNorm << std::endl;

  // put the solution in the deformation field
  for(longIt.GoToBegin(), vfIt.GoToBegin(); !longIt.IsAtEnd(); ++longIt, ++vfIt)
    {
      if(longIt.Get() >= 0)
        {
          displacement[0] = (*solution)[longIt.Get()*3 + 0];
          displacement[1] = (*solution)[longIt.Get()*3 + 1];
          displacement[2] = (*solution)[longIt.Get()*3 + 2];
        }
      else
        {
          displacement[0] = 0;
          displacement[1] = 0;
          displacement[2] = 0;
        }
      vfIt.Set(displacement);
    }

  delete solution;
}

/** Insert a single tensor into the residual in the system to solve */
template < class TFixedImage, class TMovingImage, class TDeformationField,
class TSolverPrecision >
void
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::InsertTensorIntoResidual(
    const TensorType t, unsigned long startPosition)
{
  (*m_Residual)[startPosition + 0] = t.GetNthComponent(0);
  (*m_Residual)[startPosition + 1] = sqrt(2.0) * t.GetNthComponent(1);
  (*m_Residual)[startPosition + 2] = t.GetNthComponent(2);
  (*m_Residual)[startPosition + 3] = sqrt(2.0) * t.GetNthComponent(3);
  (*m_Residual)[startPosition + 4] = sqrt(2.0) * t.GetNthComponent(4);
  (*m_Residual)[startPosition + 5] = t.GetNthComponent(5);
}

/** Insert a single tensor gradient into the sparse matrix in the system to
 * solve */
template < class TFixedImage, class TMovingImage, class TDeformationField,
class TSolverPrecision >
void
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::InsertTensorGradientIntoGradient(
    const TensorGradientType grad, unsigned long startRow,
    unsigned long startCol)
{
  for (int i = 0; i < 3; i++)
    {
      (*m_Grad)(startRow + 0, startCol + i) = grad[0][i];
      (*m_Grad)(startRow + 1, startCol + i) = sqrt(2.0) * grad[1][i];
      (*m_Grad)(startRow + 2, startCol + i) = grad[2][i];
      (*m_Grad)(startRow + 3, startCol + i) = sqrt(2.0) * grad[3][i];
      (*m_Grad)(startRow + 4, startCol + i) = sqrt(2.0) * grad[4][i];
      (*m_Grad)(startRow + 5, startCol + i) = grad[5][i];
    }
}

/*
 * Update the metric and release the per-thread-global data.
 */
template < class TFixedImage, class TMovingImage, class TDeformationField,
class TSolverPrecision >
void
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::ReleaseGlobalDataPointer(void *gd) const
{
  GlobalDataStruct * globalData = (GlobalDataStruct *) gd;

  m_MetricCalculationLock.Lock();
  m_SumOfSquaredDifference += globalData->m_SumOfSquaredDifference;
  m_NumberOfPixelsProcessed += globalData->m_NumberOfPixelsProcessed;
  m_SumOfSquaredChange += globalData->m_SumOfSquaredChange;

  if ( m_NumberOfPixelsProcessed )
    {
      m_Metric = m_SumOfSquaredDifference
      / static_cast<double> (m_NumberOfPixelsProcessed);
      m_RMSChange = vcl_sqrt(m_SumOfSquaredChange
          / static_cast<double> (m_NumberOfPixelsProcessed));
    }
  m_MetricCalculationLock.Unlock();

  delete globalData;
}

/** Element wise power of a tensor. */
template < class TFixedImage, class TMovingImage, class TDeformationField,
class TSolverPrecision >
typename ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::TensorType
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::ElementWisePowerTensor(
    const TensorType tensor, double power)
{
  TensorType outTensor;

  for (unsigned int i = 0; i < TensorType::NDegreesOfFreedom; i++)
    {
      outTensor.SetNthComponent(i, std::pow( (double)tensor.GetNthComponent(i), power));
    }

  return outTensor;
}

/** Element wise multiplication of a tensor by a another. */
template < class TFixedImage, class TMovingImage, class TDeformationField,
class TSolverPrecision >
typename ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::TensorType
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::ElementWiseMultiplyTensor(
    const TensorType tensor1, const TensorType tensor2)
{
  TensorType outTensor;

  for (unsigned int i = 0; i < TensorType::NDegreesOfFreedom; i++)
    {
      outTensor.SetNthComponent(i, tensor1.GetNthComponent(i)
          * tensor2.GetNthComponent(i));
    }

  return outTensor;
}

/** Element wise division of a tensor by a another. */
template < class TFixedImage, class TMovingImage, class TDeformationField,
class TSolverPrecision >
typename ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::TensorType
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::ElementWiseDivideTensor(
    const TensorType tensor1, const TensorType tensor2)
{
  TensorType outTensor;

  for (unsigned int i = 0; i < TensorType::NDegreesOfFreedom; i++)
    {
      outTensor.SetNthComponent(i, tensor1.GetNthComponent(i)
          / tensor2.GetNthComponent(i));
    }

  return outTensor;
}

/** Creates the 'cross product' operating matrix of a vector. */
template < class TFixedImage, class TMovingImage, class TDeformationField,
class TSolverPrecision >
typename ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::DeformationFieldMatrixType
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::CreateCrossProductMatrix(
    const DeformationFieldVectorType v)
{
  DeformationFieldMatrixType m;

  m(0, 0) = 0;
  m(0, 1) = -v[2];
  m(0, 2) = v[1];

  m(1, 0) = -m(0, 1);
  m(1, 1) = 0;
  m(1, 2) = -v[0];

  m(2, 0) = -m(0, 2);
  m(2, 1) = -m(1, 2);
  m(2, 2) = 0;

  return m;
}

/**
 * Performs reorientation on a tensor gradient.
 */
template < class TFixedImage, class TMovingImage, class TDeformationField,
class TSolverPrecision >
typename ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::TensorGradientType
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::ReorientateMovingGradient(
    const DeformationFieldMatrixType R,
    const TensorGradientType movingGradient)
{
  TensorGradientType output;
  TensorType T;

  for (unsigned int col = 0; col < TensorGradientType::ColumnDimensions; col++)
    {
      // first copy onto tensor
      for (unsigned int row = 0; row < TensorGradientType::RowDimensions; row++)
        {
          T.SetNthComponent(row, movingGradient[row][col]);
        }

      // then rotate with RTranspose because ApplyMatrix does R*T*R'
      T.SetVnlMatrix(T.ApplyMatrix(R.GetTranspose()));

      // then copy to output
      for (unsigned int row = 0; row < TensorGradientType::RowDimensions; row++)
        {
          output[row][col] = T.GetNthComponent(row);
        }
    }
  return output;
}

/**
 * Creates a nx3 matrix with a tensor (with n degrees of freedom) in each column.
 */
template < class TFixedImage, class TMovingImage, class TDeformationField,
class TSolverPrecision >
typename ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::TensorGradientType
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::InsertTensorsInTensorGradient(
    const TensorType tensor0, const TensorType tensor1,
    const TensorType tensor2)
{
  TensorGradientType tensorGradient;

  for (unsigned int i = 0; i < TensorType::NDegreesOfFreedom; i++)
    {
      tensorGradient[i][0] = tensor0.GetNthComponent(i);
      tensorGradient[i][1] = tensor1.GetNthComponent(i);
      tensorGradient[i][2] = tensor2.GetNthComponent(i);
    }

  return tensorGradient;
}

template < class TFixedImage, class TMovingImage, class TDeformationField,
class TSolverPrecision >
typename ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::TensorGradientType
ESMDemonsRegistrationTensorFunction<TFixedImage, TMovingImage,
TDeformationField, TSolverPrecision>::ComputeMovingGradient(
    const TensorType movingValue, IndexType index,
    const IndexType firstIndex, const IndexType lastIndex)
{
  // we don't use a CentralDifferenceImageFunction here to be able to
  // check for NumericTraits<MovingPixelType>::Zero()

  MovingImagePixelType movingPixValue;
  TensorGradientType movingGradient;
  IndexType tmpIndex = index;

  for (unsigned int dim = 0; dim < ImageDimension; dim++)
    {
      // bounds checking
      if ( firstIndex[dim] == lastIndex[dim] || index[dim] < firstIndex[dim]
                                                                        || index[dim] >= lastIndex[dim] )
        {
          for (int row = 0; row < MovingImagePixelType::DegreesOfFreedom; row++)
            {
              movingGradient[row][dim] = 0.0;
            }
          continue;
        }
      else if ( index[dim] == firstIndex[dim] )
        {
          // compute derivative
          tmpIndex[dim] += 1;

          if ( this->m_UseRotationType == Rotation )
            {
              movingPixValue
              = m_MovingImageWOReorientationWarper->GetOutput()->GetPixel(
                  tmpIndex);
            }
          else
            {
              movingPixValue = m_MovingImageWarper->GetOutput()->GetPixel(
                  tmpIndex);
            }

          if ( movingPixValue == NumericTraits<MovingImagePixelType>::Zero )
            {
              // weird crunched border case
              for (int row = 0; row < TensorType::DegreesOfFreedom; row++)
                {
                  movingGradient[row][dim] = 0.0;
                }
            }
          else
            {
              // forward difference
              const TensorType diff =
                static_cast<TensorType> (movingPixValue) - movingValue;

              for (int row = 0; row < TensorType::DegreesOfFreedom; row++)
                {
                  movingGradient[row][dim] = diff.GetNthComponent(row)
                  / m_FixedImageSpacing[dim];
                }
            }
          tmpIndex[dim] -= 1;
          continue;
        }
      else if ( index[dim] == (lastIndex[dim] - 1) )
        {
          // compute derivative
          tmpIndex[dim] -= 1;
          if ( this->m_UseRotationType == Rotation )
            {
              movingPixValue
              = m_MovingImageWOReorientationWarper->GetOutput()->GetPixel(
                  tmpIndex);
            }
          else
            {
              movingPixValue = m_MovingImageWarper->GetOutput()->GetPixel(
                  tmpIndex);
            }

          if ( movingPixValue == NumericTraits<MovingImagePixelType>::Zero )
            {
              // weird crunched border case
              for (int row = 0; row < TensorType::DegreesOfFreedom; row++)
                {
                  movingGradient[row][dim] = 0.0;
                }
            }
          else
            {
              // backward difference
              const TensorType diff = movingValue
              - static_cast<TensorType> (movingPixValue);
              for (int row = 0; row < TensorType::DegreesOfFreedom; row++)
                {
                  movingGradient[row][dim] = diff.GetNthComponent(row)
                  / m_FixedImageSpacing[dim];
                }
            }
          tmpIndex[dim] += 1;
          continue;
        } //End Special Boundary case


      // compute derivative
      tmpIndex[dim] += 1;
      if ( this->m_UseRotationType == Rotation )
        {
          movingPixValue
          = m_MovingImageWOReorientationWarper->GetOutput()->GetPixel(
              tmpIndex);
        }
      else
        {
          movingPixValue = m_MovingImageWarper->GetOutput()->GetPixel(
              tmpIndex);
        }

      if ( movingPixValue == NumericTraits<MovingImagePixelType>::Zero )
        {
          // backward difference
          tmpIndex[dim] -= 2;
          if ( this->m_UseRotationType == Rotation )
            {
              movingPixValue
              = m_MovingImageWOReorientationWarper->GetOutput()->GetPixel(
                  tmpIndex);
            }
          else
            {
              movingPixValue = m_MovingImageWarper->GetOutput()->GetPixel(
                  tmpIndex);
            }
          if ( movingPixValue == NumericTraits<MovingImagePixelType>::Zero )
            {
              // weird crunched border case
              for (int row = 0; row < TensorType::DegreesOfFreedom; row++)
                {
                  movingGradient[row][dim] = 0.0;
                }
            }
          else
            {
              // backward difference
              const TensorType diff = movingValue
              - static_cast<TensorType> (movingPixValue);
              for (int row = 0; row < TensorType::DegreesOfFreedom; row++)
                {
                  movingGradient[row][dim] = diff.GetNthComponent(row)
                  / m_FixedImageSpacing[dim];
                }
            }
        }
      else
        {
          tmpIndex[dim] -= 2;
          TensorType movingPixValue2;
          if ( this->m_UseRotationType == Rotation )
            {
              movingPixValue2
              = m_MovingImageWOReorientationWarper->GetOutput()->GetPixel(
                  tmpIndex);
            }
          else
            {
              movingPixValue2 = m_MovingImageWarper->GetOutput()->GetPixel(
                  tmpIndex);
            }

          if ( movingPixValue2 == NumericTraits<MovingImagePixelType>::Zero )
            {
              // forward difference
              const TensorType diff =
                static_cast<TensorType> (movingPixValue) - movingValue;
              for (int row = 0; row < TensorType::DegreesOfFreedom; row++)
                {
                  movingGradient[row][dim] = diff.GetNthComponent(row)
                  / m_FixedImageSpacing[dim];
                }
            }
          else
            {
              // normal case, central difference
              const TensorType diff =
                static_cast<TensorType> (movingPixValue)
                - static_cast<TensorType> (movingPixValue2);
              for (int row = 0; row < TensorType::DegreesOfFreedom; row++)
                {
                  movingGradient[row][dim] = 0.5 * diff.GetNthComponent(row)
                  / m_FixedImageSpacing[dim];
                }
            }
        }
      tmpIndex[dim] += 1;

    }// end of for loop

  return movingGradient;
}

} // end namespace itk

#endif
