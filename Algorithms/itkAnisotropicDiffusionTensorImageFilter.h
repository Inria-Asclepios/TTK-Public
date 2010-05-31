#ifndef __itkAnisotropicDiffusionTensorImageFilter_h
#define __itkAnisotropicDiffusionTensorImageFilter_h

#include <itkDenseFiniteDifferenceImageFilter.h>
#include "itkAnisotropicDiffusionTensorFunction.h"
#include "itkGradientMagnitudeTensorImageFilter.h"
#include <itkImage.h>


namespace itk
{

  /*!\class AnisotropicDiffusionTensorImageFilter itkAnisotropicDiffusionTensorImageFilter.h itkAnisotropicDiffusionTensorImageFilter.h
    \ingroup TensorToolKit
    Filter class for doing anisotropic filtering of itk tensor images.
     */
  
  template <class TInputImage, class TOutputImage >
  class ITK_EXPORT AnisotropicDiffusionTensorImageFilter
    : public DenseFiniteDifferenceImageFilter< TInputImage, TOutputImage >
  {
  public:
    /** Standard typedefs */
    typedef AnisotropicDiffusionTensorImageFilter                         Self;
    typedef DenseFiniteDifferenceImageFilter< TInputImage, TOutputImage > Superclass;
    /** Smart pointer typedef support.   */
    typedef SmartPointer<Self>        Pointer;
    typedef SmartPointer<const Self>  ConstPointer;

    itkNewMacro(Self);
    itkTypeMacro(AnisotropicDiffusionTensorImageFilter,
		 ImageToImageFilter);    
    itkStaticConstMacro(ImageDimension, unsigned int,
			TOutputImage::ImageDimension);
    
    /** Image typedef support. */
    typedef TInputImage                          InputImageType;
    typedef TOutputImage                         OutputImageType;
    typedef typename OutputImageType::PixelType  PixelType;
    typedef typename PixelType::ValueType        ValueType;  
    typedef typename OutputImageType::RegionType OutputImageRegionType;
    typedef typename InputImageType::Pointer     InputImagePointer;
    

    /** Gradient typedefs */
    typedef Image<ValueType, ImageDimension>           ScalarImageType;
    typedef GradientMagnitudeTensorImageFilter<InputImageType, ScalarImageType>
      GradientFilterType;

    
    /** The container type for the update buffer. */
    typedef OutputImageType                      UpdateBufferType;
    

    /** FiniteDifferenceFunction type. */
    typedef typename Superclass::FiniteDifferenceFunctionType
      FiniteDifferenceFunctionType;
    
    /** anisotropic diffusion function type */
    typedef AnisotropicDiffusionTensorFunction<InputImageType>
      AnisotropicDiffusionTensorFunctionType;
    typedef typename AnisotropicDiffusionTensorFunctionType::TimeStepType
      TimeStepType;
    

    /** This method returns true when the current iterative solution of the
     * equation has met the criteria to stop solving. */
    bool Halt(void);

    /** Get an energy criterion used to evaluate the convergence */
    const double GetRMSChange(void);


    /** Methods for the Anisotropic function */
    void SetTimeStep (const TimeStepType &t)
    {
      m_Function->SetTimeStep (t);
      this->Modified();
    }
    const TimeStepType & GetTimeStep (void) const
    {return m_Function->GetTimeStep();}

    void SetKappa (double k)
    {
      m_Function->SetKappa (k);
      this->Modified();
    }
    double GetKappa (void) const
    { return m_Function->GetKappa(); }

    void SetLambda (double l)
    {
      m_Function->SetLambda (l);
      this->Modified();
    }
    double GetLambda (void) const
    {return m_Function->GetLambda();}    


  protected:
    AnisotropicDiffusionTensorImageFilter()
    {
      m_GradientFilter = GradientFilterType::New();
      m_Function = AnisotropicDiffusionTensorFunctionType::New();
      this->SetDifferenceFunction (m_Function);
      
    };
    ~AnisotropicDiffusionTensorImageFilter(){};
    
    void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf (os, indent);      
    }
    
    void InitializeIteration(void);
    

  private:
    AnisotropicDiffusionTensorImageFilter(const Self&); // purposely not implemented
    void operator=(const Self&); // purposely not implemented

    typename AnisotropicDiffusionTensorFunctionType::Pointer m_Function;    
    typename GradientFilterType::Pointer m_GradientFilter;
    
  };

} // end of namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAnisotropicDiffusionTensorImageFilter.txx"
#endif

#endif
