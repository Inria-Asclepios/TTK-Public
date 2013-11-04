#ifndef _itk_FiberBundleStatisticsCalculator_h_
#define _itk_FiberBundleStatisticsCalculator_h_

#include <itkObject.h>
#include <itkObjectFactory.h>

#include <itkTensor.h>
#include <itkFiber.h>

#include <vtkSmartPointer.h>

class vtkPolyData;

namespace itk
{
    class FiberBundleStatisticsCalculator : public Object
    {
    public:
        typedef FiberBundleStatisticsCalculator Self;
        typedef Object Superclass;

        typedef SmartPointer<Self>       Pointer;
        typedef SmartPointer<const Self> ConstPointer;

        itkNewMacro (Self);
        itkTypeMacro(FiberBundleStatisticsCalculator, Object);

        // specific typedef
        typedef double                           ScalarType;
        typedef std::vector<ScalarType>          ScalarArrayType;
        typedef Tensor<ScalarType, 3>            TensorType;
        typedef Fiber<ScalarType, 3, ScalarType> FiberType;

        typedef vtkPolyData FiberBundleType;

        void SetInput(FiberBundleType *bundle);
        FiberBundleType *GetInput(void) const;

        virtual void Compute (void);

        ScalarArrayType GetFAValues (void) const;
        ScalarArrayType GetADCValues (void) const;
        ScalarArrayType GetFiberLengthValues (void) const;

        void GetFAStatistics     (ScalarType &mean, ScalarType &min, ScalarType &max, ScalarType &var);
        void GetADCStatistics    (ScalarType &mean, ScalarType &min, ScalarType &max, ScalarType &var);
        void GetLengthStatistics (ScalarType &mean, ScalarType &min, ScalarType &max, ScalarType &var);

    protected:
        FiberBundleStatisticsCalculator();
        ~FiberBundleStatisticsCalculator();

        virtual void PrintSelf(std::ostream &os, Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

    private:
        FiberBundleStatisticsCalculator (const Self&);
        void operator=(const Self&);

        vtkSmartPointer<vtkPolyData> m_Input;

        ScalarArrayType m_FAValues;
        ScalarArrayType m_ADCValues;
        ScalarArrayType m_FiberLengthValues;
    };
}

#endif
