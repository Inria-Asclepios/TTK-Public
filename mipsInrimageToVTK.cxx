#include "GetPot.h"

#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsWriter.h>

#include <inrimage/Inrimage.h>

using namespace yav;

void print_help(const char* exec)
{

  std::cout << std::endl;
  std::cout << exec << ": " << std::endl;
  std::cout << "-i [Inrimage]" << std::endl;
  std::cout << "-o [VTK File]" << std::endl;
  exit(0);

}

int main(int narg, char* arg[])
{

  GetPot   cl(narg, arg); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") ) 
    print_help(cl[0]);

  const bool IsInputPresent = cl.search(2,"-i","-I");
  const bool IsOutputPresent = cl.search(2,"-o","-O");
  
   if(!IsInputPresent || !IsOutputPresent)
    {
      std::cerr << "Input and/or output not set" << std::endl;
      return -1;
    }
   
   const char* tensorsFile = cl.follow("NoFile", 2, "-i","-I");
   const char* output = cl.follow("NoFile", 2, "-o","-O");
   
   if(strcmp(tensorsFile,"NoFile")==0 || strcmp(output,"NoFile")==0)
     {
       std::cerr << "Input file and/or output not set" << std::endl;
       return -1;
     }
   

   
   Inrimage* tens;
   try
     {
       tens=new Inrimage(tensorsFile);
       if(tens->getVdim() !=6 || tens->getVectorialMode() != VM_INTERLACED)
	 {
	    delete tens;
	    return -1;
	  }
     }
   catch(...)
     {
       return -1;
     }
   std::cout << "Done." << std::endl;
   

   tens->convertCast(Inrimage::WT_DOUBLE_VECTOR);
   int numTensors=tens->getX()*tens->getY()*tens->getZ();
   
   vtkStructuredPoints *vtkTens=vtkStructuredPoints::New();
   vtkDoubleArray *tensArray=vtkDoubleArray::New();
   tensArray->SetNumberOfComponents(9);
   tensArray->SetNumberOfTuples(numTensors);
   double* buffer = (double*)tens->getData();

   for(int i=0; i<numTensors; i++) {
     int offset = i*6;
     double t[9] = {buffer[offset+0], buffer[offset+1], buffer[offset+3],
		    buffer[offset+1], buffer[offset+2], buffer[offset+4],
		    buffer[offset+3], buffer[offset+4], buffer[offset+5]};
     tensArray->SetTuple(i, t);
    }
   
   int dims[3] = {tens->getX(), tens->getY(), tens->getZ()};
   double spacing[3] = {tens->getVX(), tens->getVY(), tens->getVZ()};
   
   delete tens;
   
   vtkTens->SetDimensions(dims);
   vtkTens->SetSpacing(spacing);
   vtkTens->GetPointData()->SetTensors(tensArray);
   vtkTens->Update();
   tensArray->Delete();
   
   vtkStructuredPointsWriter* writer = vtkStructuredPointsWriter::New();
   writer->SetFileName(output);
   writer->SetInput(vtkTens);
   writer->SetFileTypeToBinary();
   writer->Update();

   vtkTens->Delete();
   
  return 0;
}
