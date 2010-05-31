#ifndef _itk_TensorToColorFAFunction_h_
#define _itk_TensorToColorFAFunction_h_

#include "itkTensorToScalarFunction.h"

namespace itk
{

  template <class TInputPixelType, class TOutputPixelType>
    class ITK_EXPORT TensorToColorFAFunction
    : public TensorToScalarFunction<TInputPixelType, TOutputPixelType>
  {

  public:
    typedef TensorToColorFAFunction Self;
    typedef TensorToScalarFunction<TInputPixelType, TOutputPixelType> Superclass;

    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    itkTypeMacro (TensorToColorFAFunction, TensorToScalarFunction);
    itkNewMacro (Self);

    typedef TInputPixelType   InputPixelType;
    typedef TOutputPixelType  OutputPixelType;    

    OutputPixelType ComputeScalar (const InputPixelType&) const;

  protected:
    TensorToColorFAFunction(){};
    ~TensorToColorFAFunction(){};
        
     void PrintSelf(std::ostream& os, Indent indent) const
    {
      Superclass::PrintSelf(os, indent);
    }

  private:
     TensorToColorFAFunction (const Self&);
     void operator=(const Self&);
    
  };
  

} // end of namespace


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTensorToColorFAFunction.txx"
#endif

#endif
