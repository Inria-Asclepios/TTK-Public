/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: https://scm.gforge.inria.fr/svn/ttk/trunk/Algorithms/itkAddRicianNoiseImageFilter.h $
  Language:  C++
  Date:      $Date: 2011-01-10 17:45:44 +0000 (Mon, 10 Jan 2011) $
  Version:   $Revision: 130 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_AddRicianNoiseImageFilter_h_
#define _itk_AddRicianNoiseImageFilter_h_

#include <itkAddGaussianNoiseImageFilter.h>
#include <itkImage.h>
#include <itkNormalVariateGenerator.h>

namespace itk
{


  template <class TInputImage, class TOutputImage>
    class ITK_EXPORT AddRicianNoiseImageFilter :
  public AddGaussianNoiseImageFilter<TInputImage, TOutputImage>
  {

  public:
    typedef AddRicianNoiseImageFilter Self;
    typedef AddGaussianNoiseImageFilter<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;

    itkNewMacro(Self);
    itkTypeMacro (AddRicianNoiseTensorImageFilter, AddGaussianNoiseImageFilter);

    typedef TInputImage                            InputImageType;
    typedef typename Superclass::InputPixelType    InputPixelType;
    typedef TOutputImage                           OutputImageType;
    typedef typename Superclass::OutputPixelType   OutputPixelType;
    typedef typename Superclass::OutputRegionType  OutputRegionType;
    typedef typename Superclass::InputImagePointer InputImagePointer;

  protected:
    AddRicianNoiseImageFilter()
    {
    }
    ~AddRicianNoiseImageFilter(){};

    void GenerateData(void);
    
  private:
    AddRicianNoiseImageFilter (const Self&);
    void operator=(const Self&);
    
  };
  

} // end of namespace


#include "itkAddRicianNoiseImageFilter.txx"

#endif
