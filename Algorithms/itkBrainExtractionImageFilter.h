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
#ifndef _itk_BrainExtractionImageFilter_h_
#define _itk_BrainExtractionImageFilter_h_

#include <itkImageToImageFilter.h>
#include <itkSmoothingRecursiveGaussianImageFilter.h>
#include <itkAutomaticImageThresholdCalculator.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryCrossStructuringElement.h>
#include <itkBinaryErodeImageFilter.h>
#include <itkBinaryDilateImageFilter.h>
#include <itkMultiplyImageFilter.h>
#include <itkVotingBinaryIterativeHoleFillingImageFilter.h>

namespace itk
{

  template <class TInputImage, class TOutputImage>
    class ITK_EXPORT BrainExtractionImageFilter : public ImageToImageFilter <TInputImage, TOutputImage>
  {
  public:
    typedef BrainExtractionImageFilter                     Self;
    typedef ImageToImageFilter <TInputImage, TOutputImage> Superclass;

    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkNewMacro  (Self);
    itkTypeMacro (BrainExtractionImageFilter, ImageToImageFilter);

    // typedefs
    typedef TInputImage                        InputImageType;
    typedef typename InputImageType::PixelType InputPixelType;
    
    typedef TOutputImage                        OutputImageType;
    typedef typename OutputImageType::PixelType OutputPixelType;


    // specific typedefs
    typedef SmoothingRecursiveGaussianImageFilter<InputImageType, InputImageType>
      GaussianFilterType;

    typedef AutomaticImageThresholdCalculator<InputImageType>
      AutomaticImageThresholdCalculatorType;

    typedef BinaryThresholdImageFilter<InputImageType, OutputImageType>
      BinaryThresholdFilterType;
    
    typedef BinaryBallStructuringElement<OutputPixelType, InputImageType::ImageDimension>
      StructuralElementType;

    typedef BinaryCrossStructuringElement<OutputPixelType, InputImageType::ImageDimension>
      CrossType;

    typedef BinaryErodeImageFilter <OutputImageType, OutputImageType, StructuralElementType>
      BinaryErodeFilterType;
    
    typedef BinaryDilateImageFilter <OutputImageType, OutputImageType, CrossType>
      DilateFilterType;
    
    typedef MultiplyImageFilter<OutputImageType, OutputImageType, OutputImageType>
      MultiplyImageFilterType;

    typedef VotingBinaryIterativeHoleFillingImageFilter <OutputImageType>
      HoleFillingFilterType;

    /**
       Set the maximum number of iterations of the conditional reconstruction
       operation to ensure convergence. Default is 500.
     */
    itkSetMacro (MaximumNumberOfIterations, int);
    itkGetMacro (MaximumNumberOfIterations, int);
    

  protected:
    BrainExtractionImageFilter();
    ~BrainExtractionImageFilter();

    void GenerateData (void);

    bool CompareImages (OutputImageType *image1, OutputImageType *image2);

  private:
    BrainExtractionImageFilter (const Self&);
    void operator=(const Self&);

    int m_MaximumNumberOfIterations;
  };

} // end of namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBrainExtractionImageFilter.txx"
#endif

#endif
