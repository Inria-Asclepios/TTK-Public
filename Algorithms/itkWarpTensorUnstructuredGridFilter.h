/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkWarpTensorUnstructuredGridFilter_h
#define __itkWarpTensorUnstructuredGridFilter_h

#include "itkProcessObject.h"
#include "itkVectorLinearInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkTensorLinearInterpolateImageFunction.h"
#include "itkWarpJacobianFilter.h"
#include "itkPoint.h"
#include "itkFixedArray.h"
#include "itkTensor.h"

#include <vector>

namespace itk
{

template <
  class TDisplacementField
  >
class ITK_EXPORT WarpTensorUnstructuredGridFilter :
    public ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef WarpTensorUnstructuredGridFilter      Self;
  typedef ProcessObject Superclass;
  typedef SmartPointer<Self>         Pointer;
  typedef SmartPointer<const Self>   ConstPointer;

  enum ReorientationType { FS, PPD };

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro( WarpTensorUnstructuredGridFilter, ProcessObject );

  /** Typedef to describe the region type. */
  typedef typename TDisplacementField::RegionType RegionType;

  /** Inherit some types from the superclass. */
  typedef TDisplacementField                           DisplacementFieldType;
  typedef typename DisplacementFieldType::Pointer      DisplacementFieldPointer;
  typedef typename DisplacementFieldType::ConstPointer DisplacementFieldConstPointer;
  typedef typename DisplacementFieldType::SpacingType  SpacingType;
  typedef typename DisplacementFieldType::IndexType    IndexType;


  
  /** Determine the image dimension. */
  itkStaticConstMacro(DisplacementFieldDimension, unsigned int,
                      TDisplacementField::ImageDimension );

  itkStaticConstMacro(VectorDimension, unsigned int,
	  TDisplacementField::PixelType::Dimension);

  typedef Matrix<double, DisplacementFieldDimension, VectorDimension> JacobianType;
  typedef Image<JacobianType, DisplacementFieldDimension>             JacobianImageType;
  typedef typename JacobianImageType::Pointer             JacobianPointer;
  typedef WarpJacobianFilter<TDisplacementField, JacobianImageType>  JacobianFilterType;

  /** Interpolator typedef support. */
  typedef VectorLinearInterpolateImageFunction<DisplacementFieldType,double>   DisplacementInterpolatorType;
  typedef typename DisplacementInterpolatorType::Pointer   DisplacementInterpolatorPointer;
/*   typedef LinearInterpolateImageFunction<JacobianImageType,double>   JacobianInterpolatorType; */
/*   typedef typename JacobianInterpolatorType::Pointer   JacobianInterpolatorPointer; */
  
  itkGetObjectMacro(Jacobian, JacobianImageType);

  /** Displacement field typedef support. */
  typedef typename DisplacementFieldType::PixelType DisplacementType;
  typedef typename DisplacementFieldType::PointType PointType;

  typedef Tensor<double, VectorDimension> TensorType;
  typedef TensorType PixelType;
  typedef std::vector<TensorType> TensorArrayType;
  typedef std::vector<PointType> PointArrayType;

  /** Interpolator typedef support. */
  typedef double CoordRepType;

  /** Type for representing the direction of the output image */
  typedef typename DisplacementFieldType::DirectionType     DirectionType;

  itkGetObjectMacro (DisplacementField, DisplacementFieldType);
  itkSetObjectMacro (DisplacementField, DisplacementFieldType);
  itkGetObjectMacro (InverseDisplacementField, DisplacementFieldType);
  itkSetObjectMacro (InverseDisplacementField, DisplacementFieldType);  

  /** Set the edge padding value */
  itkSetMacro( EdgePaddingValue, PixelType );

  /** Get the edge padding value */
  itkGetMacro( EdgePaddingValue, PixelType );
  
    /** Set the type of tensor reorientation */
  itkSetMacro( ReorientationStrategy, ReorientationType );

  /** Get the edge padding value */
  itkGetMacro( ReorientationStrategy, ReorientationType );

  /** WarpTensorUnstructuredGridFilter produces an image which is a different
   * size than its input image. As such, it needs to provide an
   * implemenation for GenerateOutputInformation() which set
   * the output information according the OutputSpacing, OutputOrigin
   * and the deformation field's LargestPossibleRegion. */
  virtual void GenerateOutputInformation();

  /** It is difficult to compute in advance the input image region
   * required to compute the requested output region. Thus the safest
   * thing to do is to request for the whole input image.
   *
   * For the deformation field, the input requested region
   * set to be the same as that of the output requested region. */
  virtual void GenerateInputRequestedRegion();


  virtual void SetPoints (PointArrayType arg)
  { m_Points = arg; }
  virtual void SetTensors (TensorArrayType arg)
  { m_Tensors = arg; }

  PointArrayType GetOutputPoints (void)
  { return m_OutputPoints; }
  TensorArrayType GetOutputTensors (void)
  { return m_OutputTensors; }

protected:
  WarpTensorUnstructuredGridFilter();
  ~WarpTensorUnstructuredGridFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** WarpTensorUnstructuredGridFilter is implemented as a filter.
   * As such, it needs to provide and implementation for 
   * GenerateData(). */
  void GenerateData();

private:
  WarpTensorUnstructuredGridFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  PixelType                  m_EdgePaddingValue;

  typename JacobianImageType::Pointer m_Jacobian;
  
  ReorientationType m_ReorientationStrategy;

  DisplacementInterpolatorPointer m_InverseDisplacementInterpolator;
/*   JacobianInterpolatorPointer m_JacobianInterpolator; */
  
  PointArrayType m_Points;
  PointArrayType m_OutputPoints;
  TensorArrayType m_Tensors;
  TensorArrayType m_OutputTensors;

  DisplacementFieldPointer m_DisplacementField;
  DisplacementFieldPointer m_InverseDisplacementField;
  
  
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkWarpTensorUnstructuredGridFilter.txx"
#endif

#endif
