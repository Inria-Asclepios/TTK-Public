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
set(${PROJECT_NAME}_VERSION_MINOR 4 CACHE STRING "TTK minor version number.")
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


# -------------------------------------------------------------------------------------
# Setup installation settings
# -------------------------------------------------------------------------------------

set(${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT 0 CACHE BOOL "Do not install development")
set(${PROJECT_NAME}_INSTALL_NO_RUNTIME     0 CACHE BOOL "Do not install runtime")

set(${PROJECT_NAME}_INSTALL_NO_LIBRARIES)
if(${PROJECT_NAME}_BUILD_SHARED_LIBS)
  if(${PROJECT_NAME}_INSTALL_NO_RUNTIME AND ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
    set(${PROJECT_NAME}_INSTALL_NO_LIBRARIES 1)
  endif(${PROJECT_NAME}_INSTALL_NO_RUNTIME AND ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
else(${PROJECT_NAME}_BUILD_SHARED_LIBS)
  if(${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
    set(${PROJECT_NAME}_INSTALL_NO_LIBRARIES 1)
  else(${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
    set(${PROJECT_NAME}_INSTALL_NO_LIBRARIES 0)
  endif(${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
endif(${PROJECT_NAME}_BUILD_SHARED_LIBS)


mark_as_advanced(${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT 
  ${PROJECT_NAME}_INSTALL_NO_RUNTIME
  ${PROJECT_NAME}_INSTALL_NO_LIBRARIES
  )


## #################################################################
## Required dependencies
## #################################################################

find_package(ITK REQUIRED)
find_package(VTK REQUIRED)


## ###################################################################
## Some packages options are required for this project. Check them now
## ###################################################################

set(_ITKVersionString "${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}.${ITK_VERSION_PATCH}" )
math(EXPR _ITKVersionNum "${ITK_VERSION_MAJOR}*100*100 + ${ITK_VERSION_MINOR}*100 + ${ITK_VERSION_PATCH}")
if( _ITKVersionNum LESS 40800 )
  message(SEND_ERROR "The ITK version you want to use (${_ITKVersionString}) is not supported by this project. Please use a more recent version of ITK. The minimum required version is 4.8.0. The recommended version is 4.8.0.")
endif()

SET(ITKIO_LIBRARIES
  ITKIOImageBase
  ITKIOBMP
  ITKIOLSM
  ITKIOJPEG
  ITKIOPNG
  ITKIOVTK
  ITKIOGDCM
  ITKIOGE
  ITKNrrdIO
  ITKIONRRD
  ITKIOMeta
  ITKIONIFTI
  ITKIOGIPL
  ITKIOHDF5
  ITKIOBioRad
  ITKIOStimulate
  ITKIOMRC
)

set(ITK_TRANSFORM_LIBRARIES
  ITKIOTransformBase
  ITKIOTransformInsightLegacy
  ITKIOTransformHDF5
  ITKIOTransformMatlab
)

## #################################################################
## Options
## #################################################################

option(TTK_USE_GMM   OFF "Use GMM to build registration (tensor demons, log domain tensor demons) module")
option(TTK_USE_MKL   OFF "Use MKL to solve eigensystems")
option(TTK_USE_ACML  OFF "Use ACML to solve eigensystems")
option(TTK_USE_MIPS  OFF "Use MIPS and make TTK compatible with inrimage file format (discouraged)")
option(TTK_USE_VTKINRIA3D  OFF "Use vtkINRIA3D and make TTK compatible with GIS file format")


if (TTK_USE_GMM)
  find_path(GMM_INCLUDE_DIR gmm/gmm.h)
endif(TTK_USE_GMM)


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


set(TTK_VTKINRIA3D_LIBRARIES
)
if(TTK_USE_VTKINRIA3D)
  find_package(vtkINRIA3D REQUIRED)
  include(${vtkINRIA3D_USE_FILE})
 
  set(TTK_VTKINRIA3D_LIBRARIES 
  itkIOAddOn
  vtkImaging
  vtkVisuManagement
  vtkGraphics
  )
endif(TTK_USE_VTKINRIA3D)


# -----------------------------------------------------------------------------
# Header file used to pass CMake settings to the source code
# -----------------------------------------------------------------------------
configure_file(${PROJECT_SOURCE_DIR}/ttkConfigure.h.in
               ${PROJECT_BINARY_DIR}/ttkConfigure.h CONFIGURE IMMEDIATE)

if(NOT ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
  install(FILES ${TTK_BINARY_DIR}/ttkConfigure.h
      DESTINATION include)
endif(NOT ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)

set(TTK_REQUIRED_C_FLAGS)
set(TTK_REQUIRED_CXX_FLAGS)

include(${TTK_SOURCE_DIR}/ttkIncludeDirectories.cmake)
include_directories(
  ${TTK_INCLUDE_DIRS_BUILD_TREE}
  ${TTK_INCLUDE_DIRS_SYSTEM}
)

add_subdirectory(IPF)
find_package(IPF)
include(${IPF_USE_FILE})

include(${ITK_USE_FILE})
include(${VTK_USE_FILE})

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

if(NOT ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES ${TTK_BINARY_DIR}/TTKBuildSettings.cmake 
      ${TTK_BINARY_DIR}/UseTTK.cmake 
      ${TTK_BINARY_DIR}/Utilities/TTKConfig.cmake
   DESTINATION lib
  )
endif(NOT ${PROJECT_NAME}_INSTALL_NO_DEVELOPMENT)


#-----------------------------------------------------------------------------
# Set any extra compilation flags here
if(CMAKE_COMPILER_IS_GNUCXX)
  # Add some useful warning flags
  # Note: -Wall and -Wno-deprecated are already pulled in by
  # UseITK.cmake
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshadow")
endif(CMAKE_COMPILER_IS_GNUCXX)

#-----------------------------------------------------------------------------
# Add compiler flags TTK needs to work on this platform.  This must be
# done AFTER the call to CMAKE_EXPORT_BUILD_SETTINGS.
SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${TTK_REQUIRED_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TTK_REQUIRED_CXX_FLAGS}")


add_subdirectory(Common)
add_subdirectory(Algorithms)
if (TTK_USE_GMM)
  add_subdirectory(Registration)
endif (TTK_USE_GMM)
add_subdirectory(Commands)



#-----------------------------------------------------------------------------
# Packing stuff
#-----------------------------------------------------------------------------
if (NOT CPACK_PACKAGE_NAME)

if(NOT ${PROJECT_NAME}_INSTALL_NO_RUNTIME)
  if (EXISTS "${CMAKE_ROOT}/Modules/InstallRequiredSystemLibraries.cmake")
    set (CMAKE_INSTALL_MFC_LIBRARIES 1)
    include (InstallRequiredSystemLibraries)
  endif (EXISTS "${CMAKE_ROOT}/Modules/InstallRequiredSystemLibraries.cmake")
endif(NOT ${PROJECT_NAME}_INSTALL_NO_RUNTIME)


set (CPACK_PACKAGE_NAME                 ${PROJECT_NAME})
set (CPACK_SOURCE_PACKAGE_FILE_NAME    "ttk-${TTK_VERSION}")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Tensor ToolKit")
set (CPACK_PACKAGE_DESCRIPTION          ${PROJECT_NAME})

set (CPACK_PACKAGE_VENDOR           "INRIA France")
set (CPACK_PACKAGE_CONTACT          "Pierre Fillard <pierre.fillard@inria.fr>")
set (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
set (CPACK_PACKAGE_VERSION_MAJOR    "${TTK_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR    "${TTK_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH    "${TTK_VERSION_BUILD}")
set (CPACK_RESOURCE_FILE_LICENSE    "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")

set (CPACK_PACKAGE_INSTALL_DIRECTORY "ttk-${TTK_VERSION}")

set (CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
if (${CMAKE_SYSTEM_NAME} MATCHES Windows)
  if (CMAKE_CL_64)
    set (CPACK_SYSTEM_NAME win64)
  else (CMAKE_CL_64)
    set (CPACK_SYSTEM_NAME win32)
  endif (CMAKE_CL_64)
endif (${CMAKE_SYSTEM_NAME} MATCHES Windows)

set (CPACK_PACKAGE_FILE_NAME "${CPACK_SOURCE_PACKAGE_FILE_NAME}-${CPACK_SYSTEM_NAME}")

set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

if (WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly.
  # Make sure there is at least one set of four (4) backslashes.
  #set (CPACK_PACKAGE_EXECUTABLES "CMakeSetup" "CMake")
  #set (CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\CMakeSetup.exe")
  set (CPACK_NSIS_DISPLAY_NAME "TTK - The TensorToolKit")
  #set (CPACK_NSIS_HELP_LINK "http:\\\\\\\\")
  #set (CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\")
  set (CPACK_NSIS_MODIFY_PATH ON)
else (WIN32 AND NOT UNIX)
  #set (CPACK_STRIP_FILES "bin/ccmake;bin/cmake;bin/cpack;bin/ctest")
  #set (CPACK_PACKAGE_EXECUTABLES "")
endif (WIN32 AND NOT UNIX)



include(CPack)

ENDIF(NOT CPACK_PACKAGE_NAME)

#### TESTING stuff ###
enable_testing()
include(CTest)
if( BUILD_TESTING )
  add_subdirectory(Testing)
endif( BUILD_TESTING )
