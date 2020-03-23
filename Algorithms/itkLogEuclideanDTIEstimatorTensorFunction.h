#pragma once

#include <itkFiniteDifferenceFunction.h>
#include <itkMacro.h>
#include <itkImage.h>

#include "itkDTIEstimatorTensorImageFilter.h"

namespace itk
{

  /*! \class LogEuclideanDTIEstimatorTensorFunction
    \ingroup TensorToolKit
   *   
   *  A function which aims at estimating the image of
   *  tensors from a set of DT-MRI plus their corresponding
   *  gradient sequence. The estimation is performed in a
   *  least square sense on the images themselves and not
   *  their logarithm, as it is usually the case.
   *  A regularization term is added, allowing to smooth
   *  in the same time the tensor field. The smoothing is
   *  an adaptation of the \phi functional anisotropic
   *  smoothing of Perona and Malik.
   */

  template<class TImage>
  class ITK_EXPORT LogEuclideanDTIEstimatorTensorFunction:
    public FiniteDifferenceFunction<TImage>
  {
  public:
    typedef LogEuclideanDTIEstimatorTensorFunction Self;
    typedef FiniteDifferenceFunction<TImage>       Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Method for creation through the object factory. */
    itkNewMacro(Self);
    
    /** Run-time type information (and related methods) */
    itkTypeMacro(LogEuclideanDTIEstimatorTensorFunction,
		 FiniteDifferenceFunction);
    
    /** Extract superclass dimension. */
    itkStaticConstMacro(ImageDimension, unsigned int,
			Superclass::ImageDimension);
    
    /** Inherit some parameters from the superclass type. */
    typedef TImage                                         ImageType;
    typedef typename ImageType::PixelType                  PixelType;
    typedef typename ImageType::IndexType                  IndexType;
    typedef typename ImageType::SpacingType                SpacingType;    
    typedef typename Superclass::RadiusType                RadiusType;
    typedef typename PixelType::ValueType                  ScalarType;
    typedef Image<ScalarType, ImageDimension>              ScalarImageType;
    typedef std::vector<typename ScalarImageType::Pointer> ImageListType;
    typedef typename ScalarImageType::SizeType             SizeType;
    typedef typename PixelType::VectorType                 VectorType;
    typedef std::vector<ScalarType>                        VarianceType;
        
    typedef typename Superclass::NeighborhoodType NeighborhoodType;
    typedef typename Superclass::FloatOffsetType  FloatOffsetType;
    typedef typename Superclass::TimeStepType     TimeStepType;


    /** Diffusion gradients typedefs */
    typedef DTIEstimatorTensorImageFilter<ScalarImageType, ImageType>
      EstimatorType;
    typedef typename EstimatorType::GradientType     GradientType;
    typedef typename EstimatorType::GradientListType GradientListType;

    
    /** Update the time step. */
    virtual TimeStepType ComputeGlobalTimeStep(void *GlobalData) const
    {
      return this->GetTimeStep();      
    }
    

    /** Return the pointer to the global structure that is passed through the threads*/
    void *GetGlobalDataPointer() const
    {
      GlobalDataStruct *global = new GlobalDataStruct();
      global->Energy  = 0.0;
      global->EnergyB0 = 0.0;      
      return global;
    };

    /** Save the global energy and free the global data*/
    void ReleaseGlobalDataPointer(void *GlobalData) const
    {
      GlobalDataStruct* data = static_cast<GlobalDataStruct*>(GlobalData);
      m_Energy += data->Energy;
      m_B0Energy += data->EnergyB0;      
      delete data;
    };

    /** Give a damn guess!*/
    virtual void InitializeIteration(void);

    
    /** Set the time step. */
    void SetTimeStep( const TimeStepType & t )
    { m_TimeStep = t; }
    
    /** Get the time step. */
    const TimeStepType &GetTimeStep() const
    { return m_TimeStep; }


    /** Set the b0 time step. */
    void SetB0TimeStep( const TimeStepType & t )
    { m_TimeStepB0 = t; }

    
    /** Get the time step. */
    const TimeStepType &GetB0TimeStep() const
    { return m_TimeStepB0; }

    
    /** Set the list of B file. */
    void SetDTI(ImageListType list)
    {m_DTI = list;}

    
    /** Get the list of B file. */
    ImageListType GetDTI(void)
    {return m_DTI;}
    

    /** Add a DT image */
    void AddDTI(typename ScalarImageType::Pointer im)
    {
      if(im.IsNull())
	throw itk::ExceptionObject(__FILE__,__LINE__,"Error: Trying to pass a null pointer.");
      m_DTI.push_back(im);
    }

    
    /** Set the gradient list */
    void SetGradientList(GradientListType list)
    {m_GradientList = list;}    


    /** Get the gradient list */
    GradientListType GetGradientList(void)
    {return m_GradientList;}    


    /** Add a gradient to the list */
    void AddGradient(GradientType g)
    {m_GradientList.push_back(g);}
    

    /** Set the Background Suppression Threshold (must be replaced by a binary mask) */
    void SetBST(ScalarType bst)
    {m_BST = bst;}

    
    /** Get The Background Suppression Threshold */
    ScalarType GetBST(void)
    {return m_BST;}
    

    /** Set Lambda - quantity of smoothing */
    void SetLambda(ScalarType l)
    {m_Lambda = l;}    


    /** Get lambda */
    ScalarType GetLambda(void)
    {return m_Lambda;}    
    

    /** Set Kappa - normalization factor for the gradient */
    void SetKappa(ScalarType k)
    {m_Kappa = k;}


    /** Get Kappa */
    ScalarType GetKappa(void)
    {return m_Kappa;}


    /** Set C: for the Geman McLure estimation*/
    void SetC (ScalarType c)
    {m_C=c;}

    ScalarType GetC (void)
    {return m_C;}


    /** Set sigma for the estimation type 2 */
    void SetVariances (VarianceType s )
    { m_Variances = s; }
    VarianceType GetVariances (void) const
    { return m_Variances; }
    
    
    /** Perform the estimation on the logarithm of the signal */
    void SetEstimationToLogGaussian (void)
    { m_EstimationType = 0; }
    /** Perform the estimation on the signal itself */
    void SetEstimationToGaussian (void)
    { m_EstimationType = 1; }
    /** Perform the estimation on the SQUARE of the signal !! */
    void SetEstimationToRician (void)
    { m_EstimationType = 2; }
    /** Manually set the estimation type */
    void SetEstimationType (int n)
    { m_EstimationType = n; }

    int GetEstimationType ( void ) const
    { return m_EstimationType ;}
        
    
    /** Set the M-Estimator type to least square*/
    void SetMEstimatorToLeastSquare (void)
    { m_EstimatorType = 0; }

    /** Set the M-Estimator type to Geman McLure*/
    void SetMEstimatorTypeToGemanMcLure (void)
    { m_EstimatorType = 1; }

    /** Specify the M-Estimator */
    void SetMEstimator (int n)
    { m_EstimatorType = n; }          
    

    /** Set the updated gradient of the TF*/
    void SetGradient(typename ScalarImageType::Pointer image)
    {m_Gradient = image;}


    /** Get the gradient*/
    typename ScalarImageType::Pointer GetGradient(void)
    {return m_Gradient;}


    /** Get the B0 */
    typename ScalarImageType::Pointer GetB0 (void)
    {return m_B0;}
    

    
    /** This method computes the solution update for each pixel that does not
     * lie on a the data set boundary. */
    virtual PixelType ComputeUpdate(const NeighborhoodType &neighborhood,
				    void * globalData,
				    const FloatOffsetType& offset = FloatOffsetType(0.0)
				    );

    
    const double GetRMSChange()
    {return fabs( m_Energy );};
    
    
    
  protected:
    
    LogEuclideanDTIEstimatorTensorFunction();
    ~LogEuclideanDTIEstimatorTensorFunction(){};
    
    struct GlobalDataStruct
    {
      double          Energy;
      double          EnergyB0;
    };

    /** for internal use only */
    //MatrixType TensorProduct(const GradientType& g);
    ScalarType C1(const ScalarType&);
    ScalarType MEstimator (const ScalarType&);
    ScalarType PHI (const ScalarType&);
    ScalarType BesselRatio (const ScalarType&);
        
    
  private:
    
    LogEuclideanDTIEstimatorTensorFunction(const Self&);
    void operator=(const Self&);
    
    ImageListType    m_DTI;
    GradientListType m_GradientList;

    int m_NumberOfIterations;    
    
    ScalarType m_TimeStep;
    ScalarType m_TimeStepB0;
    ScalarType m_BST;
    ScalarType m_Lambda;
    ScalarType m_Kappa;
    ScalarType m_C;
    VarianceType m_Variances;
        
    mutable ScalarType m_Energy;
    mutable ScalarType m_B0Energy;    
    mutable ScalarType m_OldEnergy;
    mutable ScalarType m_OldB0Energy;    

    typename ScalarImageType::ConstPointer m_Gradient;
    typename ScalarImageType::Pointer m_B0;

    int m_EstimationType;
    int m_EstimatorType;
                
  };


} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLogEuclideanDTIEstimatorTensorFunction.txx"
#endif
