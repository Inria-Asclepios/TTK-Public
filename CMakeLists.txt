project(TTK)

cmake_minimum_required(VERSION 2.4)
IF(COMMAND cmake_policy)
  cmake_policy(VERSION 2.4)
  cmake_policy(SET CMP0005 OLD)
  cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

SET(EXECUTABLE_OUTPUT_PATH 
  ${CMAKE_BINARY_DIR}/bin 
  CACHE PATH "Single output directory for building all executables."
  )
SET(LIBRARY_OUTPUT_PATH 
  ${CMAKE_BINARY_DIR}/lib 
  CACHE PATH "Single output directory for building all libraries."
  )

find_package(ITK)
find_package(VTK)

if( ITK_FOUND AND VTK_FOUND )

include(${ITK_USE_FILE})
include(${VTK_USE_FILE})

option(TTK_USE_MIPS "Use MIPS and make TTK compatible with inrimage file format (discouraged)")
if(TTK_USE_MIPS)
  find_package(MIPS REQUIRED)
  include(${MIPS_USE_FILE})
  link_libraries(mipsItkImageIO InrimageNG LinearAlgebra Basics mipsZ) # ugly but I don't want to detail dependencies for each project

  if(MIPS_USE_MKL OR MIPS_USE_ACML)
    if(MIPS_USE_MKL)
      link_directories(${MKL_LINK_DIRS})
      link_libraries(${MKL_LIBRARIES})
    else(MIPS_USE_MKL)
      link_directories(${ACML_LINK_DIRS})
      link_libraries(${ACML_LIBRARIES})
    endif(MIPS_USE_MKL)
  endif(MIPS_USE_MKL OR MIPS_USE_ACML)

endif(TTK_USE_MIPS)

configure_file(${PROJECT_SOURCE_DIR}/ttkConfigure.h.in
               ${PROJECT_BINARY_DIR}/ttkConfigure.h CONFIGURE IMMEDIATE)


set(TTK_REQUIRED_C_FLAGS)
set(TTK_REQUIRED_CXX_FLAGS)


include(${TTK_SOURCE_DIR}/ttkIncludeDirectories.cmake)
include_directories(
${TTK_INCLUDE_DIRS_BUILD_TREE}
${TTK_INCLUDE_DIRS_SOURCE_TREE}
)

SET(TTK_DIR "${PROJECT_BINARY_DIR}" CACHE INTERNAL "TTK_DIR variable if required")

add_library(ITKTensor
itkNumericTraitsTensorPixel2.cxx
)
TARGET_LINK_LIBRARIES(ITKTensor
ITKCommon
)
		
#  IF (VTK_FOUND)
#    LINK_LIBRARIES(ITKTensor vtkIO vtkCommon ${LAPACK_LIBRARIES})
#  ELSE (VTK_FOUND)
#    LINK_LIBRARIES(ITKTensor ${LAPACK_LIBRARIES})
#  ENDIF (VTK_FOUND)


INSTALL(TARGETS ITKTensor DESTINATION lib/)


# A simple program to average several images
ADD_EXECUTABLE(itkAverageStdImageFilter itkAverageStdImageFilter.cxx)
  
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
# LinearAlgebra
)
  
TARGET_LINK_LIBRARIES(itkResampleImage
ITKCommon
ITKIO
# LinearAlgebra
)
  
TARGET_LINK_LIBRARIES(itkB0BrainMaskExtractor
ITKCommon
ITKIO
# LinearAlgebra
)

TARGET_LINK_LIBRARIES(itkApplyMaskToImage
ITKTensor
ITKCommon
ITKIO
vtkIO
# InrimageNG
)
  
TARGET_LINK_LIBRARIES(itkRead4DDWI
ITKCommon
ITKIO
# LinearAlgebra
)
  
TARGET_LINK_LIBRARIES(imconvert
ITKCommon
ITKIO
# mipsItkImageIO
# LinearAlgebra
)

TARGET_LINK_LIBRARIES(itkStanleyToITK
ITKCommon
ITKIO
# LinearAlgebra
)
  
TARGET_LINK_LIBRARIES(vtkConsolidateFiberBundle
vtkIO
# LinearAlgebra
)

TARGET_LINK_LIBRARIES(itkTensorAnisotropicFilter
ITKCommon
ITKIO
vtkIO
# InrimageNG
)

TARGET_LINK_LIBRARIES(itkDTIEstimator
ITKCommon
ITKIO
vtkIO
# InrimageNG
)

TARGET_LINK_LIBRARIES(itkRemoveNPT
ITKCommon
ITKIO
vtkIO
# InrimageNG
)

TARGET_LINK_LIBRARIES(itkFlipTensors
ITKCommon
ITKIO
vtkIO
# InrimageNG
)

TARGET_LINK_LIBRARIES(itkAddNoiseToTensors
ITKCommon
ITKStatistics
ITKIO
vtkIO
# InrimageNG
)

TARGET_LINK_LIBRARIES(itkNormalizeTensors
ITKCommon
ITKStatistics
ITKIO
vtkIO
# InrimageNG
)

TARGET_LINK_LIBRARIES(itkDTIEstimatorWithBFGS
ITKCommon
ITKIO
vtkIO
# InrimageNG
)
  
# A Log-Euclidean dti estimator
ADD_EXECUTABLE(itkLogEuclideanDTIEstimator itkLogEuclideanDTIEstimator.cxx)

TARGET_LINK_LIBRARIES(itkLogEuclideanDTIEstimator
ITKCommon
ITKIO
vtkIO
# InrimageNG
)

# A Tensors to DWI converter
ADD_EXECUTABLE(itkWarpTensorImage itkWarpTensorImage.cxx)
TARGET_LINK_LIBRARIES(itkWarpTensorImage
ITKTensor
ITKCommon
# InrimageNG
ITKIO
vtkIO
)

# A Tensor Gaussian smoother
ADD_EXECUTABLE(itkGaussianTensorImageFilter itkGaussianTensorImageFilter.cxx)
TARGET_LINK_LIBRARIES(itkGaussianTensorImageFilter
ITKTensor
ITKCommon
ITKIO
vtkIO
# InrimageNG
)

# A tensor z-score calculator 
ADD_EXECUTABLE(itkZScoreTensorImageFilter itkZScoreTensorImageFilter.cxx)
TARGET_LINK_LIBRARIES(itkZScoreTensorImageFilter
ITKCommon
ITKIO
vtkIO
# InrimageNG
)
		
# A tensor statistics calculator 
ADD_EXECUTABLE(itkAverageStdTensorImageFilter itkAverageStdTensorImageFilter.cxx)
TARGET_LINK_LIBRARIES(itkAverageStdTensorImageFilter
ITKCommon
ITKIO
vtkIO
# InrimageNG
)

# A library to read a vtk tensor file
ADD_LIBRARY(VTKTensorImageIO STATIC itkVTKStructuredPointsTensorImageIO.cxx)
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
# InrimageNG
)

# A RBF Tensor Interpolator
ADD_EXECUTABLE(itkRBFTensorInterpolation itkRBFTensorInterpolation.cxx)
		
TARGET_LINK_LIBRARIES(itkRBFTensorInterpolation
ITKCommon
# InrimageNG
ITKIO
vtkIO
vtkCommon
)

# A RBF Rotation Matrix Interpolator
ADD_EXECUTABLE(itkRBFRotationMatrixInterpolation itkRBFRotationMatrixInterpolation.cxx)
		
TARGET_LINK_LIBRARIES(itkRBFRotationMatrixInterpolation
ITKCommon
# InrimageNG
ITKIO
vtkIO
vtkCommon
)

ADD_EXECUTABLE(itkNormalizedGaussianTensorInterpolation itkNormalizedGaussianTensorInterpolation.cxx)
TARGET_LINK_LIBRARIES(itkNormalizedGaussianTensorInterpolation
ITKCommon
# InrimageNG
ITKIO
vtkIO
vtkCommon
)

ADD_EXECUTABLE(itkFiberTracking mipsFiberTracking.cxx)

TARGET_LINK_LIBRARIES(itkFiberTracking
ITKTensor
ITKIO
vtkIO
vtkCommon
${ITK_LIBRARIES}
# InrimageNG
)

ADD_EXECUTABLE(VTK2Fib VTK2Fib.cxx)

TARGET_LINK_LIBRARIES(VTK2Fib
ITKTensor
vtkIO
vtkCommon
${ITK_LIBRARIES}
)

ADD_EXECUTABLE(itkWarpFiberBundle itkWarpFiberBundle.cxx)

TARGET_LINK_LIBRARIES(itkWarpFiberBundle
vtkIO
vtkCommon
vtkRendering
# LinearAlgebra
${ITK_LIBRARIES}
)

# A Hessian calculator
ADD_EXECUTABLE(itkHessianTensorImageFilter itkHessianTensorImageFilter.cxx)

TARGET_LINK_LIBRARIES(itkHessianTensorImageFilter
ITKCommon
ITKIO
vtkIO
# InrimageNG
)

# A Nrrd file format converter
ADD_EXECUTABLE(itkNrrdToTensor itkNeerdToTensor.cxx)
TARGET_LINK_LIBRARIES(itkNrrdToTensor
ITKCommon
ITKIO
vtkIO
# InrimageNG
)

# A program to resample tensors
ADD_EXECUTABLE(itkResampleTensorImage itkResampleTensorImage.cxx)

TARGET_LINK_LIBRARIES(itkResampleTensorImage
ITKTensor
ITKCommon
ITKIO
vtkIO
# InrimageNG
)

# A 2nd program to resample tensors
ADD_EXECUTABLE(itkResampleTensorImage2 itkResampleTensorImage2.cxx)

TARGET_LINK_LIBRARIES(itkResampleTensorImage2
ITKTensor
ITKCommon
ITKIO
vtkIO
# InrimageNG
)

# A tensor to scalar function calculator
ADD_EXECUTABLE(itkTensorToScalar itkTensorToScalar.cxx)

TARGET_LINK_LIBRARIES(itkTensorToScalar
ITKCommon
ITKIO
vtkIO
# InrimageNG
)

# A Tensors to DWI converter
ADD_EXECUTABLE(itkTensorsToDWI itkTensorsToDWI.cxx)

TARGET_LINK_LIBRARIES(itkTensorsToDWI
ITKCommon
# InrimageNG
ITKIO
vtkIO
)


if(TTK_USE_MIPS)
 # An Inria to VTK tensor file format converter 
  ADD_EXECUTABLE(inr2vtk mipsInrimageToVTK.cxx)
  TARGET_LINK_LIBRARIES(inr2vtk
  ITKCommon
  vtkIO
  InrimageNG
  )

  # A VTK tensor file format to Inria converter 
  ADD_EXECUTABLE(vtk2inr mipsVTKToInrimage.cxx)
  TARGET_LINK_LIBRARIES(vtk2inr
    ITKCommon
    vtkIO
    InrimageNG
    )
endif(TTK_USE_MIPS)


#   # A Tensors to DWI converter
	#   ADD_EXECUTABLE(itkMultiResolutionTensorFilter
	#   itkMultiResolutionTensorFilter.cxx
	#   )
	#   TARGET_LINK_LIBRARIES(itkMultiResolutionTensorFilter
	#   ITKCommon
	#   InrimageNG
	#   ITKIO
	#   )


#  IF(VTK_FOUND)
		
#   A tensor field gradient magnitude calculator
#	   ADD_EXECUTABLE(itkGradientMagnitudeTensorImageFilter
#	   itkGradientMagnitudeTensorImageFilter.cxx
#	   )
#     TARGET_LINK_LIBRARIES(itkGradientMagnitudeTensorImageFilter
#	   InrimageNG
#	   ITKCommon
#	   ITKIO
#	   )

    # A Tensor derivative Gaussian filter
    #ADD_EXECUTABLE(itkMultiscaleGaussianDerivativeTensorImageFilter
    #itkMultiscaleGaussianDerivativeTensorImageFilter.cxx
    #)
    #TARGET_LINK_LIBRARIES(itkMultiscaleGaussianDerivativeTensorImageFilter
    #ITKCommon
    #InrimageNG
    #ITKIO
    #)



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

# IF(ITK_FOUND)

	#   IF(USE_MKL AND MKL_FOUND)
	#      INCLUDE_DIRECTORIES(${MKL_INCLUDE_DIRS})
	#      LINK_DIRECTORIES(${MKL_LINK_DIRS})
	#      LINK_LIBRARIES(${MKL_LIBRARIES})
	#      ADD_DEFINITIONS(-DMKL_IN_USE)
	#   ENDIF(USE_MKL AND MKL_FOUND)

	#   IF(USE_ACML AND ACML_FOUND)
	#    INCLUDE_DIRECTORIES(${ACML_INCLUDE_DIRS})
	#    LINK_DIRECTORIES(${ACML_LINK_DIRS})
	#    LINK_LIBRARIES(${ACML_LIBRARIES})
	#    ADD_DEFINITIONS(-DACML_IN_USE)
	#   ENDIF(USE_ACML AND ACML_FOUND)
  

else( ITK_FOUND AND VTK_FOUND )

message(WARNING "ITK or VTK was not set, TensorToolKit will not be built.")

endif( ITK_FOUND AND VTK_FOUND )
