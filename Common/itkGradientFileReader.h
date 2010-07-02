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
#ifndef __itkGradientFileReader_h
#define __itkGradientFileReader_h

#include "itkLightProcessObject.h"
#include "itkVector.h"
#include <string>
#include <vector>

namespace itk
{

/** \class GradientFileReader
 * 
 * \brief 
 */
class GradientFileReader : public LightProcessObject
{
public:

  /** SmartPointer typedef support */
  typedef GradientFileReader Self;
  typedef SmartPointer<Self>  Pointer;

  typedef double ScalarType;
  typedef Vector<ScalarType, 3> VectorType;
  typedef std::vector<VectorType> VectorListType;
    
  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  typedef Object Superclass;
  itkTypeMacro(GradientFileReader, LightProcessObject);

  /** Set the filename  */
  itkSetStringMacro(FileName);

  /** Get the filename */
  itkGetStringMacro(FileName);

  /** Read out the gradients the transform */
  void Update();

  /** Get the gradient list */
  VectorListType GetGradientList() {return m_GradientList;}

protected:
  GradientFileReader(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  void ReadInsightStyleFile (std::string data);
  void ReadBvecFile (std::string data);
  void ReadSimpleTextFile (std::string data);
  
  std::string m_FileName;

  GradientFileReader();
  virtual ~GradientFileReader();

private:

  VectorListType m_GradientList;
  
};

} // namespace itk

#endif // __itkGradientFileReader_h
