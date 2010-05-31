#ifndef _itk_VTKFibersToITKFibers_h_
#define _itk_VTKFibersToITKFibers_h_

#include <itkObject.h>
#include <itkDTITubeSpatialObject.h>
#include <itkDTITubeSpatialObjectPoint.h>
#include <itkGroupSpatialObject.h>
#include <itkContinuousIndex.h>

#include "itkLogTensorImageFilter.h"
#include "itkTensorLinearInterpolateImageFunction.h"

#include <vtkPolyData.h>

namespace itk
{


  template <class TensorImageType>
    class ITK_EXPORT VTKFibersToITKFibers : public Object
  {
    
  public:

    typedef VTKFibersToITKFibers     Self;
    typedef Object                   Superclass;
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkNewMacro (Self);
    itkTypeMacro (VTKFibersToITKFibers, Object);


    typedef LogTensorImageFilter<TensorImageType,TensorImageType>  LogTensorFilterType;
    typedef GroupSpatialObject<3>                                  GroupSpatialObjectType;
    typedef DTITubeSpatialObject<3>                                DTITubeSpatialObjectType;
    typedef DTITubeSpatialObjectPoint<3>                           DTIPointType;

    
    typedef itk::TensorLinearInterpolateImageFunction<TensorImageType, double> InterpolatorType;

    typedef typename TensorImageType::PixelType                    TensorType;
    typedef typename TensorImageType::PointType                    PointType;
    typedef typename TensorType::VectorType                        VectorType;

    enum ColorModeType
    {
      COLOR_BY_POINTDATA,
      COLOR_BY_USERCOLOR
    };
        
    itkSetObjectMacro (Input, vtkPolyData);
    vtkPolyData* GetInput (void) const
    { return m_Input; }

    itkSetObjectMacro (TensorImage, TensorImageType);
    itkGetObjectMacro (TensorImage, TensorImageType);

    itkSetMacro (BVal, double);
    itkGetMacro (BVal, double);

    itkSetMacro (ColorMode, ColorModeType);
    itkGetMacro (ColorMode, ColorModeType);

    void SetColorModeToPointData (void)
    { this->SetColorMode (COLOR_BY_POINTDATA); }

    void SetColorModeToUserColor (void)
    { this->SetColorMode (COLOR_BY_USERCOLOR); }

    void SetUserColor (const double col[3])
    {
      for( int i=0; i<3; i++)
      {
        m_UserColor[i] = col[i];
      }
    }
    
    
    itkGetObjectMacro (Output, GroupSpatialObjectType);
    
    void Update (void);
    
  protected:
    VTKFibersToITKFibers()
    {
      m_Input       = 0;
      m_TensorImage = 0;
      m_Output      = GroupSpatialObjectType::New();
      m_BVal        = 1.0;
      for( int i=0; i<3; i++)
      {
        m_UserColor[i] = 0.0;
      }
    }
    ~VTKFibersToITKFibers(){};
    

  private:

    vtkPolyData*                             m_Input;
    typename TensorImageType::Pointer        m_TensorImage;
    typename GroupSpatialObjectType::Pointer m_Output;
    double                                   m_BVal;
    ColorModeType                            m_ColorMode;
    double                                   m_UserColor[3];
  };

  

} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVTKFibersToITKFibers.txx"
#endif

#endif
