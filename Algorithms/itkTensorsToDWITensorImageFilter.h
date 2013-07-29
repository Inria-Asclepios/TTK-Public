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
#ifndef _itk_TensorsToDWITensorImageFilter_h_
#define _itk_TensorsToDWITensorImageFilter_h_

#include "itkImageToImageFilter.h"


namespace itk
{

  template <class TInputImage, class TOutputImage>
    class ITK_EXPORT TensorsToDWITensorImageFilter
    : public ImageToImageFilter<TInputImage, TOutputImage>
  {
    
  public:
    
    typedef TensorsToDWITensorImageFilter Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    
    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    itkTypeMacro (TensorsToDWITensorImageFilter, ImageToImageFilter);
    
    itkStaticConstMacro (ImageDimension, unsigned int,
                         TOutputImage::ImageDimension);
    
    itkNewMacro (Self);

    typedef double                                ScalarType;
    typedef TInputImage                           InputImageType;
    typedef typename InputImageType::PixelType    InputPixelType;    
    typedef TOutputImage                          OutputImageType;
    typedef typename OutputImageType::PixelType   OutputPixelType;
    typedef typename OutputImageType::RegionType  OutputImageRegionType;

    typedef Vector<ScalarType,3>          GradientType;
    typedef std::vector<GradientType>     GradientListType;


    /** Manually Set/Get a list of gradients */
    void SetGradientList(const GradientListType list)
    {
      m_GradientList = list;
      this->Modified();
      this->SetNumberOfRequiredOutputs (list.size());
      for(unsigned int i=1; i<list.size(); i++)
      {
        this->SetNthOutput (i, OutputImageType::New());
      }
    }
    GradientListType GetGradientList(void) const
    {return m_GradientList;}


	void SetBValue( const ScalarType& bval)
	{ m_BValue = bval; }
	
	//ScalarType GetBValue(void) const
	//{return m_BValue;}
	//itkSetMacro (BValue, ScalarType);
    //itkGetMacro (BValue, ScalarType);

    itkSetObjectMacro (BaselineImage, OutputImageType);
    itkGetObjectMacro (BaselineImage, OutputImageType);
    
    
  protected:
    TensorsToDWITensorImageFilter()
    {
      m_BValue = 1.0;
      m_BaselineImage = 0;
    };
    ~TensorsToDWITensorImageFilter(){};

    void PrintSelf (std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf (os, indent);      
    }
    

    void BeforeThreadedGenerateData( void );
    
    void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, ThreadIdType threadId);

    
  private:
    TensorsToDWITensorImageFilter (const Self&);
    void operator=(const Self&);

    GradientListType                   m_GradientList;
    ScalarType                         m_BValue;
    typename OutputImageType::Pointer  m_BaselineImage;

  };    


} // end of namespace


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTensorsToDWITensorImageFilter.txx"
#endif


#endif
