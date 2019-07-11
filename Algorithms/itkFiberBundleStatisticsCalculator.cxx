#include "itkFiberBundleStatisticsCalculator.h"

#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkCellArray.h>

namespace itk
{

    FiberBundleStatisticsCalculator::FiberBundleStatisticsCalculator()
    {
        m_Input = nullptr;
    }

    FiberBundleStatisticsCalculator::~FiberBundleStatisticsCalculator() = default;

    void FiberBundleStatisticsCalculator::SetInput(FiberBundleType *bundle)
    {
        m_Input = bundle;
        m_FAValues.clear();
        m_ADCValues.clear();
        m_FiberLengthValues.clear();
    }

    FiberBundleStatisticsCalculator::FiberBundleType*
        FiberBundleStatisticsCalculator::GetInput () const
    {
        return m_Input;
    }

    void FiberBundleStatisticsCalculator::Compute()
    {
        if (!m_Input)
            itkExceptionMacro (<< "input is not set");

        if (!m_Input->GetPointData()->HasArray("Tensors"))
            itkExceptionMacro(<< "bundle has no Tensors attribute, cannot compute scalars");

        vtkCellArray* lines = m_Input->GetLines();
        if( lines == nullptr )
        {
            return;
        }

        vtkDataArray *tCoefficients = m_Input->GetPointData()->GetArray("Tensors");
        vtkPoints* points = m_Input->GetPoints();

        lines->InitTraversal();
  
        vtkIdType  npts  = 0;
        vtkIdType* ptids = nullptr;
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

            double meanFA, junk1, junk2, junk3;
            fiber.GetFAStatistics(meanFA, junk1, junk2, junk3);
            double meanADC;
            fiber.GetADCStatistics(meanADC, junk1, junk2, junk3);

            m_FAValues.push_back (meanFA);
            m_ADCValues.push_back (meanADC);
            m_FiberLengthValues.push_back (fiber.GetLength());

            test = lines->GetNextCell (npts, ptids);
        }

    }


    FiberBundleStatisticsCalculator::ScalarArrayType FiberBundleStatisticsCalculator::GetFAValues () const
    {
        return m_FAValues;
    }


    FiberBundleStatisticsCalculator::ScalarArrayType FiberBundleStatisticsCalculator::GetADCValues () const
    {
        return m_ADCValues;
    }
        
    FiberBundleStatisticsCalculator::ScalarArrayType FiberBundleStatisticsCalculator::GetFiberLengthValues () const
    {
        return m_FiberLengthValues;
    }

    void FiberBundleStatisticsCalculator::GetFAStatistics(ScalarType &mean, ScalarType &min, ScalarType &max, ScalarType &var)
    {
        if (m_FAValues.size()==0)
        {
            this->Compute();
            if (m_FAValues.size()==0)
            {
                mean = 0.0;
                min = 0.0;
                max = 0.0;
                var = 0.0;
                return;
            }
        }

        ScalarType meanFA      = 0.0;
        ScalarType minFA       = 99999.9;
        ScalarType maxFA       = -1.0;
        ScalarType varFA       = 0.0;

        ScalarType totalLength = 0.0;
        for (unsigned int i=0; i<m_FAValues.size(); i++)
        {            
            if (m_FAValues [i]<minFA)
                minFA = m_FAValues [i];
            if (m_FAValues [i]>maxFA)
                maxFA = m_FAValues [i];

            meanFA += m_FAValues [i] * m_FiberLengthValues[i];
            totalLength += m_FiberLengthValues[i];
        }

        if (totalLength>0.0)
            meanFA /= totalLength;

        for (unsigned int i=0; i<m_FAValues.size(); i++)
        {
            ScalarType diff = m_FAValues[i] - meanFA;
            varFA += m_FiberLengthValues[i] * diff * diff;
        }

        if (totalLength>0.0)
            varFA /= totalLength;


        mean = meanFA;
        min = minFA;
        max = maxFA;
        var = varFA;
    }
        
    void FiberBundleStatisticsCalculator::GetADCStatistics(ScalarType &mean, ScalarType &min, ScalarType &max, ScalarType &var)
    {
        if (m_ADCValues.size()==0)
        {
            this->Compute();
            if (m_ADCValues.size()==0)
            {
                mean = 0.0;
                min = 0.0;
                max = 0.0;
                var = 0.0;
                return;
            }
        }

        ScalarType meanADC      = 0.0;
        ScalarType minADC       = 99999.9;
        ScalarType maxADC       = -1.0;
        ScalarType varADC       = 0.0;

        ScalarType totalLength = 0.0;
        for (unsigned int i=0; i<m_ADCValues.size(); i++)
        {
            if (m_ADCValues [i]<minADC)
                minADC = m_ADCValues [i];
            if (m_ADCValues [i]>maxADC)
                maxADC = m_ADCValues [i];

            meanADC += m_ADCValues [i] * m_FiberLengthValues[i];
            totalLength += m_FiberLengthValues[i];
        }

        if (totalLength>0.0)
            meanADC /= totalLength;

        for (unsigned int i=0; i<m_ADCValues.size(); i++)
        {
            ScalarType diff = m_ADCValues [i] - meanADC;
            varADC += m_FiberLengthValues[i] * diff * diff;
        }

        if (totalLength>0.0)
            varADC /= totalLength;


        mean = meanADC;
        min = minADC;
        max = maxADC;
        var = varADC;
    }

    void FiberBundleStatisticsCalculator::GetLengthStatistics(ScalarType &mean, ScalarType &min, ScalarType &max, ScalarType &var)
    {
        if (m_FiberLengthValues.size()==0)
        {
            this->Compute();
            if (m_FiberLengthValues.size()==0)
            {
                mean = 0.0;
                min = 0.0;
                max = 0.0;
                var = 0.0;
                return;
            }
        }

        ScalarType meanFiberLength      = 0.0;
        ScalarType minFiberLength       = 99999.9;
        ScalarType maxFiberLength       = -1.0;
        ScalarType varFiberLength       = 0.0;

        ScalarType totalLength = 0.0;
        for (unsigned int i=0; i<m_FiberLengthValues.size(); i++)
        {
            if (m_FiberLengthValues [i]<minFiberLength)
                minFiberLength = m_FiberLengthValues [i];
            if (m_FiberLengthValues [i]>maxFiberLength)
                maxFiberLength = m_FiberLengthValues [i];

            meanFiberLength += m_FiberLengthValues[i];
            totalLength += 1.0;
        }

        if (totalLength>0.0)
            meanFiberLength /= totalLength;

        for (unsigned int i=0; i<m_FiberLengthValues.size(); i++)
        {
            ScalarType diff = m_FiberLengthValues[i] - meanFiberLength;
            varFiberLength += diff * diff;
        }

        if (totalLength>0.0)
            varFiberLength /= totalLength;

        mean = meanFiberLength;
        min = minFiberLength;
        max = maxFiberLength;
        var = varFiberLength;
    }

} // end of namespace
