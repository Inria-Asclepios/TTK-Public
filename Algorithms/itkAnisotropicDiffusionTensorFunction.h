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
#ifndef __itkAnisotropicDiffusionTensorFunction_h
#define __itkAnisotropicDiffusionTensorFunction_h

#include <itkFiniteDifferenceFunction.h>
#include <itkImage.h>
#include <itkMacro.h>


namespace itk
{

  /*! \class AnisotropicDiffusionTensorFunction itkAnisotropicDiffusionTensorFunction.h itkAnisotropicDiffusionTensorFunction.h
    \ingroup TensorToolKit
    Base function for anisotropic filtering of itk tensor images.
   */
  
  template <class TImage>
    class ITK_EXPORT AnisotropicDiffusionTensorFunction:
  public FiniteDifferenceFunction<TImage>
  {  
  public:

    /** Standard typedefs */
    typedef AnisotropicDiffusionTensorFunction  Self;
    typedef FiniteDifferenceFunction<TImage>    Superclass;
    typedef SmartPointer<Self>                  Pointer;
    typedef SmartPointer<const Self>            ConstPointer;
    
    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods) */
    itkTypeMacro(AnisotropicDiffusionTensorFunction,
		 FiniteDifferenceFunction);
    
    /** Extract superclass dimension. */
    itkStaticConstMacro(ImageDimension, unsigned int,
                        Superclass::ImageDimension);
    
    /** Inherit some parameters from the superclass type. */
    typedef typename Superclass::ImageType  ImageType;
    typedef typename ImageType::PixelType   PixelType;
    typedef typename ImageType::IndexType   IndexType;
    typedef typename Superclass::RadiusType RadiusType;
    typedef typename ImageType::SpacingType SpacingType;
    
    typedef typename PixelType::ValueType      ScalarType;
    typedef Image<ScalarType,ImageDimension>   ScalarImageType;
    typedef typename ScalarImageType::SizeType SizeType;

    typedef typename Superclass::NeighborhoodType              NeighborhoodType;
    typedef typename Superclass::FloatOffsetType               FloatOffsetType;
    typedef typename Superclass::TimeStepType                  TimeStepType;
    typedef ConstNeighborhoodIterator<ScalarImageType>         IteratorImageType;


    virtual TimeStepType ComputeGlobalTimeStep(void *GlobalData) const
    { return m_TimeStep;   }
    

    void *GetGlobalDataPointer() const
    {
      GlobalDataStruct *global = new GlobalDataStruct();
      global->Energy  = 0.0;
      return global;
    };
    
    void ReleaseGlobalDataPointer(void *GlobalData) const
    {
      GlobalDataStruct* data = static_cast<GlobalDataStruct*>(GlobalData);
      m_Energy += data->Energy;
      delete data;
    };

    /** Executed before each iteration */
    virtual void InitializeIteration(void);
    
    /** Set the time step  */
    void SetTimeStep( const TimeStepType & t )
    { m_TimeStep = t; }
    
    /** Get the time step  */
    const TimeStepType &GetTimeStep() const
    { return m_TimeStep; }

    /** Set/Get Kappa */
    void SetKappa(double val)
    {m_Kappa = val;}
    double GetKappa(void)
    {return m_Kappa;}
    
    /** Set/Get Lambda */
    void SetLambda(double val)
    {m_Lambda = val;}    
    double GetLambda(void)
    {return m_Lambda;}

    /** Set the gradient norm image */
    //itkSetConstObjectMacro (Gradient, ScalarImageType);

    void SetGradient(typename ScalarImageType::ConstPointer image)
    {m_Gradient = image;}
    
    /** Get the gradient norm image */
    //itkGetConstObjectMacro (Gradient, ScalarImageType);
    
    typename ScalarImageType::Pointer GetGradient(void) const
    {return m_Gradient;}

    /** Set the data attach term */
    //itkSetConstObjectMacro (Attach, ImageType);
    
    void SetAttach(typename ImageType::ConstPointer image)
    {m_Attach = image;}

    /** Get the data attach term */
    //itkGetConstObjectMacro (Attach, ImageType);
    
    typename ImageType::Pointer GetAttach(void) const
    {return m_Attach;}
    
    /** This method computes the solution update for each pixel */
    virtual PixelType ComputeUpdate(const NeighborhoodType &neighborhood,
				    void * globalData,
				    const FloatOffsetType& offset = FloatOffsetType(0.0)
				    );

    const double GetRMSChange()
    {return fabs(m_Energy-m_OldEnergy);};

  protected:
    AnisotropicDiffusionTensorFunction();
    ~AnisotropicDiffusionTensorFunction(){};

    struct GlobalDataStruct
    {
      double          Energy;
    };

    /** Anisotropic function */
    ScalarType C1(ScalarType);

    
  private:
    AnisotropicDiffusionTensorFunction(const Self&); // purposely not implemented
    void operator=(const Self&); // purposely not implemented
    
    TimeStepType m_TimeStep;
    double       m_Kappa;
    double       m_Lambda;
    mutable ScalarType m_Energy;
    mutable ScalarType m_OldEnergy;
    
    typename ScalarImageType::ConstPointer m_Gradient;
    typename ImageType::ConstPointer       m_Attach;
    
  };  

} // end of namespace


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAnisotropicDiffusionTensorFunction.txx"
#endif

#endif
