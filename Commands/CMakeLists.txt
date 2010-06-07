  set(TTK_COMMAND_SRCS
    itkWarpTensorImageCommand.cxx
    itkWarpTensorImageCommandFactory.cxx
    itkNPTRemoverCommand.cxx
    itkNPTRemoverCommandFactory.cxx
    itkTensorFlipCommand.cxx
    itkTensorFlipCommandFactory.cxx
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
    itkBoostTensorAnisotropyCommand.cxx
    itkBoostTensorAnisotropyCommandFactory.cxx
    itkAverageStdTensorCalculatorCommand.cxx
    itkAverageStdTensorCalculatorCommandFactory.cxx
    itkTensorGradientMagnitudeCalculatorCommand.cxx
    itkTensorGradientMagnitudeCalculatorCommandFactory.cxx
    itkNormalize2TensorsCommand.cxx
    itkNormalize2TensorsCommandFactory.cxx
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
    itkConsolidateFiberBundleCommand.cxx
    itkConsolidateFiberBundleCommandFactory.cxx
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
 
 



  # ttk utils

  set(TTK-UTILS_COMMAND_SRCS
    itkDWIBrainMaskExtractorCommand.cxx
    itkDWIBrainMaskExtractorCommandFactory.cxx
    itkApplyMaskToImageCommand.cxx
    itkApplyMaskToImageCommandFactory.cxx
    itkResampleImageCommand.cxx
    itkResampleImageCommandFactory.cxx
    itkWarpImageCommand.cxx
    itkWarpImageCommandFactory.cxx
    itkAverageStdCalculatorCommand.cxx
    itkAverageStdCalculatorCommandFactory.cxx
    itkRBFRotationMatrixExtrapolationCommand.cxx
    itkRBFRotationMatrixExtrapolationCommandFactory.cxx
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



  # A vector field converter
  add_executable(itkStanleyToITK itkStanleyVectorFieldToITKVectorField.cxx)

  TARGET_LINK_LIBRARIES(itkStanleyToITK
  ITKCommon
  ITKIO
  )


if(NOT ${PROJECT_NAME}_INSTALL_NO_LIBRARIES)
  install(TARGETS ttkCommands
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )
endif(NOT ${PROJECT_NAME}_INSTALL_NO_LIBRARIES)


if(NOT ${PROJECT_NAME}_INSTALL_NO_RUNTIME)
  install(TARGETS ttk
    BUNDLE DESTINATION bin
    RUNTIME DESTINATION bin
    )
endif(NOT ${PROJECT_NAME}_INSTALL_NO_RUNTIME)


if(NOT ${PROJECT_NAME}_INSTALL_NO_LIBRARIES)
  install(TARGETS ttkConvertCommands
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )
endif(NOT ${PROJECT_NAME}_INSTALL_NO_LIBRARIES)


if(NOT ${PROJECT_NAME}_INSTALL_NO_RUNTIME)
  install(TARGETS ttk-convert
    BUNDLE DESTINATION bin
    RUNTIME DESTINATION bin
    )
endif(NOT ${PROJECT_NAME}_INSTALL_NO_RUNTIME)


if(NOT ${PROJECT_NAME}_INSTALL_NO_LIBRARIES)
  install(TARGETS ttkUtilCommands
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )
endif(NOT ${PROJECT_NAME}_INSTALL_NO_LIBRARIES)


if(NOT ${PROJECT_NAME}_INSTALL_NO_RUNTIME)
  install(TARGETS ttk-utils
    BUNDLE DESTINATION bin
    RUNTIME DESTINATION bin
    )
endif(NOT ${PROJECT_NAME}_INSTALL_NO_RUNTIME)

 

if(NOT ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  file(GLOB __files3 "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
  install(FILES ${__files1} ${__files2} ${__files3}
    DESTINATION include
    COMPONENT   Development
  )
endif(NOT ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
