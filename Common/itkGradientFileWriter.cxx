/*=========================================================================

  Program:   Tensor ToolKit - TTK
  Module:    $URL: svn+ssh://ntoussaint@scm.gforge.inria.fr/svn/ttk/trunk/Common/itkAffineTensorTransform.h $
  Language:  C++
  Date:      $Date: 2010-06-07 12:39:13 +0100 (Mon, 07 Jun 2010) $
  Version:   $Revision: 68 $

  Copyright (c) INRIA 2010. All rights reserved.
  See LICENSE.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGradientFileWriter_cxx
#define __itkGradientFileWriter_cxx

#include "itkGradientFileWriter.h"


namespace itk
{
  
/** Constructor */
GradientFileWriter
::GradientFileWriter()
{
  m_FileName = "";
}

/** Destructor */
GradientFileWriter
::~GradientFileWriter()
{
}

/** Update the Writer */
void GradientFileWriter
::Update()
{
  if ( !m_GradientList.size() )
  {
    itkExceptionMacro ( "he input vector list is empty" << std::endl );
  }
  
  std::ofstream out( m_FileName.c_str(), std::ios::out);
  if( out.fail() )
  {
    out.close();
    itkExceptionMacro ( "The file could not be opened for write access "
                        << std::endl << "Filename: \"" << m_FileName << "\"" );
  }
  
  std::string fileextension = itksys::SystemTools::GetFilenameLastExtension(m_FileName);
  
  if (fileextension == ".bvec")
    this->WriteBvecFile (out);
  else if (fileextension == ".txt")
    this->WriteSimpleTextFile (out);
  else
    this->WriteInsightStyleFile (out);

  out.close();
}
  
/** Update the Reader */
void GradientFileWriter
::WriteBvecFile(std::ofstream& out)
{
  for (unsigned int j=0; j<3; j++)
  {
    for (unsigned int i=0; i<m_GradientList.size(); i++)
    {
      VectorType g = m_GradientList[i];
      out << g[j] << "\t";
    }
    out << std::endl;
  }
}

/** Update the Reader */
void GradientFileWriter
::WriteSimpleTextFile(std::ofstream& out)
{
  out << m_GradientList.size()<< std::endl;  
  for (unsigned int i=0; i<m_GradientList.size(); i++)
  {
    VectorType g = m_GradientList[i];
    out << g[0] << "\t" << g[1] << "\t" << g[2] << std::endl;
  }  
}

/** Update the Reader */
void GradientFileWriter
::WriteInsightStyleFile(std::ofstream& out)
{
  out << "#Insight GradientList File V1.0" << std::endl;
  for (unsigned int i=0; i<m_GradientList.size(); i++)
  {
    VectorType g = m_GradientList[i];
    out << i << ": " << g[0] << " " << g[1] << " " << g[2] << std::endl;
  }
}

} // namespace itk

#endif
