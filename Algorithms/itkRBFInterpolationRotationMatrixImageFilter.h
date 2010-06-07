/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL:$
  Language:  C++
  Date:      $Date:$
  Version:   $Revision:$

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_RBFInterpolationRotationMatrixTensorImageFilter_h_
#define _itk_RBFInterpolationRotationMatrixTensorImageFilter_h_

#include <itkImageToImageFilter.h>
#include <itkVector.h>
#include <vnl/vnl_matrix.h>

namespace itk
{

  template<class TInputImage, class TOutputImage>
    class ITK_EXPORT RBFInterpolationRotationMatrixImageFilter :
  public ImageToImageFilter<TInputImage, TOutputImage>
  {
    
  public:

    typedef RBFInterpolationRotationMatrixImageFilter     Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer<Self>                            Pointer;
    typedef SmartPointer<const Self>                      ConstPointer;

    itkNewMacro (Self);
    itkTypeMacro (RBFInterpolationRotationMatrixImageFilter,
                  ImageToImageFilter);
    itkStaticConstMacro(ImageDimension, unsigned int,
			TOutputImage::ImageDimension);

    /** Image typedefs */
    typedef TInputImage                          InputImageType;
    typedef TOutputImage                         OutputImageType;
    typedef typename InputImageType::PixelType   InputPixelType;
    typedef typename OutputImageType::PixelType  OutputPixelType;
    typedef typename OutputPixelType::ValueType  ScalarType;
    typedef typename OutputImageType::RegionType OutputImageRegionType;
    typedef typename OutputImageType::PointType  PointType;
    typedef typename OutputImageType::IndexType  IndexType;

        
    /** Specific typedefs */    
    typedef std::vector<OutputPixelType>         VectorOfPixelType;
    typedef std::vector<PointType>               VectorOfPointsType;
    typedef itk::Vector<ScalarType, 3>           VectorType;
    typedef vnl_matrix<ScalarType>               GeneralMatrixType;
    
    void SetRotationMatrices (const VectorOfPixelType vec )
    { m_RotationMatrices = vec; }
    VectorOfPixelType GetRotationMatrices (void) const
    { return m_RotationMatrices; }

    /** Set/Get the vector of positions */
    void SetPoints(const VectorOfPointsType vec)
    { m_Points=vec; }
    VectorOfPointsType GetPoints(void) const 
    { return m_Points; }

    /** Set/Get the parameter sigma */
    itkSetMacro(Sigma,double);
    itkGetMacro(Sigma,double);
    /** Set/Get the parameter gamma */
    itkSetMacro(Gamma,double);
    itkGetMacro(Gamma,double);
  
    
    /** Directly give the interpolated value given a list of positions */
    static VectorOfPixelType InterpolateValuesAt(const VectorOfPixelType&,
                                                 const VectorOfPointsType&,
                                                 const VectorOfPointsType&,
                                                 const ScalarType&, const ScalarType&);
    
    
  protected:
    RBFInterpolationRotationMatrixImageFilter()
    {
      m_Sigma = 1.0;
      m_Gamma = 1.0;
    };
    ~RBFInterpolationRotationMatrixImageFilter(){};

    /** Threaded implementation */
    void BeforeThreadedGenerateData(void);
    void ThreadedGenerateData(const OutputImageRegionType &,int);
    void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    }

    /** Internal methods */
    static OutputPixelType ComputeMeanOfRotationMatrices(const VectorOfPixelType&);
    static ScalarType h(const PointType&, const PointType&, const ScalarType& sigma, const ScalarType& gamma);

    static VectorType      RotationMatrixToVector (const OutputPixelType& );
    static OutputPixelType VectorToRotationMatrix (const VectorType& );
    
  private:

    VectorOfPixelType  m_RotationMatrices;
    VectorOfPointsType m_Points;

    OutputPixelType   m_Mean;
    
    GeneralMatrixType m_L;

    double m_Sigma; // sigma used for interpolation
    double m_Gamma;
    
  };
  


} // end of namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkRBFInterpolationRotationMatrixImageFilter.txx"
#endif

#endif
