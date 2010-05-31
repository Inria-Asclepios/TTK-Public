#include "itkDTIEstimatorCommand.h"

#include "itkDTIEstimatorTensorImageFilter.h"
#include "itkRemoveNonPositiveTensorsTensorImageFilter.h"
#include "itkTensorImageIO.h"
#include <itkImageFileReader.h>
#include <itkExtractImageFilter.h>
#include <itkImage.h>
#include "itkNumericTraits.h"

#include "GetPot.h"

namespace itk
{

  DTIEstimatorCommand::DTIEstimatorCommand()
  {
    m_ShortDescription = "Estimate tensors using a linear least-square approach";
    m_LongDescription = "Usage: estimate\n";
    m_LongDescription += "-i [List of 3D DWIs (.txt) or 4D image]\n";
    m_LongDescription += "-g [List of gradients]\n";
    m_LongDescription += "-b [Background Suppression Threshold]\n";
    m_LongDescription += "-r [NPT removing type: 0: null tensor / 1: mean of neighbors]\n";
    m_LongDescription += "-o [Output file]\n\n";
    m_LongDescription += m_ShortDescription;
  }

  DTIEstimatorCommand::~DTIEstimatorCommand()
  {}
  
  int DTIEstimatorCommand::Execute(int narg, const char *arg[])
  {
    
    GetPot   cl(narg, const_cast<char**>(arg)); // argument parser
    if( cl.size() == 1 || cl.search(2, "--help", "-h") )
    {
      std::cout << this->GetLongDescription() << std::endl;
      return -1;
    }
    
    const bool IsInputPresent    = cl.search(2,"-i","-I");
    const bool IsOutputPresent   = cl.search(2,"-o","-O");
    const bool IsGradientPresent = cl.search(2,"-g","-G");
    
    if(!IsInputPresent || !IsOutputPresent || !IsGradientPresent)
    {
      std::cerr << "Error: Input and (or) output and (or) gradients not set." << std::endl;
      return -1;
    }
    
    const char* fileIn = cl.follow("NoFile",2,"-i","-I");
    const char* fileOut = cl.follow("NoFile",2,"-o","-O");
    const char* fileGrad = cl.follow("NoFile",2,"-g","-G");
    const double bst = cl.follow(0.0,2,"-b","-B");
    const int RemovingType = cl.follow (0,2,"-r","-R");
    
    typedef double ScalarType;  
    typedef itk::Image<ScalarType, 3>                      ImageType;
    typedef itk::Image<ScalarType, 4>                      Image4DType;
    
    typedef itk::TensorImageIO<ScalarType, 3, 3>           IOType;
    typedef IOType::TensorImageType                        TensorImageType;    
    typedef itk::ImageFileReader<ImageType>                ReaderType;
    typedef itk::ImageFileReader<Image4DType>              Reader4DType;
    
    typedef itk::DTIEstimatorTensorImageFilter<ImageType, TensorImageType>
      EstimatorType;  
    typedef EstimatorType::GradientType GradientType;
    typedef EstimatorType::GradientListType GradientListType;
    typedef itk::RemoveNonPositiveTensorsTensorImageFilter<TensorImageType,TensorImageType>
      RemoveNPTFilterType;
    
    typedef Image4DType::RegionType Region4dType;
    typedef Image4DType::SpacingType Spacing4Dtype;
    
    typedef itk::ImageRegionIterator<Image4DType> Iterator4DType;
    typedef Iterator4DType::IndexType Index4DType;
    typedef ImageType::DirectionType Direction3Dtype;
    typedef Image4DType::DirectionType Direction4Dtype;
    
    // Create the filter
    EstimatorType::Pointer myEstimator = EstimatorType::New();
    myEstimator->SetBST(bst);
    
    if (strcmp (itksys::SystemTools::GetFilenameLastExtension (fileIn).c_str(), ".txt") )
    {
      
      itk::ImageFileReader<Image4DType>::Pointer reader 
	= itk::ImageFileReader<Image4DType>::New();
      reader->SetFileName (fileIn);
      
      try
      {
	std::cout << "reading: " << fileIn << std::endl;
	reader->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return EXIT_FAILURE;
      }  
      
      // here we look for the 4th dimension:
      Image4DType::Pointer image = reader->GetOutput();
      Image4DType::SizeType size = image->GetLargestPossibleRegion().GetSize();
      unsigned int nVolumes = size[3];
      
    // split the 4D volume into 3D volumes
      Image4DType::RegionType regionToExtract = image->GetLargestPossibleRegion();
      Image4DType::IndexType index;
      index[0] = 0;index[1] = 0;index[2] = 0;index[3] = 0;size[3] = 0;
      regionToExtract.SetSize (size);
      regionToExtract.SetIndex (index);
      
      if( !nVolumes)
      {
	return EXIT_FAILURE;
      }

      for( unsigned int n=0; n<nVolumes;  n++)
      {
	typedef itk::ExtractImageFilter<Image4DType, ImageType> ExtractImageType;
	regionToExtract.SetIndex (3,n);
	ExtractImageType::Pointer myExtractor = ExtractImageType::New();
	myExtractor->SetExtractionRegion (regionToExtract);
	myExtractor->SetInput (image);
	
	try
	{
	  myExtractor->Update();
	}
	catch (itk::ExceptionObject &e)
	{
	  std::cerr << e;
	  return EXIT_FAILURE;
	}
	
	myEstimator->SetInput(n,myExtractor->GetOutput());
      }
    }
    else
    {
      
      // read the fileIn
      std::ifstream file(fileIn,std::ifstream::in);
      // first number must be the num. of images:
      int numOfImages=0;
      file >> numOfImages;
      std::cout << "Number of Images: " << numOfImages << std::endl;
      
      for (int i=0; i<numOfImages; i++)
      {
	char filename[256];
	file >> filename;
	ReaderType::Pointer myReader = ReaderType::New();
	myReader->SetFileName(filename);
	std::cout << "Reading " << filename << std::flush;
	try
	{
	  myReader->Update();
	}
	catch(itk::ExceptionObject &e)
	{
	  std::cerr << e;
	  return EXIT_FAILURE;
	}
	std::cout << " Done." << std::endl;
	myEstimator->SetInput(i, myReader->GetOutput());
      }
    }
    

  
    // read the fileGrad
    std::ifstream fileg(fileGrad,std::ifstream::in);
    // first: must be the number of gradients
    int numOfGrad = 0;
    fileg >> numOfGrad;
    
    for(int i=0;i<numOfGrad;i++)
    {
      GradientType g;
      fileg >> g[0];
      fileg >> g[1];
      fileg >> g[2];
      std::cout << g << std::endl;
      if (itk::NumericTraits<double>::IsPositive (g.GetNorm()))
	myEstimator->AddGradient(g);
    }
    
    
    RemoveNPTFilterType::Pointer myRemover = RemoveNPTFilterType::New();
    myRemover->SetInput (myEstimator->GetOutput());
    myRemover->SetRemovingType (RemovingType);
    
    // now: filter
    std::cout << "Estimating..." << std::flush;
    try
    {
      myRemover->Update();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return EXIT_FAILURE;
    }
    std::cout << "Done." << std::endl;
    
    // write the output
    IOType::Pointer myWriter = IOType::New();
    myWriter->SetFileName(fileOut);
    myWriter->SetInput (myRemover->GetOutput());
    
    std::cout << "Writing..." << std::flush;
    try
    {
      myWriter->Write();
    }
    catch(itk::ExceptionObject &e)
    {
      std::cerr << e;
      return EXIT_FAILURE;
    }
    std::cout << "Done." << std::endl;
    
    return 0;

  }

}
