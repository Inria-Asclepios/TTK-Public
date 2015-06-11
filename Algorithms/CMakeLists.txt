if(NOT ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  file(GLOB __files3 "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
  install(FILES ${__files1} ${__files2} ${__files3}
    DESTINATION include
    COMPONENT   Development
  )
endif(NOT ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)

add_library(ttkAlgorithms ${TTK_BUILD_TYPE}
itkFiberBundleStatisticsCalculator.cxx
)

target_link_libraries(ttkAlgorithms
  ITKCommon
  vtkCommonCore
  vtkIOCore
)

if(NOT ${PROJECT_NAME}_INSTALL_NO_LIBRARIES)
  install(TARGETS ttkAlgorithms 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )
endif(NOT ${PROJECT_NAME}_INSTALL_NO_LIBRARIES)

