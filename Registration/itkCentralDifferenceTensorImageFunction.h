#ifndef __itkCentralDifferenceTensorImageFunction_h
#define __itkCentralDifferenceTensorImageFunction_h

#include "itkImageFunction.h"
#include "itkMatrix.h"
#include "itkImageBase.h"
#include "itkTensor.h"

namespace itk
{

/**
 * Due to a bug in MSVC, an enum value cannot be accessed out of a template
 * parameter until the template class opens.  In order for templated classes
 * to access the dimension of a template parameter in defining their
 * own dimension, this class is needed as a work-around.
 */


template <typename T>
struct GetNDegreesOfFreedom
{
  itkStaticConstMacro(DegreesOfFreedom, unsigned int, T::DegreesOfFreedom);
};


/**
 * \class VectorCentralDifferenceImageFunction
 * \brief Calculate the derivative by central differencing.
 *
 * This class is templated over the input image type and
 * the coordinate representation type (e.g. float or double).
 *
 * Possible improvements:
 * - the use of Neighborhood operators may improve efficiency.
 *
 * This class was contributed by Tom Vercauteren, INRIA
 *
 * \ingroup ImageFunctions
 */
template <
  class TInputImage,
  class TCoordRep = float >
class ITK_EXPORT CentralDifferenceTensorImageFunction :
  public ImageFunction< TInputImage,
                        Matrix<double, \
                        ::itk::GetNDegreesOfFreedom<typename TInputImage::PixelType>::DegreesOfFreedom, \
                        ::itk::GetImageDimension<TInputImage>::ImageDimension>,
                        TCoordRep >
{
public:
  typedef typename TInputImage::PixelType InputPixelType;

  /** Extract the vector dimension from the pixel template parameter. */
  itkStaticConstMacro(Dimension, unsigned int,
                      InputPixelType::Dimension);

  /* Define Tensor Degree of Freedom */
  itkStaticConstMacro(DegreeOfFreedom, unsigned int,
                      InputPixelType::Dimension*(InputPixelType::Dimension+1)/2);

  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Standard class typedefs. */
  typedef CentralDifferenceTensorImageFunction Self;
  typedef ImageFunction<TInputImage,
                        Matrix<double,itkGetStaticConstMacro(Dimension)*(itkGetStaticConstMacro(Dimension)+1)/2,
                               itkGetStaticConstMacro(ImageDimension)>,
                        TCoordRep>       Superclass;
  typedef SmartPointer<Self>             Pointer;
  typedef SmartPointer<const Self>       ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(CentralDifferenceTensorImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage InputImageType;

  /** OutputType typdef support. */
  typedef typename Superclass::OutputType OutputType;

  /** Index typedef support. */
  typedef typename Superclass::IndexType IndexType;

  /** ContinuousIndex typedef support. */
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

  /** Point typedef support. */
  typedef typename Superclass::PointType PointType;

  /** Evalulate the image derivative by central differencing at specified index.
   *
   *  No bounds checking is done.
   *  The point is assume to lie within the image buffer.
   *
   *  ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  virtual OutputType EvaluateAtIndex( const IndexType& index ) const;

  /** Evalulate the image derivative by central differencing at non-integer
   *  positions.
   *
   *  No bounds checking is done.
   *  The point is assume to lie within the image buffer.
   *
   *  ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
   virtual OutputType Evaluate( const PointType& point ) const
    {
    IndexType index;
    this->ConvertPointToNearestIndex( point, index );
    return this->EvaluateAtIndex( index );
    }
  virtual OutputType EvaluateAtContinuousIndex(
    const ContinuousIndexType& cindex ) const
    {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex( cindex, index );
    return this->EvaluateAtIndex( index );
    }

   itkSetMacro( UseImageDirection, bool );
   itkGetConstMacro( UseImageDirection, bool );
   itkBooleanMacro( UseImageDirection );
  
protected:
  CentralDifferenceTensorImageFunction();
  ~CentralDifferenceTensorImageFunction(){};
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  CentralDifferenceTensorImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  bool m_UseImageDirection;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCentralDifferenceTensorImageFunction.txx"
#endif

#endif
