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


SET(EXECUTABLE_OUTPUT_PATH 
  ${CMAKE_BINARY_DIR}/bin 
  CACHE PATH "Single output directory for building all executables."
  )
SET(LIBRARY_OUTPUT_PATH 
  ${CMAKE_BINARY_DIR}/lib 
  CACHE PATH "Single output directory for building all libraries."
  )


SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)


find_package(ITK)
find_package(VTK)
find_package(Boost)



### Use MKL or ACML ###

option(TTK_USE_MKL  OFF "Use MKL to solve eigensystems")
option(TTK_USE_ACML OFF "Use ACML to solve eigensystems")

if (TTK_USE_MKL AND TTK_USE_ACML)
  message(FATAL_ERROR "Both TTK_USE_MKL and TTK_USE_ACML are set to ON, only one can be used at the same time")
endif (TTK_USE_MKL AND TTK_USE_ACML)

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


### Use Mips? ###

set(TTK_MIPS_LIBRARIES
)

option(TTK_USE_MIPS OFF "Use MIPS and make TTK compatible with inrimage file format (discouraged)")

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

  include(${ITK_USE_FILE})
  include(${VTK_USE_FILE})

  include(${TTK_SOURCE_DIR}/ttkIncludeDirectories.cmake)
  include_directories(
    ${TTK_INCLUDE_DIRS_BUILD_TREE}
    ${TTK_INCLUDE_DIRS_SYSTEM}
  )


  set(TTK_TENSOR_BUILD_TYPE
  )

  if (NOT WIN32)
    if (ITK_BUILD_SHARED)
      set (TTK_TENSOR_BUILD_TYPE SHARED)
    endif (ITK_BUILD_SHARED)
  endif (NOT WIN32)


  # set TTK_DIR so it can be used by subprojects
  SET(TTK_DIR "${PROJECT_BINARY_DIR}" CACHE INTERNAL "TTK_DIR variable to be used by subprojects")



  add_library(ITKTensor ${TTK_TENSOR_BUILD_TYPE}
  itkNumericTraitsTensorPixel2.cxx
  )
  TARGET_LINK_LIBRARIES(ITKTensor
  ITKCommon
  ${TTK_LAPACK_LIBRARIES}
  )
	

  if( Boost_FOUND )
    add_subdirectory(Hardi)
  endif( Boost_FOUND )


  INSTALL(TARGETS ITKTensor 
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
  COMPONENT RuntimeLibraries
  )


  # A simple program to average several images
  add_executable(itkAverageStdImageFilter itkAverageStdImageFilter.cxx)
  
  # A simple program to warp an image
  add_executable(itkWarpImage itkWarpImage.cxx)

  # A simple program to resample an image
  add_executable(itkResampleImage itkResampleImage.cxx)

  # A B0 brain mask extractor
  add_executable(itkB0BrainMaskExtractor itkB0BrainMaskExtraction.cxx)

  # A program to apply a mask to an image
  add_executable(itkApplyMaskToImage itkApplyMaskToImages.cxx)

  # A 4D DWI reader
  add_executable(itkRead4DDWI itkRead4DDWI.cxx)

  # An image file converter
  add_executable(imconvert gipl2ana.cxx)

  # A vector field converter
  add_executable(itkStanleyToITK itkStanleyVectorFieldToITKVectorField.cxx)

  add_executable(vtkConsolidateFiberBundle vtkConsolidateFiberBundle.cxx)

  # An anisotropic filter for tensor field
  add_executable(itkTensorAnisotropicFilter itkTensorAnisotropicFilter.cxx)

  # An ITK dti estimator
  add_executable(itkDTIEstimator itkDTIEstimator.cxx)

  # This program removes NPT (Non-Positive Tensors)
  add_executable(itkRemoveNPT itkRemoveNPT.cxx)

  # This program Flips tensors around an axis
  add_executable(itkFlipTensors itkFlipTensors.cxx)

  add_executable(itkAddNoiseToTensors itkAddNoiseToTensors.cxx)

  ADD_EXECUTABLE(itkNormalizeTensors itkNormalizeTensorImage.cxx)

  # A Log-Euclidean BFGS dti estimator
  ADD_EXECUTABLE(itkDTIEstimatorWithBFGS itkDTIEstimatorWithBFGS.cxx)


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

  TARGET_LINK_LIBRARIES(itkTensorAnisotropicFilter
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  TARGET_LINK_LIBRARIES(itkDTIEstimator
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  TARGET_LINK_LIBRARIES(itkRemoveNPT
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  TARGET_LINK_LIBRARIES(itkFlipTensors
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  )

  TARGET_LINK_LIBRARIES(itkAddNoiseToTensors
  ITKCommon
  ITKStatistics
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  TARGET_LINK_LIBRARIES(itkNormalizeTensors
  ITKCommon
  ITKStatistics
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  TARGET_LINK_LIBRARIES(itkDTIEstimatorWithBFGS
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )
  
  # A Log-Euclidean dti estimator
  ADD_EXECUTABLE(itkLogEuclideanDTIEstimator itkLogEuclideanDTIEstimator.cxx)

  TARGET_LINK_LIBRARIES(itkLogEuclideanDTIEstimator
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  # A Tensors to DWI converter
  ADD_EXECUTABLE(itkWarpTensorImage itkWarpTensorImage.cxx)
  TARGET_LINK_LIBRARIES(itkWarpTensorImage
  ${TTK_MIPS_LIBRARIES}
  ITKTensor
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_LAPACK_LIBRARIES}
  )

  # A Tensor Gaussian smoother
  ADD_EXECUTABLE(itkGaussianTensorImageFilter itkGaussianTensorImageFilter.cxx)
  TARGET_LINK_LIBRARIES(itkGaussianTensorImageFilter
  ITKTensor
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  # A tensor z-score calculator 
  ADD_EXECUTABLE(itkZScoreTensorImageFilter itkZScoreTensorImageFilter.cxx)
  TARGET_LINK_LIBRARIES(itkZScoreTensorImageFilter
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )
		
  # A tensor statistics calculator 
  ADD_EXECUTABLE(itkAverageStdTensorImageFilter itkAverageStdTensorImageFilter.cxx)
  TARGET_LINK_LIBRARIES(itkAverageStdTensorImageFilter
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  # A library to read a vtk tensor file
  ADD_LIBRARY(VTKTensorImageIO ${TTK_TENSOR_BUILD_TYPE}
   itkVTKStructuredPointsTensorImageIO.cxx
   )
  TARGET_LINK_LIBRARIES(VTKTensorImageIO
  ITKCommon  
  vtkIO
  vtkCommon
  )

  # An itk Sparse tensor diffusion appz
  ADD_EXECUTABLE(itkSparseTensorsDiffusion itkSparseTensorsDiffusion.cxx)
  TARGET_LINK_LIBRARIES(itkSparseTensorsDiffusion
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  # A RBF Tensor Interpolator
  ADD_EXECUTABLE(itkRBFTensorInterpolation itkRBFTensorInterpolation.cxx)
		
  TARGET_LINK_LIBRARIES(itkRBFTensorInterpolation
  ${TTK_MIPS_LIBRARIES}
  ITKCommon
  ITKIO
  vtkIO
  vtkCommon
  ${TTK_LAPACK_LIBRARIES}
  )

  # A RBF Rotation Matrix Interpolator
  ADD_EXECUTABLE(itkRBFRotationMatrixInterpolation itkRBFRotationMatrixInterpolation.cxx)
		
  TARGET_LINK_LIBRARIES(itkRBFRotationMatrixInterpolation
  ITKCommon
  ITKIO
  vtkIO
  vtkCommon
  ${TTK_LAPACK_LIBRARIES}
  )

  ADD_EXECUTABLE(itkNormalizedGaussianTensorInterpolation itkNormalizedGaussianTensorInterpolation.cxx)
  TARGET_LINK_LIBRARIES(itkNormalizedGaussianTensorInterpolation
  ${TTK_MIPS_LIBRARIES}
  ITKCommon
  ITKIO
  vtkIO
  vtkCommon
  ${TTK_LAPACK_LIBRARIES}
  )

  ADD_EXECUTABLE(itkFiberTracking mipsFiberTracking.cxx)

  TARGET_LINK_LIBRARIES(itkFiberTracking
  ITKTensor
  vtkIO
  vtkCommon
  ${TTK_MIPS_LIBRARIES}
  ${ITK_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  ADD_EXECUTABLE(VTK2Fib VTK2Fib.cxx)

  TARGET_LINK_LIBRARIES(VTK2Fib
  ITKTensor
  vtkIO
  vtkCommon
  ${TTK_MIPS_LIBRARIES}
  ${ITK_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  ADD_EXECUTABLE(itkWarpFiberBundle itkWarpFiberBundle.cxx)

  TARGET_LINK_LIBRARIES(itkWarpFiberBundle
  vtkIO
  vtkCommon
  vtkRendering
  ${ITK_LIBRARIES}
  )

  # A Hessian calculator
  ADD_EXECUTABLE(itkHessianTensorImageFilter itkHessianTensorImageFilter.cxx)

  TARGET_LINK_LIBRARIES(itkHessianTensorImageFilter
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  # A Nrrd file format converter
  ADD_EXECUTABLE(itkNrrdToTensor itkNeerdToTensor.cxx)
  TARGET_LINK_LIBRARIES(itkNrrdToTensor
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  )

  # A program to resample tensors
  ADD_EXECUTABLE(itkResampleTensorImage itkResampleTensorImage.cxx)

  TARGET_LINK_LIBRARIES(itkResampleTensorImage
  ITKTensor
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  # A 2nd program to resample tensors
  ADD_EXECUTABLE(itkResampleTensorImage2 itkResampleTensorImage2.cxx)

  TARGET_LINK_LIBRARIES(itkResampleTensorImage2
  ITKTensor
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  # A tensor to scalar function calculator
  ADD_EXECUTABLE(itkTensorToScalar itkTensorToScalar.cxx)

  TARGET_LINK_LIBRARIES(itkTensorToScalar
  ITKCommon
  ITKIO
  vtkIO
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )

  # A Tensors to DWI converter
  ADD_EXECUTABLE(itkTensorsToDWI itkTensorsToDWI.cxx)

  TARGET_LINK_LIBRARIES(itkTensorsToDWI
  ITKCommon
  ITKIO
  ${TTK_MIPS_LIBRARIES}
  vtkIO
  )

  ADD_EXECUTABLE(itkGenerateSyntheticTensorField generateSyntheticTensorField.cxx)

  TARGET_LINK_LIBRARIES(itkGenerateSyntheticTensorField
  vtkIO
  ITKTensor
  ${ITK_LIBRARIES}  
  ${TTK_MIPS_LIBRARIES}
  ${TTK_LAPACK_LIBRARIES}
  )



  ADD_EXECUTABLE(itkAddGaussianNoiseToTensorDWIs
    itkAddGaussianNoiseToTensorDWIs.cxx
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
  
  ADD_EXECUTABLE(itkBoostTensorAnisotropy
    itkBoostTensorAnisotropy.cxx
  )      
  TARGET_LINK_LIBRARIES(itkBoostTensorAnisotropy
    ITKCommon
    ITKIO
    ITKTensor
    vtkIO
    ${TTK_MIPS_LIBRARIES}
    ${TTK_LAPACK_LIBRARIES}
  )
  

  ADD_EXECUTABLE(itkTensorsToVTKUnstructuredGrid
    itkTensorsToVTKUnstructuredGrid.cxx
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
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    COMPONENT Development
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
