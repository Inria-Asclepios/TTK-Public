#ifndef _itk_FiberBundleToScalarFunction_h_
#define _itk_FiberBundleToScalarFunction_h_

#include <itkObject.h>
#include <itkObjectFactory.h>

#include <itkTensor.h>
#include <itkFiber.h>

#include <vtkSmartPointer.h>

class vtkPolyData;

namespace itk
{
    class FiberBundleToScalarFunction : public Object
    {
    public:
        typedef FiberBundleToScalarFunction Self;
        typedef Object Superclass;

        typedef SmartPointer<Self>       Pointer;
        typedef SmartPointer<const Self> ConstPointer;

        itkNewMacro (Self);
        itkTypeMacro(FiberBundleToScalarFunction, Object);

        // specific typedef
        typedef std::vector<double> ScalarArrayType;
        typedef double              MeanType;
        typedef double              VarianceType;
        typedef Tensor<double, 3>   TensorType;
        typedef Fiber<double, 3, double> FiberType;

        typedef vtkPolyData FiberBundleType;

        void SetInput(FiberBundleType *bundle);
        FiberBundleType *GetInput(void) const;

        virtual void Compute (void);

        ScalarArrayType GetFAValues (void) const;
        ScalarArrayType GetADCValues (void) const;
        ScalarArrayType GetFiberLengthValues (void) const;

        MeanType GetMeanFA (void) const;
        MeanType GetMeanADC (void) const;
        MeanType GetMeanFiberLength (void) const;

        VarianceType GetVarianceFA (void) const;
        VarianceType GetVarianceADC (void) const;
        VarianceType GetVarianceFiberLength (void) const;

    protected:
        FiberBundleToScalarFunction();
        ~FiberBundleToScalarFunction();

        virtual void PrintSelf(std::ostream &os, Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

    private:
        FiberBundleToScalarFunction (const Self&);
        void operator=(const Self&);

        vtkSmartPointer<vtkPolyData> m_Input;

        ScalarArrayType m_FAValues;
        ScalarArrayType m_ADCValues;
        ScalarArrayType m_FiberLengthValues;
    };
}

#endif
