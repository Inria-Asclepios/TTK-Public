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
        using Self         = FiberBundleStatisticsCalculator;
        using Superclass   = Object;
        using Pointer      = SmartPointer<Self>;
        using ConstPointer = SmartPointer<const Self>;

        itkNewMacro (Self);
        itkTypeMacro(FiberBundleStatisticsCalculator, Object);

        // specific typedef
        using ScalarType      = double;
        using ScalarArrayType = std::vector<ScalarType>;
        using TensorType      = Tensor<ScalarType, 3>;
        using FiberType       = Fiber<ScalarType, 3, ScalarType>;
        using FiberBundleType = vtkPolyData;

        void SetInput(FiberBundleType *bundle);
        FiberBundleType *GetInput() const;

        virtual void Compute ();

        ScalarArrayType GetFAValues () const;
        ScalarArrayType GetADCValues () const;
        ScalarArrayType GetFiberLengthValues () const;

        void GetFAStatistics     (ScalarType &mean, ScalarType &min, ScalarType &max, ScalarType &var);
        void GetADCStatistics    (ScalarType &mean, ScalarType &min, ScalarType &max, ScalarType &var);
        void GetLengthStatistics (ScalarType &mean, ScalarType &min, ScalarType &max, ScalarType &var);

    protected:
        FiberBundleStatisticsCalculator();
        ~FiberBundleStatisticsCalculator() override;

        void PrintSelf(std::ostream &os, Indent indent) const override
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
