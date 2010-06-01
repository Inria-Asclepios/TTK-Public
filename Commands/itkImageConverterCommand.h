#ifndef _itk_ImageConverterCommand_h_
#define _itk_ImageConverterCommand_h_

#include "itkCommandObjectBase.h"

namespace itk {

  class ImageConverterCommand : public CommandObjectBase
  {
    
  public:
		
    typedef ImageConverterCommand Self;
    typedef CommandObjectBase Superclass;
    typedef SmartPointer <Self> Pointer;
    typedef SmartPointer <const Self> ConstPointer;
    
    itkTypeMacro(ImageConverterCommand, CommandObjectBase);
    itkNewMacro(Self);
    
    const char *GetCommandName(void)
    { return "image"; }
    
    int Execute(int nargs, const char *args[]);
    
  protected:
    ImageConverterCommand();
    ~ImageConverterCommand();
    
  private:
    ImageConverterCommand(const Self&);
    void operator=(const Self&);
    
  };
  
}

#endif
