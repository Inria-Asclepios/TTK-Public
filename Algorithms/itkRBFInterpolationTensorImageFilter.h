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
#ifndef _itk_RBFInterpolationTensorImageFilter_h_
#define _itk_RBFInterpolationTensorImageFilter_h_

#include <itkImageToImageFilter.h>
#include <vnl/vnl_matrix.h>


namespace itk
{
  
  template<class TInputImage, class TOutputImage>
    class ITK_EXPORT RBFInterpolationTensorImageFilter :
  public ImageToImageFilter<TInputImage, TOutputImage>
  {
    
  public:
    
    typedef RBFInterpolationTensorImageFilter            Self;
    typedef ImageToImageFilter<TInputImage,TOutputImage> Superclass;
    typedef SmartPointer<Self>                           Pointer;
    typedef SmartPointer<const Self>                     ConstPointer;

    itkNewMacro(Self);
    itkTypeMacro(RBFInterpolationTensorImageFilter,
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
    typedef vnl_matrix<ScalarType>               GeneralMatrixType;

    /** Set/Get the vector of tensors */
    void SetTensors( const VectorOfPixelType vec )
    { m_Tensors=vec; }
    VectorOfPixelType GetTensors(void) const
    {return m_Tensors;}
    
    /** Set/Get the vector of positions */
    void SetPoints(const VectorOfPointsType vec)
    { m_Points=vec; }
    VectorOfPointsType GetPoints(void) const 
    { return m_Points; }
    
    void SetTangents( VectorOfPointsType vec )
    { m_Tangents = vec; }
    VectorOfPointsType GetTangents(void) const
    { return m_Tangents; }

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
    
    RBFInterpolationTensorImageFilter()
    {
      m_Sigma = 1.0;
      m_Gamma = 1.0;
      m_DoNotDiffuseTangent = false;
      OutputPixelType m_Mean( static_cast<ScalarType>(0.0) ); // limit value == value at infinity
    }
    ~RBFInterpolationTensorImageFilter(){};
    
    /** Threaded implementation */
    void BeforeThreadedGenerateData(void);
    void ThreadedGenerateData(const OutputImageRegionType &,int);
    void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    }

    /** Internal methods */
    static OutputPixelType ComputeMeanOfTensors(const VectorOfPixelType&);
    static ScalarType h(const PointType&, const PointType&, const ScalarType& sigma, const ScalarType& gamma);
    

  private:
    
    VectorOfPixelType  m_Tensors;
    VectorOfPointsType m_Points;
    VectorOfPointsType m_Tangents;
    
    OutputPixelType m_Mean;

    GeneralMatrixType m_L;

    double m_Sigma; // sigma used for interpolation
    double m_Gamma;
    
    bool m_DoNotDiffuseTangent;

  };


} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkRBFInterpolationTensorImageFilter.txx"
#endif

#endif
