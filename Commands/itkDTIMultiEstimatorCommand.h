#pragma once

#include "itkCommandObjectBase.h"

#include "itkDTIEstimatorTensorImageFilter.h"
#include "itkRemoveNonPositiveTensorsTensorImageFilter.h"
#include "itkTensorImageIO.h"
#include <itkImageFileReader.h>
#include <itkExtractImageFilter.h>
#include <itkImage.h>
#include "itkNumericTraits.h"
#include "itkGradientFileReader.h"

#include <vector>

namespace itk {

  class DTIMultiEstimatorCommand : public CommandObjectBase
  {
    
  public:
		
    typedef DTIMultiEstimatorCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(DTIMultiEstimatorCommand, CommandObjectBase);
    itkNewMacro(Self);


    typedef double ScalarType;  
    typedef itk::Image<ScalarType, 3>                      ImageType;
    typedef itk::Image<ScalarType, 4>                      Image4DType;
    
    typedef itk::TensorImageIO<ScalarType, 3, 3>           IOType;
    typedef IOType::TensorImageType                        TensorImageType;    
    typedef itk::ImageFileReader<ImageType>                ReaderType;
    typedef itk::ImageFileReader<Image4DType>              Reader4DType;
    
    typedef itk::DTIEstimatorTensorImageFilter<ImageType, TensorImageType>
      EstimatorType;  
    typedef EstimatorType::GradientType GradientType;
    typedef EstimatorType::GradientListType GradientListType;
    typedef itk::RemoveNonPositiveTensorsTensorImageFilter<TensorImageType,TensorImageType>
      RemoveNPTFilterType;
    
    typedef Image4DType::RegionType Region4dType;
    typedef Image4DType::SpacingType Spacing4Dtype;
    
    typedef itk::ImageRegionIterator<Image4DType> Iterator4DType;
    typedef Iterator4DType::IndexType Index4DType;
    typedef ImageType::DirectionType Direction3Dtype;
    typedef Image4DType::DirectionType Direction4Dtype;
    
    typedef std::pair<GradientType, ImageType::Pointer> DWIType;
    typedef std::vector<DWIType> ListOfDWIType;
    
    const char *GetCommandName(void)
    { return "estimate-multi"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    DTIMultiEstimatorCommand();
    ~DTIMultiEstimatorCommand();
    
  private:
    DTIMultiEstimatorCommand(const Self&);
    void operator=(const Self&);

    std::vector<ListOfDWIType> GetAllPossibleDWILists (ListOfDWIType input);
    std::vector<ListOfDWIType> GetDWIListsWithOneRemoval (ListOfDWIType input);
    std::vector<ListOfDWIType> GetAllNSubsets (ListOfDWIType input, unsigned int outputsize);
    
  };
  
}
