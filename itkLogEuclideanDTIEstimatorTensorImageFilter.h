#ifndef _itk_LogEuclideanDTIEstimatorTensorImageFilter_h_
#define _itk_LogEuclideanDTIEstimatorTensorImageFilter_h_

#include "itkDenseFiniteDifferenceImageFilter2.h"
#include "itkLogEuclideanDTIEstimatorTensorFunction.h"
#include "itkGradientMagnitudeTensorImageFilter.h"
#include <itkImage.h>


namespace itk
{
  
  template<class TInputImage, class TOutputImage>
    class ITK_EXPORT LogEuclideanDTIEstimatorTensorImageFilter :
  public DenseFiniteDifferenceImageFilter2<TInputImage, TOutputImage>
  {
    
  public:
    typedef LogEuclideanDTIEstimatorTensorImageFilter Self;
    typedef DenseFiniteDifferenceImageFilter2<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer<Self>        Pointer;
    typedef SmartPointer<const Self>  ConstPointer;

    itkNewMacro(Self);
    itkTypeMacro(LogEuclideanDTIEstimatorTensorImageFilter,
                 ImageToImageFilter);    
    itkStaticConstMacro(ImageDimension, unsigned int,
			TOutputImage::ImageDimension);
    
    /** Image typedef support. */
    typedef TInputImage                          InputImageType;
    typedef TOutputImage                         OutputImageType;
    typedef typename OutputImageType::PixelType  PixelType;
    typedef typename OutputImageType::RegionType OutputImageRegionType;
    typedef typename PixelType::ValueType        ScalarType;    
    typedef Image<ScalarType, ImageDimension>    ScalarImageType;

      /** The container type for the update buffer. */
    typedef OutputImageType                      UpdateBufferType;
    
    /** FiniteDifferenceFunction type. */
    typedef typename Superclass::FiniteDifferenceFunctionType
      FiniteDifferenceFunctionType;
    
    /** function type */
    typedef LogEuclideanDTIEstimatorTensorFunction<InputImageType>
      FunctionType;
    typedef typename FunctionType::ImageListType    ImageListType;
    typedef typename FunctionType::GradientListType GradientListType;
    typedef typename FunctionType::GradientType     GradientType;
    typedef typename FunctionType::TimeStepType     TimeStepType;
    typedef typename FunctionType::VarianceType     VarianceType;
                
    /** Gradient typedefs */
    typedef GradientMagnitudeTensorImageFilter<InputImageType,
      ScalarImageType> GradientFilterType;    

    /** This method returns true when the current iterative solution of the
     * equation has met the criteria to stop solving. */
    bool Halt(void);

    /** Get an energy criterion used to evaluate the convergence */
    const double GetRMSChange(void)
    { return m_Function->GetRMSChange(); }
    
    

    /** Specific methods for the function */

    /** Set the time step. */
    void SetTimeStep( const TimeStepType & t )
    { m_Function->SetTimeStep (t); }    
    /** Get the time step. */
    const TimeStepType &GetTimeStep() const
    { return m_Function->GetTimeStep(); }

    /** Set the b0 time step. */
    void SetB0TimeStep( const TimeStepType & t )
    { m_Function->SetB0TimeStep (t); }    
    /** Get the b0 time step. */
    const TimeStepType &GetB0TimeStep() const
    { return m_Function->GetB0TimeStep(); }
    
    /** Set the list of B file. */
    void SetDTI(ImageListType list)
    { m_Function->SetDTI (list); }    
    /** Get the list of B file. */
    ImageListType GetDTI(void)
    { return m_Function->GetDTI(); }
    
    /** Add a DT image */
    void AddDTI(typename ScalarImageType::Pointer im)
    { m_Function->AddDTI (im); }    
    /** Set the gradient list */
    void SetGradientList(GradientListType list)
    { m_Function->SetGradientList ( list ); }    


    /** Get the gradient list */
    GradientListType GetGradientList(void)
    { return m_Function->GetGradientList(); }
    /** Add a gradient to the list */
    void AddGradient(GradientType g)
    { m_Function->AddGradient (g); }
    

    /** Set the Background Suppression Threshold */
    void SetBST(ScalarType bst)
    { m_Function->SetBST (bst); }    
    /** Get The Background Suppression Threshold */
    ScalarType GetBST(void)
    { return m_Function->GetBST(); }
    

    /** Set Lambda - quantity of smoothing */
    void SetLambda(ScalarType l)
    { m_Function->SetLambda (l); }
    /** Get lambda */
    ScalarType GetLambda(void)
    { return m_Function->GetLambda(); }    
    

    /** Set Kappa - normalization factor for the gradient */
    void SetKappa(ScalarType k)
    { m_Function->SetKappa (k); }
    /** Get Kappa */
    ScalarType GetKappa(void)
    { return m_Function->GetKappa(); }

    /** Set C: For the GemanMcLure estimator */
    void SetC (ScalarType c)
    { m_Function->SetC (c);}
    ScalarType GetC (void)
    {return m_Function->GetC();}


    /** Set the estimation mode*/
    void SetEstimationMode (int a)
    { m_Function->SetEstimationType(a); }

    /** Set sigma */
    void SetVariances (VarianceType s)
    { m_Function->SetVariances(s); }
    
    

    /** Specify the M-Estimator: 0: least squre, 1: Geman McLure*/
    void SetMEstimator (int n)
    { m_Function->SetMEstimator (n); }
    

    
    typename ScalarImageType::Pointer GetB0 (void)
    { return m_Function->GetB0(); }
    
    
    
  protected:
    
    LogEuclideanDTIEstimatorTensorImageFilter()
    {
      m_Function = FunctionType::New();
      this->SetDifferenceFunction (m_Function);
      m_GradientFilter = GradientFilterType::New();
    };
    ~LogEuclideanDTIEstimatorTensorImageFilter(){};
    
    void BeforeThreadedGenerateData(void);    
    void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf (os, indent );
    }
    
    void InitializeIteration(void);
    
  private:
    
    LogEuclideanDTIEstimatorTensorImageFilter(const Self&);
    void operator=(const Self&);
    
    typename GradientFilterType::Pointer m_GradientFilter;
    typename FunctionType::Pointer m_Function;
    
  };
  


} // end of namespace mips


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLogEuclideanDTIEstimatorTensorImageFilter.txx"
#endif

#endif
