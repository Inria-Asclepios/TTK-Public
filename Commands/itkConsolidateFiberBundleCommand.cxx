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
#include "itkConsolidateFiberBundleCommand.h"

#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkUnsignedCharArray.h>

#include <iostream>
#include "GetPot.h"


namespace itk
{
  
  ConsolidateFiberBundleCommand::ConsolidateFiberBundleCommand()
  {
    m_ShortDescription = "Consolidate a fiber bundle by retrieving point data from all fibers";
    m_LongDescription = "Usage:\n";
    m_LongDescription += "-i [fiber bundle]\n";
    m_LongDescription += "-a [all fibers]\n";
    m_LongDescription += "-o [output fiber bundle]\n\n";
    m_LongDescription += m_ShortDescription;
  }

  ConsolidateFiberBundleCommand::~ConsolidateFiberBundleCommand()
  {}

  int ConsolidateFiberBundleCommand::Execute (int narg, const char* arg[])
  {
    
    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
  

    const char* bundle      = cl.follow("NoFile",2,"-i","-I");
    const char* all_fibers  = cl.follow("NoFile",2,"-a","-A");
    const char* output      = cl.follow("output.vtk",2,"-o","-O");
    
    const bool IsInputPresent    = cl.search(2,"-a","-A");
    
    vtkPolyDataReader* reader2 = vtkPolyDataReader::New();
    reader2->SetFileName (bundle);
    reader2->Update();
    vtkPolyData* vtkBundle = reader2->GetOutput();
    
    vtkPolyDataReader* reader1 = vtkPolyDataReader::New();
    vtkPolyData* vtkAllFibers = NULL;
    vtkPoints* allPoints = NULL;
    vtkCellArray* lines = NULL;
    
    if (IsInputPresent)
    {
      reader1->SetFileName (all_fibers);
      reader1->Update();
      vtkAllFibers = reader1->GetOutput();
    }
    else
    {
      vtkAllFibers = reader2->GetOutput();
    }
    
    allPoints = vtkAllFibers->GetPoints();
    
    lines = vtkBundle->GetLines();
    lines->InitTraversal();
    
    vtkPolyData* vtkOutput = vtkPolyData::New();
    vtkOutput->Initialize();
    vtkOutput->Allocate();  
    
    vtkPoints* bundlePoints = vtkPoints::New();
    bundlePoints->Initialize();
    
    vtkUnsignedCharArray* colors = vtkUnsignedCharArray::New();
    colors->SetNumberOfComponents(3);
    
    vtkIdType npt, *pto;
    vtkIdType cellId = lines->GetNextCell (npt, pto);
    
    while (cellId!=0)
    {
      
      vtkIdType* newLine = new vtkIdType[npt];
      
      for( int i=0; i<npt; i++)
      {
	double* pt = NULL;
	pt = allPoints->GetPoint (pto[i]);
	
	newLine[i] = bundlePoints->InsertNextPoint (pt);
	colors->InsertNextTuple( vtkAllFibers->GetPointData()->GetScalars()->GetTuple (pto[i]));
      }
      
      vtkOutput->InsertNextCell (VTK_POLY_LINE, npt, newLine);
      
      //     delete [] newLine;
      
      cellId = lines->GetNextCell (npt, pto);
    }
    
    vtkOutput->SetPoints (bundlePoints);
    vtkOutput->GetPointData()->SetScalars (colors);
    
    
    vtkPolyDataWriter* writer = vtkPolyDataWriter::New();
    writer->SetFileName (output);
    writer->SetInputData (vtkOutput);
    writer->Update();
    
    reader1->Delete();
    reader2->Delete();
    vtkOutput->Delete();
    bundlePoints->Delete();
    writer->Delete();
    colors->Delete();
    
    
    return 0;
    
  }
  
}
