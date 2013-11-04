/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: https://scm.gforge.inria.fr/svn/ttk/trunk/Algorithms/itkVectorImageToTensor.h $
  Language:  C++
  Date:      $Date: 2011-08-02 12:43:00 +0100 (Tue, 02 Aug 2011) $
  Version:   $Revision: 124 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itk_VectorImageToTensorImage_h_
#define _itk_VectorImageToTensorImage_h_

#include <itkImageToImageFilter.h>

namespace itk
{

template <class TInputImage, class TOutputImage>
class ITK_EXPORT VectorImageToTensorImage :
public ImageToImageFilter<TInputImage, TOutputImage>
{

public:

    typedef VectorImageToTensorImage                      Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer<Self>                            Pointer;
    typedef SmartPointer<const Self>                      ConstPointer;

    itkNewMacro(  Self );
    itkTypeMacro( VectorImageToTensorImage, ImageToImageFilter );

    typedef TInputImage                                   InputImageType;
    typedef typename InputImageType::PixelType            InputPixelType;
    typedef TOutputImage                                  OutputImageType;
    typedef typename OutputImageType::PixelType           OutputPixelType;
    typedef typename OutputImageType::RegionType          OutputImageRegionType;


protected:

    VectorImageToTensorImage(void){};
    virtual ~VectorImageToTensorImage(void){};

    void BeforeThreadedGenerateData(void);
    void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, ThreadIdType threadId);
    void PrintSelf(std::ostream& os, Indent indent) const
    {
        Superclass::PrintSelf(os,indent);
    }


private:


};


} // end of namespace


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVectorImageToTensorImage.txx"
#endif

#endif
