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
    itkScaleTensorCommand.cxx
    itkScaleTensorCommandFactory.cxx
  )

  if (TTK_USE_GMM)
    set(TTK_COMMAND_SRCS
      ${TTK_COMMAND_SRCS}
      itkDemonsTensorRegistrationCommand.cxx
      itkDemonsTensorRegistrationCommandFactory.cxx
      itkLogDomainDemonsTensorRegistrationCommand.cxx
      itkLogDomainDemonsTensorRegistrationCommandFactory.cxx
    )
  endif (TTK_USE_GMM)


  add_library(ttkCommands ${TTK_BUILD_TYPE}
    ${TTK_COMMAND_SRCS}
  )

  target_link_libraries(ttkCommands
    ${TTK_MIPS_LIBRARIES}
    ${TTK_VTKINRIA3D_LIBRARIES}
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


  if(WIN32)
     # The file itkImageConverterCommand.cxx causes the following compile-time error using visual studio 2010:
     #     ...\itk\code\common\itkdefaultpixelaccessorfunctor.h(66): 
     #     fatal error C1128: number of sections exceeded object file format limit : compile with /bigobj
     # The additional flag /bigobj is added to this one file here. 
     get_source_file_property(itkImageConverterCommand_COMPILE_FLAGS itkImageConverterCommand.cxx COMPILE_FLAGS)
     if (itkImageConverterCommand_COMPILE_FLAGS STREQUAL NOTFOUND)
       set_source_files_properties(itkImageConverterCommand.cxx PROPERTIES COMPILE_FLAGS "/bigobj")
     else (itkImageConverterCommand_COMPILE_FLAGS STREQUAL NOTFOUND)
       set_source_files_properties(itkImageConverterCommand.cxx PROPERTIES COMPILE_FLAGS "${itkImageConverterCommand_COMPILE_FLAGS} /bigobj")
     endif (itkImageConverterCommand_COMPILE_FLAGS STREQUAL NOTFOUND)
  endif(WIN32)
  
  target_link_libraries(ttkConvertCommands
    ${TTK_MIPS_LIBRARIES}
    ${TTK_VTKINRIA3D_LIBRARIES}
    ITKProgramFactory
    ITKTensor
    ${ITK_LIBRARIES}
    vtkIO
    ${TTK_LAPACK_LIBRARIES}
  )
  if (ITK_USE_REVIEW)
    target_link_libraries(ttkConvertCommands
      ITKIOReview
      )
  endif()
  
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
    itkAddGaussianNoiseToImageCommand.cxx
    itkAddGaussianNoiseToImageCommandFactory.cxx
    itkResampleImageCommand.cxx
    itkResampleImageCommandFactory.cxx
    itkWarpImageCommand.cxx
    itkWarpImageCommandFactory.cxx
    itkAverageStdCalculatorCommand.cxx
    itkAverageStdCalculatorCommandFactory.cxx
    itkRBFRotationMatrixExtrapolationCommand.cxx
    itkRBFRotationMatrixExtrapolationCommandFactory.cxx
    itkPermute23TensorCoefficientsCommand.cxx
    itkPermute23TensorCoefficientsCommandFactory.cxx
    itkSubtractImageCommand.cxx
    itkSubtractImageCommandFactory.cxx
    itkAverage4DImageCommand.cxx
    itkAverage4DImageCommandFactory.cxx
  )

  add_library(ttkUtilCommands ${TTK_BUILD_TYPE}
    ${TTK-UTILS_COMMAND_SRCS}
  )

  target_link_libraries(ttkUtilCommands
    ${TTK_MIPS_LIBRARIES}
    ${TTK_VTKINRIA3D_LIBRARIES}
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

  
  # registration commands
  if (TTK_USE_GMM)

    add_executable(AssessRegistrationTensor
      AssessRegistrationTensor.cxx
      )

    add_executable(DemonsRegistrationTensor
      DemonsRegistrationTensor.cxx
      )

    add_executable(LogDomainDemonsRegistrationTensor
      LogDomainDemonsRegistrationTensor.cxx
      )

    target_link_libraries(AssessRegistrationTensor
      ${TTK_MIPS_LIBRARIES}
      ${TTK_VTKINRIA3D_LIBRARIES}
      ${TTK_LAPACK_LIBRARIES}
      ITKCommon
      ITKIO
      ITKTensor
      ITKBasicFilters
      vtkCommon
      vtkIO
      )
    target_link_libraries(DemonsRegistrationTensor
      ${TTK_MIPS_LIBRARIES}
      ${TTK_VTKINRIA3D_LIBRARIES}
      ${TTK_LAPACK_LIBRARIES}
      ITKCommon
      ITKIO
      ITKTensor
      ITKBasicFilters
      vtkCommon
      vtkIO
      )
    target_link_libraries(LogDomainDemonsRegistrationTensor
      ${TTK_MIPS_LIBRARIES}
      ${TTK_VTKINRIA3D_LIBRARIES}
      ${TTK_LAPACK_LIBRARIES}
      ITKCommon
      ITKIO
      ITKTensor
      ITKBasicFilters
      vtkCommon
      vtkIO
      )

  endif (TTK_USE_GMM)


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
