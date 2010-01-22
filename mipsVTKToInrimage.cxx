#include "GetPot.h"

#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>

#include <linearAlgebra/Vec3.h>
#include <inrimage/Inrimage.h>


using namespace yav;

void print_help(const char* exec)
{

  std::cout << std::endl;
  std::cout << exec << ": " << std::endl;
  std::cout << "-i [VTK File]" << std::endl;
  std::cout << "-o [Inrimage]" << std::endl;
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



   // read the vtk file
   vtkStructuredPointsReader* myReader = vtkStructuredPointsReader::New();
   myReader->SetFileName(tensorsFile);
   if( !myReader->IsFileStructuredPoints())
   {
     std::cerr << "File: " << tensorsFile << " is not of type vtkStructuredPoints" << std::endl;
     exit (-1);     
   }
   
   vtkStructuredPoints* vtkTens = myReader->GetOutput();
   vtkTens->Update();

   
   // create an inrimage and save it
   vtkDoubleArray *array=(vtkDoubleArray *)vtkTens->GetPointData()->GetTensors();
   int dims[3];
   double spacing[3];
   double origin[3];
   vtkTens->GetSpacing(spacing);
   vtkTens->GetDimensions(dims);
   vtkTens->GetOrigin (origin);   
   cout<<"dims = "<<dims[0]<<" "<<dims[1]<<" "<<dims[2]<<endl;
   Inrimage *inr=new Inrimage(dims[0],
                              dims[1],
                              dims[2],
                              Inrimage::WT_DOUBLE_VECTOR,
                              6,
                              VM_INTERLACED,
                              spacing[0],
                              spacing[1],
                              spacing[2]
                              );

   // Completement debile: on est oblige de passer par
   // la structure Vec3 pour mettre l'origine de l'image
   // a jour. Y'en a qui ont encore pense tres fort pour
   // nous pondre quelque chose comme ca.
    Vec3<double> orig;
    orig[0] = origin[0];
    orig[1] = origin[1];
    orig[2] = origin[2];
    inr->setTranslation(orig);

        
   double *dt=(double*)(inr->getData());
   cout<<"Converting"<<endl;
   for(int i=0; i<dims[0]*dims[1]*dims[2]; i++)
   {
     double t[9];
     array->GetTuple(i, t);

     dt[i*6+0] = t[0];
     dt[i*6+1] = t[3];
     dt[i*6+2] = t[4];
     dt[i*6+3] = t[6];
     dt[i*6+4] = t[7];
     dt[i*6+5] = t[8];

   }


   inr->write (output);
      
  return 0;
}
