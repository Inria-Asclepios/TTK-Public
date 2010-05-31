#include <iostream>

#include "GetPot.h"

#include "itkTensorImageIO.h"
#include "itkCrossingTensorImageSource.h"
#include "itkCircleTensorImageSource.h"


void PrintHelp (const char* exec)
{
  std::cout << "Usage:\n"
	    << exec << "\n-o <output file>\n"
    	    << "-x <dim_x>\n"
	    << "-y <dim_y>\n"
	    << "-z <dim_z>\n"
	    << "-sx <spacing_x>\n"
	    << "-sy <spacing_y>\n"
	    << "-sz <spacing_z>\n"
    	    << "-ox <origin_x>\n"
	    << "-oy <origin_y>\n"
	    << "-oz <origin_z>\n"
	    << "-t <type=crossing/circle>\n"
	    << "-c <number of crossings - default: 1>\n"
	    << "-r1 <radius1 - default: 1.0>\n"
	    << "-r2 <radius2 - default: 2.0>\n"
	    << std::endl;
}


int main (int narg, char* arg[])
{

  GetPot cl (narg, arg);
  if( cl.size()==1 || cl.search (2,"-h","--help") )
  {
    PrintHelp (cl[0]);
    return -1;
  }

  const char* output = cl.follow ("tensors.inr.gz", 2, "-o", "-O");
  const char* type = cl.follow ("crossing", 2, "-t", "-T");
  const int crossings = cl.follow (1, 2, "-c", "-C");
  const double r1 = cl.follow (1.0, 2, "-r1", "-R1");
  const double r2 = cl.follow (1.0, 2, "-r2", "-R2");

  int sizex = cl.follow (16, 2, "-x", "-X");
  int sizey = cl.follow (16, 2, "-y", "-Y");
  int sizez = cl.follow (16, 2, "-z", "-Z");

  double spacingx = cl.follow (1.0, 2, "-sx", "-SX");
  double spacingy = cl.follow (1.0, 2, "-sy", "-SY");
  double spacingz = cl.follow (1.0, 2, "-sz", "-SZ");

  double originx = cl.follow (0, 2, "-ox", "-OX");
  double originy = cl.follow (0, 2, "-oy", "-OY");
  double originz = cl.follow (0, 2, "-oz", "-OZ");

  
  typedef itk::TensorImageIO<double, 3> TensorIOType;
  typedef TensorIOType::TensorImageType TensorImageType;
  typedef itk::CrossingTensorImageSource<TensorImageType>  CrossingTensorSourceType;
  typedef itk::CircleTensorImageSource<TensorImageType>    CircleTensorSourceType;

  TensorImageType::PointType   origin;
  TensorImageType::SpacingType spacing;
  TensorImageType::SizeType    size;

  origin[0] = originx;
  origin[1] = originy;
  origin[2] = originz;

  spacing[0] = spacingx;
  spacing[1] = spacingy;
  spacing[2] = spacingz;

  size[0] = sizex;
  size[1] = sizey;
  size[2] = sizez; 


  TensorImageType::Pointer tensorImage = 0;
    
  if( strcmp (type, "crossing")==0 )
  {
    CrossingTensorSourceType::Pointer mySource = CrossingTensorSourceType::New();
    mySource->SetNumberOfCrossings ( crossings );
    mySource->SetOrigin ( origin );
    mySource->SetSpacing ( spacing );
    mySource->SetSize ( size );
    mySource->SetRadius (r1);
    try
    {
      mySource->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }

    tensorImage = mySource->GetOutput();
    
  }
  else if ( strcmp (type, "circle")==0 )
  {
    CircleTensorSourceType::Pointer mySource = CircleTensorSourceType::New();
    mySource->SetRadius1 ( r1 );
    mySource->SetRadius2 ( r2 );
    try
    {
      mySource->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return -1;
    }

    tensorImage = mySource->GetOutput();
  }
  else
  {
    std::cerr << "Error: type " << type << " is not recognized." << std::endl;
    return -1;
  }


  
  TensorIOType::Pointer myIO = TensorIOType::New();
  myIO->SetInput ( tensorImage );
  myIO->SetFileName(output);
  
  try
  {
    myIO->Write();
  }
  catch (itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }
  

  return 0;
  
}
