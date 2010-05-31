#ifndef _itk_TensorToScalarFunction_h_
#define _itk_TensorToScalarFunction_h_

#include <itkLightObject.h>

namespace itk
{

  template < class TInputPixelType, class TOutputPixelType >
    class ITK_EXPORT TensorToScalarFunction : public LightObject
  {
    
  public:
    
    typedef TensorToScalarFunction Self;
    typedef LightObject Superclass;

    typedef SmartPointer<Self>        Pointer;
    typedef SmartPointer<const Self>  ConstPointer;

    itkTypeMacro (TensorToScalarFunction, LightObject);

    typedef TInputPixelType   InputPixelType;
    typedef TOutputPixelType  OutputPixelType;
    
    virtual OutputPixelType ComputeScalar (const  InputPixelType&) const = 0;
        
    
  protected:
    TensorToScalarFunction(){};
    ~TensorToScalarFunction(){};

    void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf(os, indent);
    }
    
    
    
  private:
    TensorToScalarFunction (const Self&);
    void operator=(const Self&);
        
    
  };   
  
  
} // end of namespace


#endif
