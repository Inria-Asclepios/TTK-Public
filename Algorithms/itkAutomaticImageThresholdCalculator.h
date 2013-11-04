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
#ifndef _itk_AutomaticImageThresholdCalculator_h_
#define _itk_AutomaticImageThresholdCalculator_h_

#include <itkObject.h>

#include <itkScalarImageToHistogramGenerator.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkImage.h>

namespace itk
{
  /**
     Automatically find a threshold by maximizing the inter-class variance.
   */

  template <class TInputImage>
    class ITK_EXPORT AutomaticImageThresholdCalculator : public Object
  {
  public:
    typedef AutomaticImageThresholdCalculator Self;
    typedef Object                            Superclass;
    
    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkNewMacro  (Self);
    itkTypeMacro (AutomaticImageThresholdCalculator, Object);

    // typedefs
    typedef TInputImage                   ImageType;
    typedef typename ImageType::PixelType PixelType;

    // specific typedefs
    typedef itk::Statistics::ScalarImageToHistogramGenerator <ImageType>
      HistogramGeneratorType;
    typedef typename HistogramGeneratorType::HistogramType HistogramType;

    typedef itk::MinimumMaximumImageCalculator <ImageType>
      MinMaxCalculatorType;

    itkSetConstObjectMacro(Image, ImageType);

    void Compute (void);

    itkGetMacro (Threshold, PixelType);

  protected:
    AutomaticImageThresholdCalculator();
    ~AutomaticImageThresholdCalculator();

  private:
    AutomaticImageThresholdCalculator (const Self&);
    void operator=(const Self&);

    PixelType                        m_Threshold;
    typename ImageType::ConstPointer m_Image;
  };
    

} // end of namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAutomaticImageThresholdCalculator.txx"
#endif

#endif
