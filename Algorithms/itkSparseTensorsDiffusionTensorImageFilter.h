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
#ifndef _itk_SparseTensorsDiffusionTensorImageFilter_h_
#define _itk_SparseTensorsDiffusionTensorImageFilter_h_

#include "itkDenseFiniteDifferenceImageFilter.h"
#include "itkSparseTensorsDiffusionFunction.h"
#include <itkImage.h>
#include <itkVectorContainer.h>
#include <itkPoint.h>


namespace itk
{

  /**
     * \ Class SparseTensorsDiffusionTensorImageFilter
     * Description here.
     *
     * Author: Pierre Fillard. Copyright INRIA 2005.
     */
  
  template <class TInputImage, class TOutputImage >
    class ITK_EXPORT SparseTensorsDiffusionTensorImageFilter :
  public DenseFiniteDifferenceImageFilter< TInputImage, TOutputImage >
  {
    
  public:
    
    /** Standard typedefs */
    typedef SparseTensorsDiffusionTensorImageFilter                       Self;
    typedef DenseFiniteDifferenceImageFilter< TInputImage, TOutputImage > Superclass;
    /** Smart pointer typedef support.   */
    typedef SmartPointer<Self>        Pointer;
    typedef SmartPointer<const Self>  ConstPointer;
    
    itkNewMacro(Self);   
    itkTypeMacro(SparseTensorsDiffusionTensorImageFilter,
		 ImageToImageFilter);    
    itkStaticConstMacro(ImageDimension, unsigned int,
			TOutputImage::ImageDimension);

    
    /** Image typedef support. */
    typedef TInputImage                          InputImageType;
    typedef TOutputImage                         OutputImageType;
    typedef typename InputImageType::PixelType   InputPixelType;    
    typedef typename OutputImageType::PixelType  OutputPixelType;
    typedef typename OutputImageType::RegionType OutputImageRegionType;
    typedef typename InputImageType::Pointer     InputImagePointer;

    /** The container type for the update buffer. */
    typedef OutputImageType                      UpdateBufferType;
    
    /** FiniteDifferenceFunction type. */
    typedef typename Superclass::FiniteDifferenceFunctionType
      FiniteDifferenceFunctionType;
    
    /** Sparsely distributed tensors function type */
    typedef SparseTensorsDiffusionFunction<InputImageType>
      SparseTensorsDiffusionFunctionType;
    typedef typename SparseTensorsDiffusionFunctionType::TimeStepType        TimeStepType;
    typedef typename SparseTensorsDiffusionFunctionType::VectorOfTensorsType VectorOfTensorsType;
    typedef typename SparseTensorsDiffusionFunctionType::PointType           PointType;
    typedef typename SparseTensorsDiffusionFunctionType::VectorOfPointsType  VectorOfPointsType;
 

    /** This method returns true when the current iterative solution of the
     * equation has met the criteria to stop solving. */
    bool Halt(void);

    /** Get an energy criterion used to evaluate the convergence */
    const double GetRMSChange(void)
    {return m_Function->GetRMSChange(); }
    
    /** Specific function methods */
    void SetTimeStep( const TimeStepType & t )
    { m_Function->SetTimeStep(t); }
    const TimeStepType &GetTimeStep() const
    { return m_Function->GetTimeStep(); }

    void SetTensors( const VectorOfTensorsType vec )
    { m_Function->SetTensors (vec); }    
    VectorOfTensorsType GetTensors(void) const
    { return m_Function->GetTensors(); }

    void SetPoints( const VectorOfPointsType vec )
    { m_Function->SetPoints (vec); }    
    VectorOfPointsType GetPoints(void) const
    { return m_Function->GetPoints(); }
    
    void SetTangents( const VectorOfPointsType vec )
    { m_Function->SetTangents (vec); }
    VectorOfPointsType GetTangents(void) const
    { return m_Function->GetTangents(); }
    
    void SetLambda(const double val)
    { m_Function->SetLambda (val); }
    double GetLambda (void) const
    { return m_Function->GetLambda(); }
    
    void SetSigma(const double s)
    { m_Function->SetSigma (s); }
    double GetSigma(void) const
    { return m_Function->GetSigma(); }
    
    void SetDoNotDiffuseTangents (const bool a)
    { m_Function->SetDoNotDiffuseTangents(a); }

    bool GetDoNotDiffuseTangents (void) const
    { return m_Function->GetDoNotDiffuseTangents(); }
    
          
    

  protected:
    
    SparseTensorsDiffusionTensorImageFilter()
    {
      m_Function = SparseTensorsDiffusionFunctionType::New();
      this->SetDifferenceFunction (m_Function);      
    }
    ~SparseTensorsDiffusionTensorImageFilter(){};
    
    void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf(os, indent);
    }
    
    
    /** Perform checks and initialization before iterating the
	solution */
    void Initialize(void);

  private:
    
    SparseTensorsDiffusionTensorImageFilter(const Self&); // purposely not implemented
    void operator=(const Self&); // purposely not implemented

    typename SparseTensorsDiffusionFunctionType::Pointer m_Function;    
    
  };

} // end of namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSparseTensorsDiffusionTensorImageFilter.txx"
#endif

#endif
