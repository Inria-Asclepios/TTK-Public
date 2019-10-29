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
#ifndef _itk_NormalizedGaussianInterpolationTensorImageFilter_h_
#define _itk_NormalizedGaussianInterpolationTensorImageFilter_h_

#include <itkImageToImageFilter.h>
#include <itkImage.h>
#include <itkPoint.h>


namespace itk
{

  /**
     * \ Class NormalizedGaussianInterpolationTensorImageFilter
     *
     * Author: Pierre Fillard. Copyright INRIA 2005.
     */
  
  template <class TInputImage, class TOutputImage >
    class ITK_EXPORT NormalizedGaussianInterpolationTensorImageFilter :
  public ImageToImageFilter< TInputImage, TOutputImage >
  {
    
  public:
    
    typedef NormalizedGaussianInterpolationTensorImageFilter Self;
    typedef ImageToImageFilter<TInputImage,TOutputImage>     Superclass;
    typedef SmartPointer<Self>                               Pointer;
    typedef SmartPointer<const Self>                         ConstPointer;
    
    
    itkNewMacro(Self);
    itkTypeMacro(NormalizedGaussianInterpolationTensorImageFilter, ImageToImageFilter);
    itkStaticConstMacro(ImageDimension, unsigned int,
			TOutputImage::ImageDimension);

    
    /** Standard image typedef */
    typedef TInputImage                          InputImageType;
    typedef typename InputImageType::PixelType   InputPixelType;
    typedef TOutputImage                         OutputImageType;
    typedef typename OutputImageType::PixelType  OutputPixelType;
    typedef typename OutputPixelType::ValueType  ScalarType;
    typedef typename OutputImageType::RegionType OutputImageRegionType;
    typedef Image<ScalarType, ImageDimension> AuxiliaryImageType;
    typedef typename AuxiliaryImageType::Pointer AuxiliaryImagePointerType;
    typedef typename AuxiliaryImageType::ConstPointer AuxiliaryImageConstPointerType;
    
    
    /** Specific typedefs */
    typedef std::vector<OutputPixelType>              VectorOfPixelsType;
    typedef Point<ScalarType, ImageDimension>         PointType;
    typedef std::vector<PointType>                    VectorOfPointsType;
    

    /** Set/Get the vector of tensors */
    void SetTensors( const VectorOfPixelsType vec )
    {
      for(unsigned int i=0; i<vec.size(); i++)
        m_Tensors.push_back(vec[i].Log());
    }
    VectorOfPixelsType GetTensors(void) const
    { return m_Tensors; }
    
    /** Set/Get the vector of positions */
    void SetPoints( const VectorOfPointsType vec )
    { m_Points=vec; }
    VectorOfPointsType GetPoints(void) const 
    { return m_Points; }

    /** Set/Get the tangents. */
    void SetTangents (const VectorOfPointsType tan)
    {
      m_Tangents = tan;
      if( m_Tangents.size() != 0 )
        m_DoNotDiffuseTangents = true;      
    }
    VectorOfPointsType GetTangents (void) const
    { return m_Tangents; }          
    

    /** Set/Get the variance of the Gaussian */
    itkSetMacro(Sigma,double);
    itkGetMacro(Sigma,double);

    AuxiliaryImagePointerType GetAuxiliaryMap(void)
    { return m_AuxiliaryMap; }
    
    void SetAuxiliaryMap (AuxiliaryImagePointerType map);

    itkSetClampMacro (UseAuxiliaryMap, unsigned int, 0, 1);
    itkGetMacro (UseAuxiliaryMap, unsigned int);
    itkBooleanMacro (UseAuxiliaryMap);
    
    itkSetClampMacro (AuxiliaryAlpha, double, 0, 1);
    itkGetMacro (AuxiliaryAlpha, unsigned int);
    
  protected:
    
    NormalizedGaussianInterpolationTensorImageFilter()
    {
      m_DoNotDiffuseTangents = false;
      m_AuxiliaryMap = AuxiliaryImageType::New();
      m_UseAuxiliaryMap = 0;
      m_AuxiliaryAlpha = 0.5;
    };
    ~NormalizedGaussianInterpolationTensorImageFilter(){};
    
    /** Threaded implementation */
    void DynamicThreadedGenerateData(const OutputImageRegionType &);
    void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf (os, indent);      
    }
    
    
  private:
    NormalizedGaussianInterpolationTensorImageFilter(const Self&); // purposely not implemented
    void operator=(const Self&); // purposely not implemented
    
    VectorOfPixelsType m_Tensors;
    VectorOfPointsType m_Points;
    VectorOfPointsType m_Tangents;
        
    double m_Sigma; // sigma used for interpolation

    bool m_DoNotDiffuseTangents;
    AuxiliaryImagePointerType m_AuxiliaryMap;
    
    bool m_UseAuxiliaryMap;
    double m_AuxiliaryAlpha;
    
    
  };

} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkNormalizedGaussianInterpolationTensorImageFilter.txx"
#endif

#endif
