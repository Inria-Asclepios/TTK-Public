/**
 * This is a small tool that assesses the results of a tensor image
 * registration. It computes the error between the images, characteristics of
 * the deformation field and distance to a true deformation field if it exists.
 *
 * \author Andrew Sweet
 */

#include <itkCommand.h>
#include <itkDisplacementFieldJacobianDeterminantFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkVectorCentralDifferenceImageFunction.h>
#include <itkWarpHarmonicEnergyCalculator.h>
#include <itkWarpImageFilter.h>
#include <itkWarpHarmonicEnergyCalculator.h>
#include <itkImageIterator.h>

#include "itkTensor.h"
#include "itkTensorImageIO.h"
#include "itkExpTensorImageFilter.h"
#include "itkLogTensorImageFilter.h"
#include "itkNumericTraitsTensorPixel2.h"
#include <itkWarpTensorImageFilter.h>

#include <metaCommand.h>

#include <errno.h>
#include <iostream>
#include <limits.h>

struct arguments
{
  std::string fixedImageFile; /* -f option */
  std::string movingImageFile; /* -m option */
  std::string inputDeformationFieldFile; /* -d option */
  std::string outputResultsFile; /* -k option */
  std::string outputMSEFile; /* -o option */
  std::string outputLMSEFile; /* -O option */
  std::string trueDeformationFieldFile; /* -t option */
  unsigned int reorientationType; /* -r option */

  friend std::ostream&
  operator<<(std::ostream& o, const arguments& args)
  {
    return o << "Arguments structure:" << std::endl << "  Fixed image file: "
    << args.fixedImageFile << std::endl << "  Moving image file: "
    << args.movingImageFile << std::endl
    << "  Input deformation field file: "
    << args.inputDeformationFieldFile << std::endl
    << "Output results file: " << args.outputResultsFile << std::endl
    << "Output MSE file: " << args.outputMSEFile << std::endl
    << "Output LMSE file: " << args.outputLMSEFile << std::endl
    << "  Tensor reorientation type: " << args.reorientationType
    << std::endl << "  True deformation field file: "
    << args.trueDeformationFieldFile << std::endl;
  }
};

void
help_callback()
{
  std::cout << std::endl;
  std::cout << "Copyright (c) 2010 INRIA." << std::endl;
  std::cout << "Code: Andrew Sweet" << std::endl;
  std::cout << "Report bugs to <andrew.sweet \\at sophia.inria.fr>"
  << std::endl;

  exit(EXIT_FAILURE);
}
;

void
parseOpts(int argc, char **argv, struct arguments & args)
{
  // Command line parser
  MetaCommand command;
  command.SetParseFailureOnUnrecognizedOption(true);
  command.SetHelpCallBack(help_callback);

  // Fill some information about the software
  command.SetAuthor("Andrew Sweet.");

  command.SetAcknowledgments(
      "This work is supported by INRIA (Asclepios team).");

  command.SetDescription("Assesses a tensor image registration.");

  // Define parsing options
  command.SetOption("FixedImageFile", "f", true, "Fixed image filename");
  command.SetOptionLongTag("FixedImageFile", "fixed-image");
  command.AddOptionField("FixedImageFile", "filename", MetaCommand::STRING,
      true);

  command.SetOption("MovingImageFile", "m", true, "Moving image filename");
  command.SetOptionLongTag("MovingImageFile", "moving-image");
  command.AddOptionField("MovingImageFile", "filename", MetaCommand::STRING,
      true);

  command.SetOption("InputDeformationFieldFile", "d", true,
      "Input deformation field filename");
  command.SetOptionLongTag("InputDeformationFieldFile", "input-def-field");
  command.AddOptionField("InputDeformationFieldFile", "filename",
      MetaCommand::STRING, true);

  command.SetOption("OutputResultsFile", "k", true, "Output results filename");
  command.SetOptionLongTag("OutputResultsFile", "output-results");
  command.AddOptionField("OutputResultsFile", "filename", MetaCommand::STRING, true, "assess_results.csv");

  command.SetOption("OutputMSEFile", "o", true, "Output MSE image filename");
  command.SetOptionLongTag("OutputMSEFile", "output-mse");
  command.AddOptionField("OutputMSEFile", "filename", MetaCommand::STRING, true, "MSE.nii.gz");

  command.SetOption("OutputLMSEFile", "O", true, "Output LMSE image filename");
  command.SetOptionLongTag("OutputLMSEFile", "output-lmse");
  command.AddOptionField("OutputLMSEFile", "filename", MetaCommand::STRING, true, "LMSE.nii.gz");

  command.SetOption(
      "TrueDeformationFieldFile",
      "t",
      false,
      "Specify a \"true\" deformation field to compare the registration result with (useful for synthetic experiments)");
  command.SetOptionLongTag("TrueDeformationFieldFile", "true-def-field");
  command.AddOptionField("TrueDeformationFieldFile", "filename",
      MetaCommand::STRING, true);

  command.SetOption(
      "ReorientationType",
      "r",
      false,
      "Tensor reorientation type. 0: Finite strain (default), 1: Preservation of principal direction");
  command.SetOptionLongTag("ReorientationType", "reorientation-type");
  command.AddOptionField("ReorientationType", "type", MetaCommand::INT, true,
      "0");
  command.SetOptionRange("ReorientationType", "type", "0", "1");

  // Actually parse the command line
  if ( !command.Parse(argc, argv) )
    {
      exit(EXIT_FAILURE);
    }

  // Store the parsed information into a struct
  args.fixedImageFile = command.GetValueAsString("FixedImageFile", "filename");
  args.movingImageFile = command.GetValueAsString("MovingImageFile",
      "filename");
  args.inputDeformationFieldFile = command.GetValueAsString(
      "InputDeformationFieldFile", "filename");
  args.outputResultsFile = command.GetValueAsString("OutputResultsFile", "filename");
  args.outputMSEFile = command.GetValueAsString("OutputMSEFile", "filename");
  args.outputLMSEFile = command.GetValueAsString("OutputLMSEFile", "filename");
  args.trueDeformationFieldFile = command.GetValueAsString(
      "TrueDeformationFieldFile", "filename");
  args.reorientationType = command.GetValueAsInt("ReorientationType", "type");
}

int
main(int argc, char *argv[])
{
  struct arguments args;
  parseOpts(argc, argv, args);

  std::cout
  << "Performing tensor registration assessment with the following arguments:"
  << std::endl;
  std::cout << args << std::endl << std::endl;

  typedef float VectorRealType;
  typedef float TensorRealType;

  typedef itk::Vector<VectorRealType, 3> VectorPixelType;
  typedef itk::Tensor<TensorRealType, 3> TensorPixelType;

  typedef itk::Image<TensorRealType, 3> TensorRealImageType;
  typedef itk::Image<VectorRealType, 3> VectorRealImageType;
  typedef itk::Image<VectorPixelType, 3> DeformationFieldType;
  typedef itk::Image<TensorPixelType, 3> TensorImageType;

  typedef itk::ExpTensorImageFilter<TensorImageType, TensorImageType>
  ExpTensorFilterType;

  // FIRST WE COMPUTE STATS ON THE DEFORMATION FIELD

  // READ FOUND DEFORMATION FIELD
  typedef itk::ImageFileReader<DeformationFieldType>
  DeformationFieldReaderType;

  DeformationFieldReaderType::Pointer inputFieldReader =
    DeformationFieldReaderType::New();

  inputFieldReader->SetFileName(args.inputDeformationFieldFile.c_str());
  try
  {
    inputFieldReader->Update();
  }
  catch ( itk::ExceptionObject& err )
  {
    std::cout << "Could not read the input deformation field." << std::endl;
    std::cout << err << std::endl;
    exit(EXIT_FAILURE);
  }
  DeformationFieldType::Pointer inputDefField = inputFieldReader->GetOutput();

  // READ TRUE DEFORMATION FIELD
  DeformationFieldReaderType::Pointer trueFieldReader =
    DeformationFieldReaderType::New();
  DeformationFieldType::Pointer trueDefField = DeformationFieldType::New();
  if ( !args.trueDeformationFieldFile.empty() )
    {
      trueFieldReader->SetFileName(args.trueDeformationFieldFile.c_str());
      try
      {
        trueFieldReader->Update();
      }
      catch ( itk::ExceptionObject& err )
      {
        std::cout << "Could not read the true deformation field." << std::endl;
        std::cout << err << std::endl;
        exit(EXIT_FAILURE);
      }
      trueDefField = trueFieldReader->GetOutput();
    }

  // COMPUTE SOME STATS ON THE FOUND DEFORMATION FIELD
  typedef itk::WarpHarmonicEnergyCalculator<DeformationFieldType>
  HarmonicEnergyCalculatorType;

  HarmonicEnergyCalculatorType::Pointer harmonicEnergyCalculator =
    HarmonicEnergyCalculatorType::New();
  harmonicEnergyCalculator->SetImage(inputDefField);
  harmonicEnergyCalculator->Compute();
  const double harmonicEnergy = harmonicEnergyCalculator->GetHarmonicEnergy();

  typedef itk::DisplacementFieldJacobianDeterminantFilter<
  DeformationFieldType, VectorRealType> JacobianFilterType;
  JacobianFilterType::Pointer jacobianFilter = JacobianFilterType::New();
  jacobianFilter->SetUseImageSpacing(true);
  jacobianFilter->ReleaseDataFlagOn();

  jacobianFilter->SetInput(inputDefField);
  jacobianFilter->UpdateLargestPossibleRegion();

  const unsigned int numPix = jacobianFilter-> GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels();

  VectorRealType* pix_start = jacobianFilter->GetOutput()->GetBufferPointer();
  VectorRealType* pix_end = pix_start + numPix;
  VectorRealType* jac_ptr;

  // Get percentage of det(Jac) below 0
  unsigned int jacBelowZero(0u);
  for (jac_ptr = pix_start; jac_ptr != pix_end; ++jac_ptr)
    {
      if ( *jac_ptr <= 0.0 )
        ++jacBelowZero;
    }
  const double jacBelowZeroPrc = static_cast<double> (jacBelowZero)
  / static_cast<double> (numPix);

  // Get min an max jac
  const double minJac = *(std::min_element(pix_start, pix_end));
  const double maxJac = *(std::max_element(pix_start, pix_end));

  // Get some quantiles
  // We don't need the jacobian image
  // we can modify/sort it in place
  jac_ptr = pix_start + static_cast<unsigned int> (0.002 * numPix);
  std::nth_element(pix_start, jac_ptr, pix_end);
  const double Q002 = *jac_ptr;

  jac_ptr = pix_start + static_cast<unsigned int> (0.01 * numPix);
  std::nth_element(pix_start, jac_ptr, pix_end);
  const double Q01 = *jac_ptr;

  jac_ptr = pix_start + static_cast<unsigned int> (0.99 * numPix);
  std::nth_element(pix_start, jac_ptr, pix_end);
  const double Q99 = *jac_ptr;

  jac_ptr = pix_start + static_cast<unsigned int> (0.998 * numPix);
  std::nth_element(pix_start, jac_ptr, pix_end);
  const double Q998 = *jac_ptr;

  // COMPUTE MEAN SQUARE ERROR BETWEEN TRUE AND FOUND FIELDS
  // (AND THEIR JACOBIANS)
  double fieldDist = -1.0;
  double fieldGradDist = -1.0;
  if ( !args.trueDeformationFieldFile.empty() )
    {
      typedef itk::VectorCentralDifferenceImageFunction<DeformationFieldType>
      WarpGradientCalculatorType;

      typedef WarpGradientCalculatorType::OutputType WarpGradientType;

      WarpGradientCalculatorType::Pointer inputWarpGradientCalculator = WarpGradientCalculatorType::New();
      inputWarpGradientCalculator->SetInputImage( inputDefField );

      WarpGradientCalculatorType::Pointer trueWarpGradientCalculator = WarpGradientCalculatorType::New();
      trueWarpGradientCalculator->SetInputImage( trueDefField );

      typedef itk::ImageRegionConstIteratorWithIndex<DeformationFieldType>
      FieldIteratorType;
      FieldIteratorType inputIter( inputDefField, inputDefField->GetLargestPossibleRegion() );
      FieldIteratorType trueIter( trueDefField, inputDefField->GetLargestPossibleRegion() );

      for ( inputIter.GoToBegin(), trueIter.GoToBegin(); ! inputIter.IsAtEnd(); ++inputIter, ++trueIter )
        {
          fieldDist += (inputIter.Value() - trueIter.Value()).GetSquaredNorm();

          // No need to add Id matrix here as we do a substraction
          double tmp = (
              ( inputWarpGradientCalculator->EvaluateAtIndex(inputIter.GetIndex())
                  -trueWarpGradientCalculator->EvaluateAtIndex(trueIter.GetIndex())
              ).GetVnlMatrix() ).frobenius_norm();
          fieldGradDist += tmp*tmp;
        }
      fieldDist = sqrt(fieldDist/ (double)(
          inputDefField->GetLargestPossibleRegion().GetNumberOfPixels()));
      fieldGradDist = sqrt(fieldGradDist/ (double)(
          inputDefField->GetLargestPossibleRegion().GetNumberOfPixels()));
    }

  // FIRST WARP WITH EUC-TENSOR

  // read fixed image
  typedef itk::TensorImageIO<TensorRealType, 3, 3> TensorIOType;

  TensorIOType::Pointer fixedImageReader = TensorIOType::New();
  fixedImageReader->SetFileName(args.fixedImageFile.c_str());
  try
  {
    fixedImageReader->Read();
  }
  catch ( itk::ExceptionObject& err )
  {
    std::cout << "Could not read the fixed image." << std::endl;
    std::cout << err << std::endl;
    exit(EXIT_FAILURE);
  }
  TensorImageType::Pointer fixedImage = fixedImageReader->GetOutput();

  TensorIOType::Pointer movingImageReader = TensorIOType::New();
  movingImageReader->SetFileName(args.movingImageFile.c_str());
  try
  {
    movingImageReader->Read();
  }
  catch ( itk::ExceptionObject& err )
  {
    std::cout << "Could not read the moving image." << std::endl;
    std::cout << err << std::endl;
    exit(EXIT_FAILURE);
  }
  TensorImageType::Pointer movingImage = movingImageReader->GetOutput();

  typedef itk::WarpTensorImageFilter
  < TensorImageType, TensorImageType, DeformationFieldType > WarperType;
  WarperType::Pointer warper = WarperType::New();
  warper->SetInput( movingImage );
  warper->SetOutputSpacing( fixedImage->GetSpacing() );
  warper->SetOutputOrigin( fixedImage->GetOrigin() );
  warper->SetOutputDirection( fixedImage->GetDirection() );
  warper->SetDeformationField( inputDefField );
  if ( args.reorientationType == 1 )
    {
      warper->SetReorientationStrategy( PPD );
    }
  else
    {
      warper->SetReorientationStrategy( FS );
    }

  try
  {
    warper->Update();
  }
  catch( itk::ExceptionObject& err )
  {
    std::cout << "Unexpected error warping final image." << std::endl;
    std::cout << err << std::endl;
    exit( EXIT_FAILURE );
  }
  TensorImageType::Pointer warpedMovingImage = warper->GetOutput();

  // THEN WITH LOG-TENSOR INTERP
  typedef itk::LogTensorImageFilter<TensorImageType, TensorImageType>
  LogTensorFilterType;
  LogTensorFilterType::Pointer logTensorFilter = LogTensorFilterType::New();
  logTensorFilter->SetInput(fixedImage);
  try
  {
    logTensorFilter->Update();
  }
  catch ( itk::ExceptionObject &e )
  {
    std::cerr << e;
    exit(-1);
  }
  TensorImageType::Pointer fixedLogImage = logTensorFilter->GetOutput();

  logTensorFilter = LogTensorFilterType::New();
  logTensorFilter->SetInput(movingImage);
  try
  {
    logTensorFilter->Update();
  }
  catch ( itk::ExceptionObject &e )
  {
    std::cerr << e;
    exit(-1);
  }
  TensorImageType::Pointer movingLogImage = logTensorFilter->GetOutput();

  warper = WarperType::New();
  warper->SetInput( movingLogImage );
  warper->SetOutputSpacing( fixedImage->GetSpacing() );
  warper->SetOutputOrigin( fixedImage->GetOrigin() );
  warper->SetOutputDirection( fixedImage->GetDirection() );
  warper->SetDeformationField( inputDefField );
  if ( args.reorientationType == 1 )
    {
      warper->SetReorientationStrategy( PPD );
    }
  else
    {
      warper->SetReorientationStrategy( FS );
    }

  try
  {
    warper->Update();
  }
  catch( itk::ExceptionObject& err )
  {
    std::cout << "Unexpected error warping final image." << std::endl;
    std::cout << err << std::endl;
    exit( EXIT_FAILURE );
  }
  TensorImageType::Pointer warpedMovingLogImage = warper->GetOutput();

  // NOW COMPUTE STATS OVER BOTH IMAGES
  typedef itk::ImageRegionConstIterator<TensorImageType> ImageConstIterator;
  typedef itk::ImageRegionIterator<TensorRealImageType> ImageIterator;

  double tensorImageSSD = 0.0;
  double logTensorImageSSD = 0.0;
  ImageConstIterator iterFixed( fixedImage, fixedImage->GetRequestedRegion() );
  ImageConstIterator iterWarpedMoving( warpedMovingImage, fixedImage->GetRequestedRegion() );
  ImageConstIterator iterFixedLog( fixedLogImage, fixedImage->GetRequestedRegion() );
  ImageConstIterator iterWarpedMovingLog( warpedMovingLogImage, fixedImage->GetRequestedRegion() );

  TensorRealImageType::Pointer mseImage = TensorRealImageType::New();
  mseImage->SetRegions(fixedImage->GetLargestPossibleRegion());
  mseImage->SetSpacing(fixedImage->GetSpacing());
  mseImage->SetOrigin(fixedImage->GetOrigin());
  mseImage->SetDirection(fixedImage->GetDirection());
  mseImage->Allocate();
  mseImage->FillBuffer(0);

  TensorRealImageType::Pointer lmseImage = TensorRealImageType::New();
  lmseImage->SetRegions(fixedImage->GetLargestPossibleRegion());
  lmseImage->SetSpacing(fixedImage->GetSpacing());
  lmseImage->SetOrigin(fixedImage->GetOrigin());
  lmseImage->SetDirection(fixedImage->GetDirection());
  lmseImage->Allocate();
  lmseImage->FillBuffer(0);

  ImageIterator iterMSE = ImageIterator ( mseImage, fixedImage->GetRequestedRegion() );
  ImageIterator iterLMSE ( lmseImage, fixedImage->GetRequestedRegion() );

  for (iterFixed.GoToBegin(), iterFixedLog.GoToBegin(), iterWarpedMoving.GoToBegin(), iterWarpedMovingLog.GoToBegin(), iterMSE.GoToBegin(), iterLMSE.GoToBegin();
  !iterFixed.IsAtEnd();
  ++iterFixed, ++iterFixedLog, ++iterWarpedMoving, ++iterWarpedMovingLog, ++iterMSE, ++iterLMSE)
    {
      TensorPixelType diff = iterFixed.Get() - iterWarpedMoving.Get();
      TensorRealType ssd = diff.GetSquaredNorm();
      iterMSE.Set(ssd);
      tensorImageSSD += ssd;

      TensorPixelType diffLog = iterFixedLog.Get() - iterWarpedMovingLog.Get();
      TensorRealType ssdLog = diffLog.GetSquaredNorm();
      iterLMSE.Set(ssdLog);
      logTensorImageSSD += ssdLog;
    }

  const double tensorImageMSE = tensorImageSSD / static_cast<double>(
      fixedImage->GetRequestedRegion().GetNumberOfPixels() );

  const double logTensorImageMSE = logTensorImageSSD / static_cast<double>(
      fixedLogImage->GetRequestedRegion().GetNumberOfPixels() );

  // WRITE THE ERROR IMAGES
  typedef itk::ImageFileWriter<TensorRealImageType> TensorRealImageWriterType;

  TensorRealImageWriterType::Pointer tensorRealImageWriter =
    TensorRealImageWriterType::New();

  tensorRealImageWriter->SetInput(mseImage);
  tensorRealImageWriter->SetFileName(args.outputMSEFile);
  try
  {
    tensorRealImageWriter->Write();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e;
    exit(-1);
  }

  tensorRealImageWriter = TensorRealImageWriterType::New();
  tensorRealImageWriter->SetInput(lmseImage);
  tensorRealImageWriter->SetFileName(args.outputLMSEFile);
  try
  {
    tensorRealImageWriter->Write();
  }
  catch(itk::ExceptionObject &e)
  {
    std::cerr << e;
    exit(-1);
  }

  // WRITE ALL STATS TO A CSV
  std::ofstream fid( args.outputResultsFile.c_str() );

  fid
  <<"MSE(F,Ms), "
  <<"LMSE(F,Ms), "
  <<"Harmonic energy, "
  <<"min|Jac|, "
  <<"0.2% |Jac|, "
  <<"01% |Jac|, "
  <<"99% |Jac|, "
  <<"99.8% |Jac|, "
  <<"max|Jac|, "
  <<"ratio(|Jac|<=0), ";
  if ( !args.trueDeformationFieldFile.empty() )
    {
      fid
      <<"dist(warp,true warp), "
      <<"dist(Jac,true Jac), ";
    }
  fid << std::endl;

  fid
  << tensorImageMSE << ", "
  << logTensorImageMSE << ", "
  << harmonicEnergy << ", "
  << minJac << ", "
  << Q002 << ", "
  << Q01 << ", "
  << Q99 << ", "
  << Q998 << ", "
  << maxJac << ", "
  << jacBelowZeroPrc;
  if ( !args.trueDeformationFieldFile.empty() )
    {
      fid
      << ", "
      << fieldDist << ", "
      << fieldGradDist;
    }
  fid << std::endl;

  fid.close();

  return EXIT_SUCCESS;
}
