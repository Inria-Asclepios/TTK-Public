#include "itkDTIEstimatorCommand.h"

#include "itkDTIEstimatorTensorImageFilter.h"
#include "itkRemoveNonPositiveTensorsTensorImageFilter.h"
#include "itkTensorImageIO.h"
#include <itkImageFileReader.h>
#include <itkExtractImageFilter.h>
#include <itkImage.h>
#include "itkNumericTraits.h"

#include "itkGradientFileReader.h"

#include "GetPot.h"

namespace itk
{
    struct argument
    {
        const char *input;
        const char *gradients;
        const char *output;
        double bst;
        int removeType;
    };

    template <class TImage>
            int DTIEstimatorCommandImplementation(const argument &arg)
    {
        typedef TImage                        ImageType;
        typedef typename ImageType::PixelType ScalarType;
        typedef itk::Image<ScalarType, 4>     Image4DType;

        typedef itk::TensorImageIO<float, 3, 3>            IOType;
        typedef typename IOType::TensorImageType           TensorImageType;
        typedef itk::ImageFileReader<ImageType>            ReaderType;
        typedef itk::ImageFileReader<Image4DType>          Reader4DType;

        typedef itk::DTIEstimatorTensorImageFilter<ImageType, TensorImageType>
                EstimatorType;
        typedef typename EstimatorType::GradientType GradientType;
        typedef typename EstimatorType::GradientListType GradientListType;
        typedef itk::RemoveNonPositiveTensorsTensorImageFilter<TensorImageType,TensorImageType>
                RemoveNPTFilterType;

        typedef typename Image4DType::RegionType  Region4dType;
        typedef typename Image4DType::SpacingType Spacing4Dtype;

        typedef itk::ImageRegionIterator<Image4DType> Iterator4DType;
        typedef typename Iterator4DType::IndexType Index4DType;
        typedef typename ImageType::DirectionType Direction3Dtype;
        typedef typename Image4DType::DirectionType Direction4Dtype;

        const char *fileIn = arg.input;
        const char *fileOut = arg.output;
        const char *fileGrad = arg.gradients;
        double bst = arg.bst;
        int RemovingType = arg.removeType;

        // Create the filter
        try
        {
            typename EstimatorType::Pointer myEstimator = EstimatorType::New();
            myEstimator->SetBST(bst);

            if (strcmp (itksys::SystemTools::GetFilenameLastExtension (fileIn).c_str(), ".txt") )
            {
                typename itk::ImageFileReader<Image4DType>::Pointer reader
                        = itk::ImageFileReader<Image4DType>::New();
                reader->SetFileName (fileIn);

                std::cout << "reading: " << fileIn << std::endl;
                reader->Update();

                // here we look for the 4th dimension:
                typename Image4DType::Pointer image = reader->GetOutput();
                typename Image4DType::SizeType size = image->GetLargestPossibleRegion().GetSize();
                unsigned int nVolumes = size[3];

                // split the 4D volume into 3D volumes
                typename Image4DType::RegionType regionToExtract = image->GetLargestPossibleRegion();
                typename Image4DType::IndexType index;
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
                    typename ExtractImageType::Pointer myExtractor = ExtractImageType::New();
                    myExtractor->SetExtractionRegion (regionToExtract);
                    myExtractor->SetInput (image);
                    myExtractor->SetDirectionCollapseToGuess();

                    myExtractor->Update();

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
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); /* ignore until next line */

                for (int i=0; i<numOfImages; i++)
                {
                    char filename[256];
                    file.getline(filename,256);
                    typename ReaderType::Pointer myReader = ReaderType::New();
                    myReader->SetFileName(filename);
                    std::cout << "Reading " << filename << std::flush;
                    myReader->Update();
                    std::cout << " Done." << std::endl;
                    myEstimator->SetInput(i, myReader->GetOutput());
                }
            }

            // read the fileGrad
            typename GradientFileReader::Pointer gradientreader = GradientFileReader::New();
            gradientreader->SetFileName (fileGrad);

            std::cout << "reading: " << fileGrad << std::endl;
            gradientreader->Update();

            typename GradientFileReader::VectorListType gradients = gradientreader->GetGradientList();

            if (gradients.size() == myEstimator->GetNumberOfIndexedInputs()-1)
            {
                // add the null gradient in this case
                typename GradientFileReader::VectorListType auxGradients;
                auxGradients.push_back( GradientType(0.0) );
                for (unsigned int i=0; i<gradients.size(); i++)
                    auxGradients.push_back(gradients[i]);
                gradients = auxGradients;
            }

            for(unsigned int i=0; i<gradients.size(); i++)
            {
                GradientType g = gradients[i];
                std::cout << g << std::endl;
                myEstimator->AddGradient(g);
            }

            typename RemoveNPTFilterType::Pointer myRemover = RemoveNPTFilterType::New();
            myRemover->SetInput (myEstimator->GetOutput());
            myRemover->SetRemovingType (RemovingType);

            // now: filter
            std::cout << "Estimating..." << std::flush;
            myRemover->Update();
            std::cout << "Done." << std::endl;

            // write the output
            typename IOType::Pointer myWriter = IOType::New();
            myWriter->SetFileName(fileOut);
            myWriter->SetInput (myRemover->GetOutput());

            std::cout << "Writing..." << std::flush;
            myWriter->Write();
            std::cout << "Done." << std::endl;

        }
        catch (itk::ExceptionObject &e)
        {
            std::cerr << e;
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

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
    {
    }

    int DTIEstimatorCommand::Execute(int argc, const char *argv[])
    {
        GetPot   cl(argc, const_cast<char**>(argv)); // argument parser
        if( cl.size() == 1 || cl.search(2, "--help", "-h") )
        {
            std::cout << this->GetLongDescription() << std::endl;
            return EXIT_FAILURE;
        }

        argument arg;
        arg.input      = cl.follow("","-i");
        arg.output     = cl.follow("","-o");
        arg.gradients  = cl.follow("","-g");
        arg.bst        = cl.follow(0.0,"-b");
        arg.removeType = cl.follow(0,"-r");

        itk::ImageIOBase::Pointer io = itk::ImageIOFactory::CreateImageIO(arg.input, IOFileModeEnum::ReadMode);
        if (io.IsNull())
        {
            if (strcmp (itksys::SystemTools::GetFilenameLastExtension (arg.input).c_str(), ".txt") ==0)
            {
                std::cout << "Reading text file " << std::endl;

                std::ifstream file(arg.input,std::ifstream::in);
                if( file.fail() )
                {
                    std::cerr << "Cannot open file " << arg.input << std::endl;
                    return EXIT_FAILURE;
                }

                int numOfImages=0;
                file >> numOfImages;
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); /* ignore until next line */
                char filename[256];
                file.getline(filename,256);
                std::cout << "Checking type of " << filename << std::endl;

                io = itk::ImageIOFactory::CreateImageIO(filename, IOFileModeEnum::ReadMode);
                if (io.IsNull())
                {
                    std::cerr << "Unknown file type for " << filename << std::endl;
                    return EXIT_FAILURE;
                }
		io->SetFileName (filename);
            }
            else
            {
                std::cerr << "Unknown file type" << std::endl;
                return EXIT_FAILURE;
            }
        }
	else
	  io->SetFileName(arg.input);

        try
        {
            io->ReadImageInformation();
        }
        catch(itk::ExceptionObject &e)
        {
            std::cerr << e;
            return EXIT_FAILURE;
        }

        switch( io->GetComponentType())
        {
        case IOComponentEnum::UCHAR:
            return DTIEstimatorCommandImplementation< itk::Image<unsigned char, 3> >(arg);

        case IOComponentEnum::CHAR:
            return DTIEstimatorCommandImplementation< itk::Image<char, 3> >(arg);

        case IOComponentEnum::USHORT:
            return DTIEstimatorCommandImplementation< itk::Image<unsigned short, 3> >(arg);

        case IOComponentEnum::SHORT:
            return DTIEstimatorCommandImplementation< itk::Image<short, 3> >(arg);

        case IOComponentEnum::UINT:
            return DTIEstimatorCommandImplementation< itk::Image<unsigned int, 3> >(arg);

        case IOComponentEnum::INT:
            return DTIEstimatorCommandImplementation< itk::Image<int, 3> >(arg);

        case IOComponentEnum::ULONG:
            return DTIEstimatorCommandImplementation< itk::Image<unsigned long, 3> >(arg);

        case IOComponentEnum::LONG:
            return DTIEstimatorCommandImplementation< itk::Image<long, 3> >(arg);

        case IOComponentEnum::FLOAT:
            return DTIEstimatorCommandImplementation< itk::Image<float, 3> >(arg);

        case IOComponentEnum::DOUBLE:
            return DTIEstimatorCommandImplementation< itk::Image<double, 3> >(arg);

        default:
            std::cerr << "unsupported component type: " << io->GetComponentTypeAsString( io->GetComponentType() );
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }
}
