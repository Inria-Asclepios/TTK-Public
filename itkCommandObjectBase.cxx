#include "itkCommandObjectBase.h"

namespace itk
{

  CommandObjectBase::CommandObjectBase()
  {}


  CommandObjectBase::~CommandObjectBase()
  {}


  const char *CommandObjectBase::GetShortDescription (void) const
  {
    return m_ShortDescription.c_str();
  }

  const char *CommandObjectBase::GetLongDescription (void) const
  {
    return m_LongDescription.c_str();
  }
  
}
