#ifndef _itk_VTKStructuredPointsTensorImageIO_h
#define _itk_VTKStructuredPointsTensorImageIO_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <itkObject.h>
#include <itkImage.h>
#include <itkImageRegionIterator.h>
#include <itkTensor.h>


namespace itk
{

  class ITK_EXPORT VTKStructuredPointsTensorImageIO : public Object
  {
    
  public:

    typedef VTKStructuredPointsTensorImageIO Self;
    typedef Object Superclass;
    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    itkNewMacro(Self);
    itkTypeMacro(VTKStructuredPointsTensorImageIO, Object);
    
    /** specific typedefs */
    typedef double                               ScalarType;
    typedef Tensor<ScalarType, 3>                TensorType;
    typedef Image<TensorType, 3>                 TensorImageType;
    typedef ImageRegionIterator<TensorImageType> IteratorType;

    void Read(void);
    void Write(void);

    itkSetStringMacro(FileName);
    itkGetStringMacro(FileName);

    itkSetObjectMacro(Input, TensorImageType);
    itkGetObjectMacro(Input, TensorImageType);
    itkGetObjectMacro(Output, TensorImageType);


  protected:
    VTKStructuredPointsTensorImageIO()
    {
      m_Output = TensorImageType::New();
    }
    ~VTKStructuredPointsTensorImageIO(){};
    

    void PrintSelf(std::ostream &os, Indent indent) const
    {
      Superclass::PrintSelf(os,indent);      
    }
    
  private:

    VTKStructuredPointsTensorImageIO(const Self&); // purposely not implemented
    void operator=(const Self&); // purposely not implemented
    
    bool CheckExtension(const char*);
    
    std::string m_FileName;
    TensorImageType::Pointer m_Input;
    TensorImageType::Pointer m_Output;
    
  };

} // end of namespace mips

#endif
