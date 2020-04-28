#include "itkDTIMultiEstimatorCommand.h"
#include "GetPot.h"

namespace itk
{
  
  DTIMultiEstimatorCommand::DTIMultiEstimatorCommand()
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

  DTIMultiEstimatorCommand::~DTIMultiEstimatorCommand()
  {}
  
  int DTIMultiEstimatorCommand::Execute(int narg, const char *arg[])
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
    
    // read the fileGrad
    GradientFileReader::Pointer gradientreader = GradientFileReader::New();
    gradientreader->SetFileName (fileGrad);
    try
    {
      gradientreader->Update();
    }
    catch (itk::ExceptionObject &e)
    {
      std::cerr << e;
      return EXIT_FAILURE;
    }
    GradientFileReader::VectorListType gradients = gradientreader->GetGradientList();


    // perform size checks
    if( !nVolumes || (nVolumes > gradients.size()))
    {
      std::cerr << "no Volume, or not enougth gradients accordingly " << std::endl;      
      return EXIT_FAILURE;
    }
    if( nVolumes <= 6)
    {
      std::cerr << "Not enougth volumes to reconstruct (any) tensor field" << std::endl;      
      return EXIT_FAILURE;
    }

    
    // split the 4D volume into 3D volumes
    typedef itk::ExtractImageFilter<Image4DType, ImageType> ExtractImageType;
    Image4DType::RegionType regionToExtract = image->GetLargestPossibleRegion();
    Image4DType::IndexType index;
    index[0] = 0;index[1] = 0;index[2] = 0;index[3] = 0;size[3] = 0;
    regionToExtract.SetSize (size);
    regionToExtract.SetIndex (index);

    ListOfDWIType DWIs;
    
    for( unsigned int n=0; n<nVolumes;  n++)
    {
      regionToExtract.SetIndex (3,n);
      ExtractImageType::Pointer iExtractor = ExtractImageType::New();
      iExtractor->SetExtractionRegion (regionToExtract);
      iExtractor->SetInput (image);
      
      try
      {
	iExtractor->Update();
      }
      catch (itk::ExceptionObject &e)
      {
	std::cerr << e;
	return EXIT_FAILURE;
      }
      
      DWIType dwi;
      dwi.first = gradients[n];
      dwi.second = iExtractor->GetOutput();
      DWIs.push_back (dwi);
    }

    std::cout<<"the DWIs list contains "<<DWIs.size()<<" pairs"<<std::endl;
    
    // now we need all possible combinations including the b-0 (should be the first volume)
    // and containing N>=6 projections
    // std::vector<ListOfDWIType> listofpossibilities = GetAllPossibleDWILists (DWIs);
    std::vector<ListOfDWIType> listofpossibilities = GetDWIListsWithOneRemoval (DWIs);

    std::cout<<"performing all "<<listofpossibilities.size()<<" possible tensor reconstructions"<<std::endl;
    
    for (unsigned int i=0; i<listofpossibilities.size(); i++)
    {
      
      ListOfDWIType dwis = listofpossibilities[i];

      // Create the estimator
      EstimatorType::Pointer iEstimator = EstimatorType::New();
      iEstimator->SetBST(bst);

      std::cout<<"adding first pair DWIs[0].first and .second "<<std::endl;
      GradientType gnull = DWIs[0].first;
      //iEstimator->AddGradient (gnull);
      iEstimator->SetInput (0, DWIs[0].second);
      
      std::cout<<"the dwis list contains "<<dwis.size()<<" pairs"<<std::endl;

      for (unsigned int n=0; n<dwis.size(); n++)
	std::cout<<dwis[n].first<<std::endl;
      
      for (unsigned int j=0; j<dwis.size(); j++)
      {
	GradientType g = dwis[j].first;
	ImageType::Pointer dwi = dwis[j].second;
	if (itk::NumericTraits<double>::IsPositive (g.GetNorm()))
	  iEstimator->AddGradient (g);
	else
	  std::cout<<"the "<<j<<"est gradient is NULL, which is weird"<<std::endl;
	iEstimator->SetInput (j+1, dwi);
      }

      RemoveNPTFilterType::Pointer iRemover = RemoveNPTFilterType::New();
      iRemover->SetInput (iEstimator->GetOutput());
      iRemover->SetRemovingType (RemovingType);
      iRemover->Update();
      
      // write the output
      IOType::Pointer iWriter = IOType::New();
      std::ostringstream os;
      os <<fileOut<<"-";
      if (i < 10)
	os<<"0";
      os<<i<<".mha";
      
      iWriter->SetFileName(os.str().c_str());
      iWriter->SetInput (iRemover->GetOutput());
      iWriter->Write();
    }
    
    std::cout << "Done." << std::endl;
    
    return 0;

  }

  std::vector<DTIMultiEstimatorCommand::ListOfDWIType> DTIMultiEstimatorCommand::GetAllPossibleDWILists (ListOfDWIType input)
  {
    ListOfDWIType dwis;
    for (unsigned int i=1; i<input.size(); i++)
      dwis.push_back (input[i]);
    
    std::vector<ListOfDWIType> ret;
    
    unsigned int starting_n = dwis.size() - 1;

    for (unsigned int n = starting_n; n < dwis.size(); n++)
    {
      std::vector<ListOfDWIType> subsets = GetAllNSubsets (dwis, n);
      
      for (unsigned int s = 0; s < subsets.size(); s++)
	ret.push_back (subsets[s]);
    }
    
    ret.push_back (dwis);
    return ret;
  }

  std::vector<DTIMultiEstimatorCommand::ListOfDWIType> DTIMultiEstimatorCommand::GetDWIListsWithOneRemoval (ListOfDWIType input)
  {
    ListOfDWIType dwis;
    std::cout<<"size of dwis enter : "<<dwis.size()<<std::endl;
    
    for (unsigned int i=1; i<input.size(); i++)
      dwis.push_back (input[i]);
        std::cout<<"size of dwis out : "<<dwis.size()<<std::endl;

    std::vector<ListOfDWIType> ret;
    
    for (unsigned int n = 0; n < dwis.size(); n++)
    {
      ListOfDWIType subset;
      for (unsigned int m = 0; m < dwis.size(); m++)
      {
	if (n == m)
	  continue;
	subset.push_back (dwis[m]);
      }	

      std::cout<<"size of dwis subset : "<<subset.size()<<std::endl;
 
      ret.push_back (subset);
    }
    
    
    //ret.push_back (dwis);
    return ret;
  }

  std::vector< DTIMultiEstimatorCommand::ListOfDWIType > DTIMultiEstimatorCommand::GetAllNSubsets (ListOfDWIType input, unsigned int outputsize)
  {
    std::vector< ListOfDWIType  > result;
    
    if (outputsize < 6 )
      return result;
    
    for (unsigned int i=0; i<1; i++)
    {
      ListOfDWIType list;
      for (unsigned int j=0; j<outputsize; j++)
	list.push_back (input[input.size() - 1 - j]);

      result.push_back (list);
    }
    
    return result;
  }
  
}
