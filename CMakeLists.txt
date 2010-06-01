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


add_subdirectory(Common)
add_subdirectory(Algorithms)
add_subdirectory(Commands)
  


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
