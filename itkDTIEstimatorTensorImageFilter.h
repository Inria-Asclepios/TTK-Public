#ifndef _itk_DTIEstimatorTensorImageFilter_h_
#define _itk_DTIEstimatorTensorImageFilter_h_

#include <itkImageToImageFilter.h>
#include <vnl/vnl_matrix.h>
#include <vnl/algo/vnl_matrix_inverse.h>


namespace itk
{

  /*! \class DTIEstimatorTensorImageFilter
    \ingroup TensorToolKit
    A least-square DTI estimator based on the linearized version of the Stejskal&Tanner diffusion equation.
   */

  template<class TInputImage, class TOutputImage>
    class ITK_EXPORT DTIEstimatorTensorImageFilter :
  public ImageToImageFilter<TInputImage,TOutputImage>
  {
    
  public:
    
    typedef DTIEstimatorTensorImageFilter                Self;
    typedef ImageToImageFilter<TInputImage,TOutputImage> Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkNewMacro(Self);
    itkTypeMacro(DTIEstimatorTensorImageFilter,ImageToImageFilter);    
    itkStaticConstMacro(ImageDimension, unsigned int,
			TOutputImage::ImageDimension);

    
    typedef TInputImage                             InputImageType;
    typedef typename InputImageType::PixelType      InputPixelType;
    typedef TOutputImage                            OutputImageType;
    typedef typename OutputImageType::PixelType     OutputPixelType;
    typedef typename OutputImageType::RegionType    OutputImageRegionType;
    typedef typename OutputPixelType::ValueType     ScalarType;
    
    /** Specific typedef for the gradient list */
    typedef Vector<ScalarType,3>          GradientType;
    typedef Vector<ScalarType,6>          VectorType;
    typedef std::vector<GradientType>     GradientListType;

    /** Internal typedefs */
    typedef vnl_matrix< ScalarType >         InternalMatrixType;
    typedef vnl_matrix_inverse< ScalarType > InverseType;
    
    
    /** Manually Set/Get a list of gradients */
    void SetGradientList(const GradientListType list)
    {m_GradientList = list;}
    GradientListType GetGradientList(void) const
    {return m_GradientList;}
    
    
    /** Add a gradient to the tail of the list of gradients */
    void AddGradient(const GradientType g)
    {m_GradientList.push_back(g);}
    
    /** Set/Get the Background Suppression Threshold (must be replaced by 
	a binary mask) */
    itkSetMacro(BST, ScalarType);
    itkGetMacro(BST, ScalarType);

    /*
      void SetBST(ScalarType bst)
      {m_BST = bst;}
      ScalarType GetBST(void)
      {return m_BST;}
    */
    
    
  protected:
    
    DTIEstimatorTensorImageFilter()
    {
      m_BST = static_cast<ScalarType>( 100.0 );
    };
    ~DTIEstimatorTensorImageFilter(){};
    
    void BeforeThreadedGenerateData(void);
    void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, int threadId);
    void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    }

    InternalMatrixType Vec2Matrix(const VectorType&) const;
    VectorType Matrix2Vec(const InternalMatrixType&) const;
    InternalMatrixType TensorProduct(const GradientType &g) const;
    

  private:
    
    DTIEstimatorTensorImageFilter(const Self&);
    void operator=(const Self&);

    GradientListType m_GradientList;
    ScalarType m_BST;
    
    InternalMatrixType m_G;
    InternalMatrixType m_IG2;
  
  };



} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkDTIEstimatorTensorImageFilter.txx"
#endif

#endif
