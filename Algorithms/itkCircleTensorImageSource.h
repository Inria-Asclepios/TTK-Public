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
#ifndef _itk_CircleTensorImageSource_h_
#define _itk_CircleTensorImageSource_h_

#include "itkImageSource.h"

namespace itk
{

  template <class TTensorImage>
    class CircleTensorImageSource :
  public ImageSource<TTensorImage>
  {

  public:
    typedef CircleTensorImageSource Self;
    typedef ImageSource<TTensorImage> Superclass;

    typedef SmartPointer<Self>       Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkNewMacro (Self);
    itkTypeMacro (CircleTensorImageSource, ImageSource);

    typedef TTensorImage                         TensorImageType;
    typedef typename TensorImageType::PixelType  TensorType;
    typedef typename TensorImageType::RegionType RegionType;
    typedef typename TensorImageType::PointType  PointType;
    typedef typename TensorType::VectorType      VectorType;


    itkSetMacro (Radius1, double);
    itkGetMacro (Radius1, double);

    itkSetMacro (Radius2, double);
    itkGetMacro (Radius2, double);

    
  protected:
    CircleTensorImageSource()
    {
      m_Radius1 = 1.0;
      m_Radius2 = 2.0;
    }
    ~CircleTensorImageSource(){};

    virtual void ThreadedGenerateData (const RegionType&, int);
    virtual void GenerateOutputInformation();

  private:
    CircleTensorImageSource(const Self&);
    void operator=(const Self&);

    double m_Radius1;
    double m_Radius2;
    

  };
  

} // end of namespace


#include "itkCircleTensorImageSource.txx"

#endif
