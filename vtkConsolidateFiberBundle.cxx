#include <iostream>

#include "GetPot.h"

#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkUnsignedCharArray.h>





void PrintHelp (const char* exec)
{

  std::cout << "Usage:\n";
  std::cout << exec << ": -i [fiber bundle]\n";
  std::cout << "-a [all fibers]\n";
  std::cout << "-o [output fiber bundle]" << std::endl;
  
}


int main (int narg, char* arg[])
{

  GetPot   cl(narg, arg); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") )
  {
    PrintHelp(cl[0]);
    return 0;
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
  writer->SetInput (vtkOutput);
  writer->Update();
  
  reader1->Delete();
  reader2->Delete();
  vtkOutput->Delete();
  bundlePoints->Delete();
  writer->Delete();
  colors->Delete();
  
  
  return 0;
  
}
