project(IPF)

cmake_minimum_required(VERSION 2.6)


# On Visual Studio 8 MS deprecated C. This removes all 1.276E1265 security
# warnings. Copied from ITK CMakeLists.
IF(WIN32)
  IF(NOT BORLAND)
    IF(NOT CYGWIN)
      IF(NOT MINGW)
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
      ENDIF(NOT MINGW)
    ENDIF(NOT CYGWIN)
  ENDIF(NOT BORLAND)
ENDIF(WIN32)

#
# Define EXECUTABLE_OUTPUT_PATH and LIBRARY_OUTPUT_PATH
#
SET(EXECUTABLE_OUTPUT_PATH 
 ${CMAKE_BINARY_DIR}/bin 
 CACHE PATH "Single output directory for building all executables."
)

SET(LIBRARY_OUTPUT_PATH 
 ${CMAKE_BINARY_DIR}/lib 
 CACHE PATH "Single output directory for building all libraries."
)

set(IPF_REQUIRED_C_FLAGS)
set(IPF_REQUIRED_CXX_FLAGS)

include(${IPF_SOURCE_DIR}/ipfIncludeDirectories.cmake)
include_directories(
${IPF_INCLUDE_DIRS_SOURCE_TREE}
)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# set IPF_DIR so it can be used by subprojects
SET(IPF_DIR "${PROJECT_BINARY_DIR}" CACHE INTERNAL "IPF_DIR variable to be used by subprojects")

set(LIBRARY_STYLE)
if (ITK_BUILD_SHARED)
  set (LIBRARY_STYLE "SHARED")
else (ITK_BUILD_SHARED)
  set (LIBRARY_STYLE "STATIC")
endif (ITK_BUILD_SHARED)

add_library(ITKProgramFactory ${LIBRARY_STYLE}
itkCommandObjectBase.cxx
itkCommandObjectFactory.cxx
)

target_link_libraries(ITKProgramFactory
${ITK_LIBRARIES}
)

add_executable(programFactoryTest
itkHelloWorldCommand.cxx
itkHelloWorldCommandFactory.cxx
programFactoryTest.cxx
)

target_link_libraries(programFactoryTest
ITKProgramFactory
)

file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
install(FILES ${__files1} ${__files2}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  COMPONENT Development
)
	
install(TARGETS ITKProgramFactory
DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
COMPONENT RuntimeLibraries
)

install(TARGETS programFactoryTest
DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
COMPONENT RuntimeLibraries
)

#-----------------------------------------------------------------------------
# Help other projects use IPF.

CONFIGURE_FILE(${IPF_SOURCE_DIR}/UseIPF.cmake.in
               ${IPF_BINARY_DIR}/UseIPF.cmake COPYONLY IMMEDIATE)

# Save the compiler settings so another project can import them.
SET(IPF_BUILD_SETTINGS_FILE ${IPF_BINARY_DIR}/IPFBuildSettings.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)
CMAKE_EXPORT_BUILD_SETTINGS(${IPF_BUILD_SETTINGS_FILE})

# Create the IPFConfig.cmake file containing the IPF configuration.
INCLUDE (${IPF_SOURCE_DIR}/ipfGenerateIPFConfig.cmake)

INSTALL_FILES(/lib .cmake
 ${IPF_BINARY_DIR}/IPFBuildSettings
 ${IPF_BINARY_DIR}/UseIPF
 ${IPF_BINARY_DIR}/Utilities/IPFConfig
)
