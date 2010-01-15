#include "itkCommandObjectBase.h"

namespace itk
{

  CommandObjectBase::CommandObjectBase()
  {}


  CommandObjectBase::~CommandObjectBase()
  {}


  const char *CommandObjectBase::GetShortDescription (void) const
  {
    return 0;
  }

  const char *CommandObjectBase::GetLongDescription (void) const
  {
    return 0;
  }
  
}
