#include "itkFiberBundleToScalarFunction.h"

#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkCellArray.h>

namespace itk
{

    FiberBundleToScalarFunction::FiberBundleToScalarFunction()
    {
        m_Input = 0;
    }

    FiberBundleToScalarFunction::~FiberBundleToScalarFunction()
    {}

    void FiberBundleToScalarFunction::SetInput(FiberBundleType *bundle)
    {
        m_Input = bundle;
        m_FAValues.clear();
        m_ADCValues.clear();
        m_FiberLengthValues.clear();
    }

    FiberBundleToScalarFunction::FiberBundleType* 
        FiberBundleToScalarFunction::GetInput (void) const
    {
        return m_Input;
    }

    void FiberBundleToScalarFunction::Compute()
    {
        if (!m_Input)
            itkExceptionMacro (<< "input is not set");

        if (!m_Input->GetPointData()->HasArray("Tensors"))
            itkExceptionMacro(<< "bundle has no Tensors attribute, cannot compute scalars");

        vtkCellArray* lines = m_Input->GetLines();
        if( lines==0 )
        {
            return;
        }

        vtkDataArray *tCoefficients = m_Input->GetPointData()->GetArray("Tensors");
        vtkPoints* points = m_Input->GetPoints();

        lines->InitTraversal();
  
        vtkIdType  npts  = 0;
        vtkIdType* ptids = 0;
        vtkIdType test = lines->GetNextCell (npts, ptids);

        while ( test )
        {
            FiberType fiber;
            for (int k=0; k<npts; k++)
            {
                double pt[3];
                points->GetPoint (ptids[k], pt);

                double t[6];
                tCoefficients->GetTuple(ptids[k], t);

                FiberType::FiberPoint fPoint;
                for (int i=0; i<3; i++)
                    fPoint.Point[i] = pt[i];
                fPoint.Tensor = t;

                fiber.AddPoint (fPoint);
            }

            m_FAValues.push_back (fiber.GetMeanFA());
            m_ADCValues.push_back (fiber.GetMeanADC());
            m_FiberLengthValues.push_back (fiber.GetLength());

            test = lines->GetNextCell (npts, ptids);
        }

    }


    FiberBundleToScalarFunction::ScalarArrayType FiberBundleToScalarFunction::GetFAValues (void) const
    {
        return m_FAValues;
    }


    FiberBundleToScalarFunction::ScalarArrayType FiberBundleToScalarFunction::GetADCValues (void) const
    {
        return m_ADCValues;
    }
        
    FiberBundleToScalarFunction::ScalarArrayType FiberBundleToScalarFunction::GetFiberLengthValues (void) const
    {
        return m_FiberLengthValues;
    }

    FiberBundleToScalarFunction::MeanType FiberBundleToScalarFunction::GetMeanFA (void) const
    {
        MeanType meanFA      = 0.0;
        MeanType totalLength = 0.0;
        for (unsigned int i=0; i<m_FAValues.size(); i++)
        {
            meanFA += m_FAValues [i] * m_FiberLengthValues[i];
            totalLength += m_FiberLengthValues[i];
        }

        if (totalLength>0.0)
            meanFA /= totalLength;

        return meanFA;
    }
        
    FiberBundleToScalarFunction::MeanType FiberBundleToScalarFunction::GetMeanADC (void) const
    {
        MeanType meanADC     = 0.0;
        MeanType totalLength = 0.0;
        for (unsigned int i=0; i<m_ADCValues.size(); i++)
        {
            meanADC += m_ADCValues [i] * m_FiberLengthValues[i];
            totalLength += m_FiberLengthValues[i];
        }

        if (totalLength>0.0)
            meanADC /= totalLength;

        return meanADC;
    }

    FiberBundleToScalarFunction::MeanType FiberBundleToScalarFunction::GetMeanFiberLength (void) const
    {
        MeanType totalLength = 0.0;
        for (unsigned int i=0; i<m_FiberLengthValues.size(); i++)
        {
            totalLength += m_FiberLengthValues[i];
        }

        if (m_FiberLengthValues.size()>0)
            totalLength /= static_cast<MeanType>(m_FiberLengthValues.size());

        return totalLength;
    }

    FiberBundleToScalarFunction::VarianceType FiberBundleToScalarFunction::GetVarianceFA (void) const
    {
        return 0.0;
    }
        
    FiberBundleToScalarFunction::VarianceType FiberBundleToScalarFunction::GetVarianceADC (void) const
    {
        return 0.0;
    }

    FiberBundleToScalarFunction::VarianceType FiberBundleToScalarFunction::GetVarianceFiberLength (void) const
    {
        return 0.0;
    }

} // end of namespace
