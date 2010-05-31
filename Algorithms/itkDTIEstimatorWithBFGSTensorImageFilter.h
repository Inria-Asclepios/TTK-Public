#ifndef _itk_DTIEstimatorWithBFGS_h_
#define _itk_DTIEstimatorWithBFGS_h_

#include <itkImageToImageFilter.h>
#include <vnl/vnl_matrix.h>


namespace itk
{
  /*! \class DTIEstimatorWithBFGSTensorImageFilter
    \ingroup TensorToolKit
    A BFGS-optimized DTI estimator based on the Stejskal&Tanner diffusion equation.
   */

  template<class TInputImage, class TOutputImage>
    class ITK_EXPORT DTIEstimatorWithBFGSTensorImageFilter :
  public ImageToImageFilter<TInputImage,TOutputImage>
  {

  public :

    typedef DTIEstimatorWithBFGSTensorImageFilter Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer<Self>  Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkNewMacro (Self);
    itkTypeMacro (DTIEstimatorWithBFGSTensorImageFilter,ImageToImageFilter);
    itkStaticConstMacro(ImageDimension, unsigned int,
			TOutputImage::ImageDimension);


    typedef TInputImage                             InputImageType;
    typedef typename InputImageType::PixelType      InputPixelType;
    typedef TOutputImage                            OutputImageType;
    typedef typename OutputImageType::PixelType     OutputPixelType;
    typedef typename OutputImageType::RegionType    OutputImageRegionType;
    typedef typename OutputPixelType::ValueType     ScalarType;
    typedef typename OutputImageType::IndexType     IndexType;
    typedef typename OutputImageType::SizeType      SizeType;
            
    /** Specific typedef for the gradient list */
    typedef Vector<ScalarType,3>          GradientType;
    typedef Vector<ScalarType,6>          VectorType;
    typedef std::vector<GradientType>     GradientListType;
    typedef std::vector<ScalarType>       DataListType;
    typedef std::vector<ScalarType>       VarianceType;
    
    /** Internal typedefs */
    typedef vnl_matrix< ScalarType >      InternalMatrixType;
    
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

    /** Set the init time step */
    itkSetMacro(Tinit, ScalarType);
    itkGetMacro(Tinit, ScalarType);

    /** Set/Get the estimation type */
    itkSetMacro(EstimationType, unsigned int);
    itkGetMacro(EstimationType, unsigned int);

    /** Set/Get the gradient descent type */
    itkSetMacro(GradientDescentType, unsigned int);
    itkGetMacro(GradientDescentType, unsigned int);

    /** Set/Get the MEstimator type */
    itkSetMacro(MEstimatorType, unsigned int);
    itkGetMacro(MEstimatorType, unsigned int);

    /** Set/Get Kappa */
    itkSetMacro(Kappa, ScalarType);
    itkGetMacro(Kappa, ScalarType);
        
    /** Set sigma for the estimation type 2 */
    void SetVariances (VarianceType s )
    { m_Variances = s; }
    VarianceType GetVariances (void) const
    { return m_Variances; }

    void SetInitialization ( const typename OutputImageType::Pointer t )
    { m_Initialization = t; };


    static ScalarType BesselRatio (const ScalarType &val);
    
    
    static ScalarType Bessel (const ScalarType &val);
    
    
  protected :
    DTIEstimatorWithBFGSTensorImageFilter()
    {
      m_BST = static_cast<ScalarType>( 0.0 );
      m_Kappa = static_cast<ScalarType>( 0.0 );
      m_Tinit = static_cast<ScalarType>( 1.0 );
      m_Initialization = 0;
      m_GradientDescentType = 0;
      m_EstimationType = 0;      
      m_MEstimatorType = 0;
            
    };
    ~DTIEstimatorWithBFGSTensorImageFilter(){};

    void BeforeThreadedGenerateData(void);
    void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, int threadId);
    void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    }

    InternalMatrixType BFGS (const OutputPixelType&,const OutputPixelType&,
                             const OutputPixelType&,const OutputPixelType&,
                             const InternalMatrixType&);

    ScalarType Simulator (const DataListType&, const OutputPixelType&, OutputPixelType&);

    OutputPixelType Optimizor (const OutputPixelType&, const OutputPixelType&,
                               const DataListType&);

    ScalarType LinearResearchWolfe (const OutputPixelType&, const OutputPixelType&,
                                    const DataListType&);

    ScalarType LinearResearch (const OutputPixelType&, const OutputPixelType&,
                                    const DataListType&);

    bool IsTooBig (const ScalarType&,const OutputPixelType&,
                   const OutputPixelType&, const DataListType&);

    bool IsTooSmall (const ScalarType&,const OutputPixelType&,
                     const OutputPixelType&, const DataListType&);

    ScalarType ExterpInterp (const ScalarType& tg, const ScalarType &td);
    

    ScalarType Huber ( const ScalarType &res);

    
    ScalarType DerHuber ( const ScalarType &res);
    
      
  private :
    DTIEstimatorWithBFGSTensorImageFilter(const Self &);
    void operator=(const Self&);

    GradientListType m_GradientList;
    ScalarType m_BST;
    ScalarType m_Kappa;
    
    VarianceType m_Variances;
    unsigned int m_EstimationType;
    unsigned int m_MEstimatorType;    
    unsigned int m_GradientDescentType;    
    ScalarType m_Tinit;
        
    typename OutputImageType::Pointer m_Initialization;
    
  };
  
  

} // end of namespace


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkDTIEstimatorWithBFGSTensorImageFilter.txx"
#endif


#endif
