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
 
 # A library to read a vtk tensor file
  # ADD_LIBRARY(VTKTensorImageIO ${TTK_TENSOR_BUILD_TYPE}
  # itkVTKStructuredPointsTensorImageIO.cxx
  # )

  ADD_EXECUTABLE(itkGenerateSyntheticTensorField generateSyntheticTensorField.cxx)

  ADD_EXECUTABLE(itkAddGaussianNoiseToTensorDWIs  itkAddGaussianNoiseToTensorDWIs.cxx)      

  ADD_EXECUTABLE(itkBoostTensorAnisotropy itkBoostTensorAnisotropy.cxx)      

  # A Log-Euclidean BFGS dti estimator
  ADD_EXECUTABLE(itkDTIEstimatorWithBFGS itkDTIEstimatorWithBFGS.cxx)

  # A Log-Euclidean dti estimator
  ADD_EXECUTABLE(itkLogEuclideanDTIEstimator itkLogEuclideanDTIEstimator.cxx)

  # A tensor z-score calculator 
  ADD_EXECUTABLE(itkZScoreTensorImageFilter itkZScoreTensorImageFilter.cxx)

  # A tensor statistics calculator 
  ADD_EXECUTABLE(itkAverageStdTensorImageFilter itkAverageStdTensorImageFilter.cxx)




  # ttk convert

  ADD_EXECUTABLE(itkTensorsToVTKUnstructuredGrid itkTensorsToVTKUnstructuredGrid.cxx)

  # A Nrrd file format converter
  ADD_EXECUTABLE(itkNrrdToTensor itkNeerdToTensor.cxx)

  ADD_EXECUTABLE(VTK2Fib VTK2Fib.cxx)

  # A vector field converter
  add_executable(itkStanleyToITK itkStanleyVectorFieldToITKVectorField.cxx)




  # ttk utils

  # A B0 brain mask extractor
  add_executable(itkB0BrainMaskExtractor itkB0BrainMaskExtraction.cxx)

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

  # An image file converter
  add_executable(imconvert gipl2ana.cxx)



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
  
  TARGET_LINK_LIBRARIES(itkB0BrainMaskExtractor
  ITKCommon
  ITKIO
  )

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
  
  TARGET_LINK_LIBRARIES(imconvert
  ITKCommon
  ITKIO
  ${TTK_MIPS_LIBRARIES}
  )

  TARGET_LINK_LIBRARIES(itkStanleyToITK
  ITKCommon
  ITKIO
  )
  
  TARGET_LINK_LIBRARIES(vtkConsolidateFiberBundle
  vtkIO
  )

#  TARGET_LINK_LIBRARIES(itkTensorAnisotropicFilter
#  ITKCommon
#  ITKIO
#  vtkIO
#  ${TTK_MIPS_LIBRARIES}
#  ${TTK_LAPACK_LIBRARIES}
#  )

#  TARGET_LINK_LIBRARIES(itkDTIEstimator
#  ITKCommon
#  ITKIO
#  vtkIO
#  ${TTK_MIPS_LIBRARIES}
#  ${TTK_LAPACK_LIBRARIES}
#  )

#  TARGET_LINK_LIBRARIES(itkRemoveNPT
#  ITKCommon
#  ITKIO
#  vtkIO
#  ${TTK_MIPS_LIBRARIES}
#  ${TTK_LAPACK_LIBRARIES}
#  )

#  TARGET_LINK_LIBRARIES(itkFlipTensors
#  ITKCommon
#  ITKIO
#  vtkIO
#  ${TTK_MIPS_LIBRARIES}
#  )

#  TARGET_LINK_LIBRARIES(itkAddNoiseToTensors
#  ITKCommon
#  ITKStatistics
#  ITKIO
#  vtkIO
#  ${TTK_MIPS_LIBRARIES}
#  ${TTK_LAPACK_LIBRARIES}
#  )

#  TARGET_LINK_LIBRARIES(itkNormalizeTensors
#  ITKCommon
#  ITKStatistics
#  ITKIO
#  vtkIO
#  ${TTK_MIPS_LIBRARIES}
#  ${TTK_LAPACK_LIBRARIES}
#  )

  TARGET_LINK_LIBRARIES(itkDTIEstimatorWithBFGS
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )
  

  TARGET_LINK_LIBRARIES(itkLogEuclideanDTIEstimator
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  # A Tensors to DWI converter
  # ADD_EXECUTABLE(itkWarpTensorImage itkWarpTensorImage.cxx)
  # TARGET_LINK_LIBRARIES(itkWarpTensorImage
  # ${TTK_MIPS_LIBRARIES}
  # ITKTensor
  # ITKCommon
  # ITKIO
  # vtkIO
  # ${TTK_LAPACK_LIBRARIES}
 #  )

#  TARGET_LINK_LIBRARIES(itkGaussianTensorImageFilter
#  ITKTensor
#  ITKCommon
#  ITKIO
#  vtkIO
#  ${TTK_MIPS_LIBRARIES}
#  ${TTK_LAPACK_LIBRARIES}
#  )

  TARGET_LINK_LIBRARIES(itkZScoreTensorImageFilter
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )
		
  TARGET_LINK_LIBRARIES(itkAverageStdTensorImageFilter
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

#  TARGET_LINK_LIBRARIES(VTKTensorImageIO
#  ITKCommon  
#  vtkIO
#  vtkCommon
#  )

#  TARGET_LINK_LIBRARIES(itkSparseTensorsDiffusion
#  ITKCommon
#  ITKIO
#  vtkIO
#  ${TTK_MIPS_LIBRARIES}
#  ${TTK_LAPACK_LIBRARIES}
#  )

		
#  TARGET_LINK_LIBRARIES(itkRBFTensorInterpolation
#  ${TTK_MIPS_LIBRARIES}
#  ITKCommon
#  ITKIO
#  vtkIO
#  vtkCommon
#  ${TTK_LAPACK_LIBRARIES}
#  )

		
  TARGET_LINK_LIBRARIES(itkRBFRotationMatrixInterpolation
  ITKCommon
  ITKIO
  vtkIO
  vtkCommon
  ${TTK_LAPACK_LIBRARIES}
  )

#  TARGET_LINK_LIBRARIES(itkNormalizedGaussianTensorInterpolation
#  ${TTK_MIPS_LIBRARIES}
#  ITKCommon
#  ITKIO
#  vtkIO
#  vtkCommon
#  ${TTK_LAPACK_LIBRARIES}
#  )


#  TARGET_LINK_LIBRARIES(itkFiberTracking
#  ITKTensor
#  vtkIO
#  vtkCommon
#  ${TTK_MIPS_LIBRARIES}
#  ${ITK_LIBRARIES}
#  ${TTK_LAPACK_LIBRARIES}
#  )


  TARGET_LINK_LIBRARIES(VTK2Fib
  ITKTensor
  vtkIO
  vtkCommon
  ${TTK_MIPS_LIBRARIES}
  ${ITK_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )


#  TARGET_LINK_LIBRARIES(itkWarpFiberBundle
#  vtkIO
#  vtkCommon
#  vtkRendering
#  ${ITK_LIBRARIES}
#  )


#  TARGET_LINK_LIBRARIES(itkHessianTensorImageFilter
#  ITKCommon
#  ITKIO
#  vtkIO
#  ${TTK_MIPS_LIBRARIES}
#  ${TTK_LAPACK_LIBRARIES}
#  )

  TARGET_LINK_LIBRARIES(itkNrrdToTensor
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  )


#  TARGET_LINK_LIBRARIES(itkResampleTensorImage
#  ITKTensor
#  ITKCommon
#  ITKIO
#  vtkIO
#  ${TTK_MIPS_LIBRARIES}
#  ${TTK_LAPACK_LIBRARIES}
#  )


#  TARGET_LINK_LIBRARIES(itkResampleTensorImage2
#  ITKTensor
#  ITKCommon
#  ITKIO
#  vtkIO
#  ${TTK_MIPS_LIBRARIES}
#  ${TTK_LAPACK_LIBRARIES}
#  )


#  TARGET_LINK_LIBRARIES(itkTensorToScalar
#  ITKCommon
#  ITKIO
#  vtkIO
#  ${TTK_MIPS_LIBRARIES}
#  ${TTK_LAPACK_LIBRARIES}
#  )


#  TARGET_LINK_LIBRARIES(itkTensorsToDWI
#  ITKCommon
#  ITKIO
#  ${TTK_MIPS_LIBRARIES}
#  vtkIO
#  )

  TARGET_LINK_LIBRARIES(itkGenerateSyntheticTensorField
  vtkIO
  ITKTensor
  ${ITK_LIBRARIES}  
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )



  TARGET_LINK_LIBRARIES(itkAddGaussianNoiseToTensorDWIs
    ITKCommon
    ITKStatistics
    ITKIO
    ITKTensor
    vtkIO
    ${TTK_MIPS_LIBRARIES}
    ${TTK_LAPACK_LIBRARIES}
  )
  
  TARGET_LINK_LIBRARIES(itkBoostTensorAnisotropy
    ITKCommon
    ITKIO
    ITKTensor
    vtkIO
    ${TTK_MIPS_LIBRARIES}
    ${TTK_LAPACK_LIBRARIES}
  )
  

  TARGET_LINK_LIBRARIES(itkTensorsToVTKUnstructuredGrid
    ITKNumerics
    ITKCommon
    ITKIO
    ITKTensor
    vtkIO
    vtkFiltering
    vtkCommon
    ${TTK_MIPS_LIBRARIES}
    ${TTK_LAPACK_LIBRARIES}
  )


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
