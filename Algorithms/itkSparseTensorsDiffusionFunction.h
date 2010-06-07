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
#ifndef _itk_SparseTensorsDiffusionFunction_h_
#define _itk_SparseTensorsDiffusionFunction_h_

#include "itkFiniteDifferenceFunction.h"
#include "itkMacro.h"

namespace itk
{

  /** \class SparseTensorsDiffusionFunction
   *
   *
   */
  
  template <class TImage>
    class ITK_EXPORT SparseTensorsDiffusionFunction :
  public FiniteDifferenceFunction<TImage>
  {
    
  public:
    
    /** Standard typedefs */
    typedef SparseTensorsDiffusionFunction   Self;
    typedef FiniteDifferenceFunction<TImage> Superclass;
    typedef SmartPointer<Self>               Pointer;
    typedef SmartPointer<const Self>         ConstPointer;
    
    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods) */
    itkTypeMacro(SparseTensorsDiffusionFunction,
		 FiniteDifferenceFunction);
   
    /** Extract superclass dimension. */
    itkStaticConstMacro(ImageDimension, unsigned int,
                        Superclass::ImageDimension);
    
    /** Inherit some parameters from the superclass type. */
    typedef typename Superclass::ImageType        ImageType;
    typedef typename ImageType::PointType         PointType;
    typedef typename ImageType::IndexType         IndexType;
    typedef typename ImageType::SpacingType       SpacingType;    
    typedef typename Superclass::PixelType        PixelType;    
    typedef typename Superclass::RadiusType       RadiusType;
    typedef typename PixelType::ValueType         ScalarType;
    typedef typename Superclass::NeighborhoodType NeighborhoodType;
    typedef typename Superclass::FloatOffsetType  FloatOffsetType;
    typedef typename Superclass::TimeStepType     TimeStepType;

    typedef std::vector<PixelType>                VectorOfTensorsType;
    typedef std::vector<PointType>                VectorOfPointsType;
        
    
    virtual TimeStepType ComputeGlobalTimeStep(void *GlobalData) const
    { return this->GetTimeStep(); }
    

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
    
    virtual void InitializeIteration(void);
    
     /** Set the time step parameter */
    void SetTimeStep( const TimeStepType & t )
    { m_TimeStep = t; }
    
    /** Get the time step parameter */
    const TimeStepType &GetTimeStep() const
    { return m_TimeStep; }

    void SetTensors( const VectorOfTensorsType vec );
    VectorOfTensorsType GetTensors(void) const
    { return m_Tensors; }
    
    void SetPoints( const VectorOfPointsType vec )
    { m_Points=vec; }    
    VectorOfPointsType GetPoints(void) const
    { return m_Points; }
    
    void SetTangents( const VectorOfPointsType vec )
    { m_Tangents = vec;  }
    VectorOfPointsType GetTangents(void) const
    { return m_Tangents; }
    
    void SetLambda(const double val)
    { m_Lambda = val; }
    double GetLambda (void) const
    { return m_Lambda; }
    
    void SetSigma(const double s)
    { m_Sigma=s; }
    double GetSigma(void) const
    { return m_Sigma; }


    void SetDoNotDiffuseTangents (const bool a)
    { m_DoNotDiffuseTangents = a; }

    bool GetDoNotDiffuseTangents (void) const
    { return m_DoNotDiffuseTangents; }
    
            
                 
    /** This method computes the solution update for each pixel that does not
     * lie on a the data set boundary. */
    virtual PixelType ComputeUpdate(const NeighborhoodType &neighborhood,
				    void * globalData,
				    const FloatOffsetType& offset = FloatOffsetType(0.0)
				    );

    const double GetRMSChange()
    {return fabs(m_Energy-m_OldEnergy);};

  protected:
    
    SparseTensorsDiffusionFunction();
    ~SparseTensorsDiffusionFunction(){};

    struct GlobalDataStruct
    {
      double          Energy;
    };

    
  private:
    
    SparseTensorsDiffusionFunction(const Self&);
    void operator=(const Self&);
    
    TimeStepType  m_TimeStep;
    ScalarType    m_Lambda;
    ScalarType    m_Sigma;
    mutable ScalarType m_Energy;
    mutable ScalarType m_OldEnergy;
    
    VectorOfTensorsType m_Tensors;
    VectorOfPointsType  m_Points;
    VectorOfPointsType  m_Tangents;

    bool m_DoNotDiffuseTangents;
        
  };  

} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSparseTensorsDiffusionFunction.txx"
#endif

#endif
