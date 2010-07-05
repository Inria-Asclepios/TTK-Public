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

#include "itkGradientFileReader.h"
#include "itkGradientFileWriter.h"

typedef itk::GradientFileReader ReaderType;
typedef itk::GradientFileWriter WriterType;
typedef ReaderType::VectorListType VectorListType;
typedef ReaderType::VectorType VectorType;
typedef ReaderType::ScalarType ScalarType;

bool CompareVectorList (VectorListType l1, VectorListType l2)
{
  if (l1.size() != l2.size())
  {
    std::cerr << "Vector lists do not have the same size." << std::endl;
    return false;
  }
  
  for (unsigned int i=0; i<l1.size(); i++)
  {
    if (l1[i] != l2[i])
    {
      std::cerr << "Vector " << i << "differs in lists."<< std::endl;
      return false;
    }
  }
  return true;
}

  
int itkGradientFileReadWriteTest(int argc, char* argv[])
{


  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  // reader->DebugOn();
  // writer->DebugOn();
  
  std::string g_name_bvec = "gradients.bvec";
  std::string g_name_txt  = "gradients.txt";
  std::string g_name_grad = "gradients.grad";

  VectorType g0 (ScalarType (0.0));
  VectorListType m_list (7, g0);
  
  m_list[0][0] =  0;     m_list[0][1] =  0;     m_list[0][2] =  0;
  m_list[1][0] = -0.667; m_list[1][1] = -0.667; m_list[1][2] = -0.333;
  m_list[2][0] = -0.333; m_list[2][1] =  0.667; m_list[2][2] = -0.667;
  m_list[3][0] = -0.667; m_list[3][1] =  0.333; m_list[3][2] =  0.667;
  m_list[4][0] = -0.707; m_list[4][1] =  0;     m_list[4][2] = -0.707;
  m_list[5][0] = -0.707; m_list[5][1] =  0.707; m_list[5][2] =  0;
  m_list[6][0] =  0;     m_list[6][1] =  0.707; m_list[6][2] =  0.707;
  
  writer->SetGradientList (m_list);
  
  std::cout<<"Writing gradients files"<<std::endl;
  try
  {
    writer->SetFileName (g_name_bvec.c_str());
    writer->Update();
    writer->SetFileName (g_name_txt.c_str());
    writer->Update();
    writer->SetFileName (g_name_grad.c_str());
    writer->Update();
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return EXIT_FAILURE;
  }

  
  try
  {
    std::cout<<"reading "<<g_name_bvec.c_str()<<"..."<<std::flush;
    reader->SetFileName (g_name_bvec.c_str());
    reader->Update();
    if (! CompareVectorList (m_list, reader->GetGradientList())) return EXIT_FAILURE;
    std::cout<<"\t OK."<<std::endl;
    
    std::cout<<"reading "<<g_name_txt.c_str()<<"..."<<std::flush;
    reader->SetFileName (g_name_txt.c_str());
    reader->Update();
    if (! CompareVectorList (m_list, reader->GetGradientList())) return EXIT_FAILURE;
    std::cout<<"\t OK."<<std::endl;
    
    std::cout<<"reading "<<g_name_grad.c_str()<<"..."<<std::flush;
    reader->SetFileName (g_name_grad.c_str());
    reader->Update();
    if (! CompareVectorList (m_list, reader->GetGradientList())) return EXIT_FAILURE;
    std::cout<<"\t OK."<<std::endl;
    
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
