cmake_minimum_required(VERSION 2.4)
IF(COMMAND cmake_policy)
  cmake_policy(VERSION 2.4)
  cmake_policy(SET CMP0005 OLD)
  cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)


project(TTK)


# On Visual Studio 8 MS deprecated C. This removes all 1.276E1265 security
# warnings
IF(WIN32)
  IF(NOT BORLAND)
    IF(NOT CYGWIN)
      IF(NOT MINGW)
        IF(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
          ADD_DEFINITIONS(
            -D_CRT_FAR_MAPPINGS_NO_DEPRECATE
            -D_CRT_IS_WCTYPE_NO_DEPRECATE
            -D_CRT_MANAGED_FP_NO_DEPRECATE
            -D_CRT_NONSTDC_NO_DEPRECATE
            -D_CRT_SECURE_NO_DEPRECATE
            -D_CRT_SECURE_NO_DEPRECATE_GLOBALS
            -D_CRT_SETERRORMODE_BEEP_SLEEP_NO_DEPRECATE
            -D_CRT_TIME_FUNCTIONS_NO_DEPRECATE
            -D_CRT_VCCLRIT_NO_DEPRECATE
            -D_SCL_SECURE_NO_DEPRECATE
            )
        ENDIF(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
      ENDIF(NOT MINGW)
    ENDIF(NOT CYGWIN)
  ENDIF(NOT BORLAND)
ENDIF(WIN32)


set(${PROJECT_NAME}_VERSION_MAJOR 1 CACHE STRING "TTK major version number.")
set(${PROJECT_NAME}_VERSION_MINOR 0 CACHE STRING "TTK minor version number.")
set(${PROJECT_NAME}_VERSION_BUILD 0 CACHE STRING "TTK build version number.")
set(${PROJECT_NAME}_VERSION
  "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD}")

mark_as_advanced(${PROJECT_NAME}_VERSION_MAJOR)
mark_as_advanced(${PROJECT_NAME}_VERSION_MINOR)
mark_as_advanced(${PROJECT_NAME}_VERSION_BUILD)


SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)


## #################################################################
## Configure path
## #################################################################


if(WIN32)
  set(${PROJECT_NAME}_ARCHIVE_OUTPUT_DIRECTORY lib)
  set(${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY bin)
  set(${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY bin)
else(WIN32)
  set(${PROJECT_NAME}_ARCHIVE_OUTPUT_DIRECTORY lib)
  set(${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY bin)
  set(${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY lib)
endif(WIN32)

set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/${${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY})
set(ARCHIVE_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/${${PROJECT_NAME}_ARCHIVE_OUTPUT_DIRECTORY})
set(RUNTIME_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY})

set(LIBRARY_INSTALL_OUTPUT_PATH    ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_LIBRARY_OUTPUT_DIRECTORY})
set(ARCHIVE_INSTALL_OUTPUT_PATH    ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_ARCHIVE_OUTPUT_DIRECTORY})
set(RUNTIME_INSTALL_OUTPUT_PATH    ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY})
set(EXECUTABLE_INSTALL_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/${${PROJECT_NAME}_RUNTIME_OUTPUT_DIRECTORY})


## #################################################################
## Required dependencies
## #################################################################


find_package(ITK)
find_package(VTK)


## #################################################################
## Options
## #################################################################

option(TTK_USE_MKL   OFF "Use MKL to solve eigensystems")
option(TTK_USE_ACML  OFF "Use ACML to solve eigensystems")
option(TTK_USE_MIPS  OFF "Use MIPS and make TTK compatible with inrimage file format (discouraged)")
option(TTK_USE_HARDI OFF "Compile the Hardi extension of the TTK")


if  (TTK_USE_MKL 
 AND TTK_USE_ACML)
  message(FATAL_ERROR "Both TTK_USE_MKL and TTK_USE_ACML are set to ON, only one can be used at the same time")
endif (TTK_USE_MKL 
   AND TTK_USE_ACML)


set(TTK_LAPACK_LIBRARIES
)
if (TTK_USE_MKL)
  find_package(MKL REQUIRED)
  set(TTK_LAPACK_LIBRARIES
  ${MKL_LIBRARIES}
  )
endif(TTK_USE_MKL)

if (TTK_USE_ACML)
  find_package(ACML REQUIRED)
  set(TTK_LAPACK_LIBRARIES
  ${ACML_LIBRARY}
  )
endif(TTK_USE_ACML)


set(TTK_MIPS_LIBRARIES
)
if(TTK_USE_MIPS)
  find_package(MIPS REQUIRED)
  include(${MIPS_USE_FILE})
 
  set(TTK_MIPS_LIBRARIES 
  mipsItkImageIO 
  InrimageNG 
  LinearAlgebra 
  Basics 
  mipsZ)
endif(TTK_USE_MIPS)


configure_file(${PROJECT_SOURCE_DIR}/ttkConfigure.h.in
               ${PROJECT_BINARY_DIR}/ttkConfigure.h CONFIGURE IMMEDIATE)


set(TTK_REQUIRED_C_FLAGS)
set(TTK_REQUIRED_CXX_FLAGS)



if( ITK_FOUND AND VTK_FOUND )

  add_subdirectory(IPF)
  find_package(IPF)
  include(${IPF_USE_FILE})

  include(${ITK_USE_FILE})
  include(${VTK_USE_FILE})

  include(${TTK_SOURCE_DIR}/ttkIncludeDirectories.cmake)
  include_directories(
    ${TTK_INCLUDE_DIRS_BUILD_TREE}
    ${TTK_INCLUDE_DIRS_SYSTEM}
  )

  set(TTK_BUILD_TYPE
  STATIC
  )

  if (NOT WIN32)
    if (ITK_BUILD_SHARED)
      set (TTK_BUILD_TYPE SHARED)
    endif (ITK_BUILD_SHARED)
  endif (NOT WIN32)


  # set TTK_DIR so it can be used by subprojects
  SET(TTK_DIR "${PROJECT_BINARY_DIR}" CACHE INTERNAL "TTK_DIR variable to be used by subprojects")


  add_library(ITKTensor ${TTK_BUILD_TYPE}
  itkNumericTraitsTensorPixel2.cxx
  )
  TARGET_LINK_LIBRARIES(ITKTensor
  ITKCommon
  ${TTK_LAPACK_LIBRARIES}
  )
	
  if(TTK_USE_HARDI)
    find_package(Boost)
    include_directories (${Boost_INCLUDE_DIR})
    if( Boost_FOUND )
      add_subdirectory(Hardi)
    endif( Boost_FOUND )
  endif(TTK_USE_HARDI)

  INSTALL(TARGETS ITKTensor 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )

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
  )


  add_library(ttkCommands ${TTK_BUILD_TYPE}
    ${TTK_COMMAND_SRCS}
  )

  target_link_libraries(ttkCommands
    ${TTK_MIPS_LIBRARIES}
    ITKTensor
    ITKCommon
    ITKIO
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
 
  # A tensor to scalar function calculator
  ADD_EXECUTABLE(itkTensorToScalar itkTensorToScalar.cxx)

  # A Tensors to DWI converter
  ADD_EXECUTABLE(itkTensorsToDWI itkTensorsToDWI.cxx)

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


  TARGET_LINK_LIBRARIES(itkTensorToScalar
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )


  TARGET_LINK_LIBRARIES(itkTensorsToDWI
  ITKCommon
  ITKIO
  ${TTK_MIPS_LIBRARIES}
  vtkIO
  )

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




#-----------------------------------------------------------------------------
# Help other projects use TTK.

CONFIGURE_FILE(${TTK_SOURCE_DIR}/UseTTK.cmake.in
               ${TTK_BINARY_DIR}/UseTTK.cmake COPYONLY IMMEDIATE)

# Save the compiler settings so another project can import them.
SET(TTK_BUILD_SETTINGS_FILE ${TTK_BINARY_DIR}/TTKBuildSettings.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)
CMAKE_EXPORT_BUILD_SETTINGS(${TTK_BUILD_SETTINGS_FILE})

# Create the TTKConfig.cmake file containing the TTK configuration.
INCLUDE (${TTK_SOURCE_DIR}/ttkGenerateTTKConfig.cmake)

INSTALL_FILES(/lib .cmake
 ${TTK_BINARY_DIR}/TTKBuildSettings
 ${TTK_BINARY_DIR}/UseTTK
)



#-----------------------------------------------------------------------------
# Add compiler flags TTK needs to work on this platform.  This must be
# done AFTER the call to CMAKE_EXPORT_BUILD_SETTINGS.
SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${TTK_REQUIRED_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TTK_REQUIRED_CXX_FLAGS}")


else( ITK_FOUND AND VTK_FOUND )

  message(WARNING "ITK or VTK was not set, TTK will not be built.")

endif( ITK_FOUND AND VTK_FOUND )
