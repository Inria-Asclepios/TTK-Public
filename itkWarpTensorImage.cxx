#include <iostream>

#include "GetPot.h"
#include "itkTensorImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkWarpTensorImageFilter.h"
#include "itkLogTensorImageFilter.h"
#include "itkExpTensorImageFilter.h"

#include "ttkConfigure.h"

#ifdef TTK_USE_MIPS
#include "mipsInrimageImageIOFactory.h"
#endif

using namespace itk;

void PrintHelp( const char* exec )
{
	std::cout << "Usage: " << std::endl;
	std::cout << exec << " <-i input> <-d deformation field> <-o output> <-t reorientation strategy (1 for FS or 2 for PPD)>" << std::endl;
	exit (0);
}


int main( int narg, char* arg[])
{

itk::Object::GlobalWarningDisplayOff();

	GetPot cl(narg, arg); // argument parser
  if( cl.size() == 1 || cl.search(2, "--help", "-h") )
  {
    PrintHelp(cl[0]);
  }
  

  const char* input = cl.follow("NoFile",2,"-I","-i");
  const char* output = cl.follow("NoFile",2,"-O","-o");
  const char* def = cl.follow("NoFile",2,"-d","-D");
  int reostrat = cl.follow(0,2,"-t","-T");
  

  typedef double                                ScalarType;  
  typedef itk::TensorImageIO<ScalarType, 3, 3>  IOType;
  typedef IOType::TensorImageType               TensorImageType;
  typedef Vector<ScalarType, 3>                 VectorType;
  typedef Image<VectorType, 3>                  DeformationFieldType;

  typedef LogTensorImageFilter<TensorImageType,TensorImageType> LogFilterType;
  typedef ExpTensorImageFilter<TensorImageType,TensorImageType> ExpFilterType;
 

  std::cout << "Reading: " << input << std::flush;
  IOType::Pointer reader = IOType::New();
  reader->SetFileName( input );
  try
  {
	reader->Read();
  }
  catch( itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }
  std::cout << " Done." << std::endl;

  TensorImageType::Pointer Input = reader->GetOutput();


  std::cout << "Loging..." << std::endl;
  LogFilterType::Pointer loger = LogFilterType::New();
  loger->SetInput ( Input );
  try
  {
    loger->Update();
  }
  catch( itk::ExceptionObject &e)
  {
    std::cerr << e;
    return -1;
  }
  std::cout << "Done." << std::endl;
  
#ifdef TTK_USE_MIPS
  itk::InrimageImageIOFactory::RegisterOneFactory();
#endif

  std::cout << "Reading: " << def << std::flush;
  itk::ImageFileReader<DeformationFieldType>::Pointer reader3 = 
		itk::ImageFileReader<DeformationFieldType>::New();
  reader3->SetFileName( def );
  try
  {
	reader3->Update();
  }
  catch( itk::ExceptionObject &e)
  {
	  std::cerr << e;
	  return -1;
  }
 
  DeformationFieldType::Pointer Deformation = reader3->GetOutput();

std::cout << " Done." << std::endl;


	// warp the result
   typedef itk::WarpTensorImageFilter
      < TensorImageType, TensorImageType, DeformationFieldType >  WarperType;
   WarperType::Pointer warper = WarperType::New();
   warper->SetInput( loger->GetOutput() );
   warper->SetOutputSpacing( Deformation->GetSpacing() );
   warper->SetOutputOrigin( Deformation->GetOrigin() );
   warper->SetOutputDirection( Deformation->GetDirection() );
   warper->SetDeformationField( Deformation );

   switch (reostrat)
   {
     case 1:
      warper->SetReorientationStrategy(FS);
      std::cout << "Reorientation Strategy: Finite Strain (FS)" << std::endl;
      break;

    case 2:
      warper->SetReorientationStrategy(PPD);
      std::cout << "Reorientation Strategy: Preservation of Principal Direction (PPD)" << std::endl;
      break;

    default:
      warper->SetReorientationStrategy(FS);
      std::cout << "Reorientation Strategy: Finite Strain (FS)" << std::endl;
      break;
   }

      std::cout << "Warping: " << std::flush;
   try
   {
	warper->Update();
   }
catch( itk::ExceptionObject &e)
  {
	  std::cerr << e;
	  return -1;
  }
std::cout << " Done." << std::endl;

 
 std::cout << "Exping..." << std::endl;
 ExpFilterType::Pointer exper = ExpFilterType::New();
 exper->SetInput ( warper->GetOutput() );
 try
 {
   exper->Update();
 }
 catch( itk::ExceptionObject &e)
 {
   std::cerr << e;
   return -1;
 }
 std::cout << " Done." << std::endl; 
 
  
std::cout << "Writing: " << output << std::flush;
IOType::Pointer writer = IOType::New();
writer->SetInput( exper->GetOutput() );
writer->SetFileName( output );

try
{
	writer->Write();
}
catch( itk::ExceptionObject &e)
  {
	  std::cerr << e;
	  return -1;
  }
std::cout << " Done." << std::endl;

	return 0;
}
