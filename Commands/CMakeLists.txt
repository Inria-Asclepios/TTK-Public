  set(TTK_COMMAND_SRCS
    itkWarpTensorImageCommand.cxx
    itkWarpTensorImageCommandFactory.cxx
    itkNPTRemoverCommand.cxx
    itkNPTRemoverCommandFactory.cxx
    itkTensorFlipCommand.cxx
    itkTensorFlipCommandFactory.cxx
    itkDTIEstimatorCommand.cxx
    itkDTIEstimatorCommandFactory.cxx
    itkAddGaussianNoiseToTensorsCommand.cxx
    itkAddGaussianNoiseToTensorsCommandFactory.cxx
    itkTensorAnisotropicFilteringCommand.cxx
    itkTensorAnisotropicFilteringCommandFactory.cxx
    itkNormalizeTensorsCommand.cxx
    itkNormalizeTensorsCommandFactory.cxx
    itkTensorGaussianFilteringCommand.cxx
    itkTensorGaussianFilteringCommandFactory.cxx
    itkSparseTensorsExtrapolationCommand.cxx
    itkSparseTensorsExtrapolationCommandFactory.cxx
    itkRBFTensorExtrapolationCommand.cxx
    itkRBFTensorExtrapolationCommandFactory.cxx
    itkTensorNormalizedGaussianInterpolationCommand.cxx
    itkTensorNormalizedGaussianInterpolationCommandFactory.cxx
    itkFiberTrackingCommand.cxx
    itkFiberTrackingCommandFactory.cxx
    itkWarpFibersCommand.cxx
    itkWarpFibersCommandFactory.cxx
    itkHessianTensorImageCalculatorCommand.cxx
    itkHessianTensorImageCalculatorCommandFactory.cxx
    itkResampleTensorImageCommand.cxx
    itkResampleTensorImageCommandFactory.cxx
    itkResampleTensorImage2Command.cxx
    itkResampleTensorImage2CommandFactory.cxx
    itkTensorToScalarCommand.cxx
    itkTensorToScalarCommandFactory.cxx
    itkTensorsToDWICommand.cxx
    itkTensorsToDWICommandFactory.cxx
    itkSyntheticTensorFieldGeneratorCommand.cxx
    itkSyntheticTensorFieldGeneratorCommandFactory.cxx
    itkAddGaussianNoiseToDWICommand.cxx
    itkAddGaussianNoiseToDWICommandFactory.cxx
    itkBoostTensorAnisotropyCommand.cxx
    itkBoostTensorAnisotropyCommandFactory.cxx
    itkDTIEstimatorWithBFGSCommand.cxx
    itkDTIEstimatorWithBFGSCommandFactory.cxx
    itkLogEuclideanDTIEstimatorCommand.cxx
    itkLogEuclideanDTIEstimatorCommandFactory.cxx
    itkTensorZScoreCalculatorCommand.cxx
    itkTensorZScoreCalculatorCommandFactory.cxx
    itkAverageStdTensorCalculatorCommand.cxx
    itkAverageStdTensorCalculatorCommandFactory.cxx
  )


  add_library(ttkCommands ${TTK_BUILD_TYPE}
    ${TTK_COMMAND_SRCS}
  )

  target_link_libraries(ttkCommands
    ${TTK_MIPS_LIBRARIES}
    ITKProgramFactory
    ITKTensor
    ${ITK_LIBRARIES}
    vtkIO
    ${TTK_LAPACK_LIBRARIES}
  )

  add_executable(ttk
    ttk.cxx
  )

  target_link_libraries(ttk
    ttkCommands
    ITKProgramFactory
    ${ITK_LIBRARIES}
  )



  # ttk convert

  set(TTK-CONVERT_COMMAND_SRCS
    itkTensorConverterCommand.cxx
    itkTensorConverterCommandFactory.cxx
    itkVTKFibersToITKGroupSpatialObjectCommand.cxx
    itkVTKFibersToITKGroupSpatialObjectCommandFactory.cxx
    itkTensorsToVTKUnstructuredGridCommand.cxx
    itkTensorsToVTKUnstructuredGridCommandFactory.cxx
    itkImageConverterCommand.cxx
    itkImageConverterCommandFactory.cxx
  )

  add_library(ttkConvertCommands ${TTK_BUILD_TYPE}
    ${TTK-CONVERT_COMMAND_SRCS}
  )

  target_link_libraries(ttkConvertCommands
    ${TTK_MIPS_LIBRARIES}
    ITKProgramFactory
    ITKTensor
    ${ITK_LIBRARIES}
    vtkIO
    ${TTK_LAPACK_LIBRARIES}
  )

  add_executable(ttk-convert
    ttk-convert.cxx
  )

  target_link_libraries(ttk-convert
    ttkConvertCommands
    ITKProgramFactory
    ${ITK_LIBRARIES}
  )
 
  # A vector field converter
  add_executable(itkStanleyToITK itkStanleyVectorFieldToITKVectorField.cxx)




  # ttk utils

  set(TTK-UTILS_COMMAND_SRCS
    itkDWIBrainMaskExtractorCommand.cxx
    itkDWIBrainMaskExtractorCommandFactory.cxx
  )

  add_library(ttkUtilCommands ${TTK_BUILD_TYPE}
    ${TTK-UTILS_COMMAND_SRCS}
  )

  target_link_libraries(ttkUtilCommands
    ${TTK_MIPS_LIBRARIES}
    ITKProgramFactory
    ITKTensor
    ${ITK_LIBRARIES}
    vtkIO
    ${TTK_LAPACK_LIBRARIES}
  )

  add_executable(ttk-utils
    ttk-utils.cxx
  )

  target_link_libraries(ttk-utils
    ttkUtilCommands
    ITKProgramFactory
    ${ITK_LIBRARIES}
  )



  add_executable(vtkConsolidateFiberBundle vtkConsolidateFiberBundle.cxx)

  # A RBF Rotation Matrix Interpolator
  ADD_EXECUTABLE(itkRBFRotationMatrixInterpolation itkRBFRotationMatrixInterpolation.cxx)


  # A simple program to average several images
  add_executable(itkAverageStdImageFilter itkAverageStdImageFilter.cxx)
  
  # A simple program to warp an image
  add_executable(itkWarpImage itkWarpImage.cxx)

  # A simple program to resample an image
  add_executable(itkResampleImage itkResampleImage.cxx)

  # A program to apply a mask to an image
  add_executable(itkApplyMaskToImage itkApplyMaskToImages.cxx)

  # A 4D DWI reader
  add_executable(itkRead4DDWI itkRead4DDWI.cxx)




  TARGET_LINK_LIBRARIES(itkAverageStdImageFilter
  ITKCommon
  ITKIO
  )

  TARGET_LINK_LIBRARIES(itkWarpImage
  ITKCommon
  ITKIO
  ${TTK_MIPS_LIBRARIES}
  )
  
  TARGET_LINK_LIBRARIES(itkResampleImage
  ITKCommon
  ITKIO
  ${TTK_MIPS_LIBRARIES}
  )
  
#  TARGET_LINK_LIBRARIES(itkB0BrainMaskExtractor
#  ITKCommon
#  ITKIO
#  )

  TARGET_LINK_LIBRARIES(itkApplyMaskToImage
  ITKTensor
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  )
  
  TARGET_LINK_LIBRARIES(itkRead4DDWI
  ITKCommon
  ITKIO
  )
  
#  TARGET_LINK_LIBRARIES(imconvert
#  ITKCommon
#  ITKIO
#  ${TTK_MIPS_LIBRARIES}
#  )

  TARGET_LINK_LIBRARIES(itkStanleyToITK
  ITKCommon
  ITKIO
  )
  
  TARGET_LINK_LIBRARIES(vtkConsolidateFiberBundle
  vtkIO
  )
	
  TARGET_LINK_LIBRARIES(itkRBFRotationMatrixInterpolation
  ITKCommon
  ITKIO
  vtkIO
  vtkCommon
  ${TTK_LAPACK_LIBRARIES}
  )


#  TARGET_LINK_LIBRARIES(VTK2Fib
#  ITKTensor
#  vtkIO
#  vtkCommon
#  ${TTK_MIPS_LIBRARIES}
#  ${ITK_LIBRARIES}
#  ${TTK_LAPACK_LIBRARIES}
#  )


#  TARGET_LINK_LIBRARIES(itkNrrdToTensor
#  ITKCommon
#  ITKIO
#  vtkIO
#  ${TTK_MIPS_LIBRARIES}
#  )

#  TARGET_LINK_LIBRARIES(itkTensorsToVTKUnstructuredGrid
#    ITKNumerics
#    ITKCommon
#    ITKIO
#    ITKTensor
#    vtkIO
#    vtkFiltering
#    vtkCommon
#    ${TTK_MIPS_LIBRARIES}
#    ${TTK_LAPACK_LIBRARIES}
#  )


if (TTK_USE_MIPS)

 # An Inria to VTK tensor file format converter 
  ADD_EXECUTABLE(inr2vtk mipsInrimageToVTK.cxx)
  TARGET_LINK_LIBRARIES(inr2vtk
    ITKCommon
    vtkIO
    ${TTK_MIPS_LIBRARIES}
  )

  # A VTK tensor file format to Inria converter 
  ADD_EXECUTABLE(vtk2inr mipsVTKToInrimage.cxx)
  TARGET_LINK_LIBRARIES(vtk2inr
    ITKCommon
    vtkIO
    ${TTK_MIPS_LIBRARIES}
  )

endif (TTK_USE_MIPS)


# A Tensors to DWI converter
#   ADD_EXECUTABLE(itkMultiResolutionTensorFilter
#   itkMultiResolutionTensorFilter.cxx
#   )
#   TARGET_LINK_LIBRARIES(itkMultiResolutionTensorFilter
#   ITKCommon
#   InrimageNG
#   ITKIO
#   )


#  IF(VTK_FOUND)
		
# A tensor field gradient magnitude calculator
#   ADD_EXECUTABLE(itkGradientMagnitudeTensorImageFilter
#   itkGradientMagnitudeTensorImageFilter.cxx
#   )
#   TARGET_LINK_LIBRARIES(itkGradientMagnitudeTensorImageFilter
#   ${TTK_MIPS_LIBRARIES}
#   ITKCommon
#   ITKIO
#   )

# A Tensor derivative Gaussian filter
#   ADD_EXECUTABLE(itkMultiscaleGaussianDerivativeTensorImageFilter
#   itkMultiscaleGaussianDerivativeTensorImageFilter.cxx
#   )
#   TARGET_LINK_LIBRARIES(itkMultiscaleGaussianDerivativeTensorImageFilter
#   ITKCommon
#   ${TTK_MIPS_LIBRARIES}
#   ITKIO
#   )


file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  file(GLOB __files3 "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
  install(FILES ${__files1} ${__files2} ${__files3}
    DESTINATION include
    COMPONENT   Development
  )
