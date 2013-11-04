#-----------------------------------------------------------------------------
# Settings specific to the build tree.

# The install-only section is empty for the build tree.
SET(TTK_CONFIG_INSTALL_ONLY)

# The "use" file.
SET(TTK_USE_FILE ${TTK_BINARY_DIR}/UseTTK.cmake)

# The build settings file.
SET(TTK_BUILD_SETTINGS_FILE ${TTK_BINARY_DIR}/TTKBuildSettings.cmake)

# Library directory.
SET(TTK_LIBRARY_DIRS_CONFIG ${LIBRARY_OUTPUT_PATH})

# Runtime library directory.
SET(TTK_RUNTIME_LIBRARY_DIRS_CONFIG ${LIBRARY_OUTPUT_PATH})

# Binary directory.
SET(TTK_BINARY_DIR_CONFIG ${EXECUTABLE_OUTPUT_PATH})

# Determine the include directories needed.
SET(TTK_INCLUDE_DIRS_CONFIG
  ${TTK_INCLUDE_DIRS_BUILD_TREE}
  ${TTK_INCLUDE_DIRS_SYSTEM}
)


#-----------------------------------------------------------------------------
# Configure TTKConfig.cmake for the build tree.
CONFIGURE_FILE(${TTK_SOURCE_DIR}/TTKConfig.cmake.in
               ${TTK_BINARY_DIR}/TTKConfig.cmake @ONLY IMMEDIATE)


#-----------------------------------------------------------------------------
# Settings specific to the install tree.

# The "use" file.
SET(TTK_USE_FILE ${CMAKE_INSTALL_PREFIX}/lib/UseTTK.cmake)

# The build settings file.
SET(TTK_BUILD_SETTINGS_FILE ${CMAKE_INSTALL_PREFIX}/lib/TTKBuildSettings.cmake)

# Include directories.
SET(TTK_INCLUDE_DIRS_CONFIG
  ${CMAKE_INSTALL_PREFIX}/include
  ${TTK_INCLUDE_DIRS_SYSTEM}
)

# Link directories.
IF(CYGWIN AND BUILD_SHARED_LIBS)
  # In Cygwin programs directly link to the .dll files.
  SET(TTK_LIBRARY_DIRS_CONFIG ${CMAKE_INSTALL_PREFIX}/bin/)
ELSE(CYGWIN AND BUILD_SHARED_LIBS)
  SET(TTK_LIBRARY_DIRS_CONFIG ${CMAKE_INSTALL_PREFIX}/lib/)
ENDIF(CYGWIN AND BUILD_SHARED_LIBS)

# Runtime directories.
IF(WIN32)
  SET(TTK_RUNTIME_LIBRARY_DIRS_CONFIG ${CMAKE_INSTALL_PREFIX}/bin/)
ELSE(WIN32)
  SET(TTK_RUNTIME_LIBRARY_DIRS_CONFIG ${CMAKE_INSTALL_PREFIX}/lib/)
ENDIF(WIN32)

# Binary directory.
SET(TTK_BINARY_DIR_CONFIG ${CMAKE_INSTALL_PREFIX}/bin/)

#-----------------------------------------------------------------------------
# Configure TTKConfig.cmake for the install tree.
CONFIGURE_FILE(${TTK_SOURCE_DIR}/TTKConfig.cmake.in
               ${TTK_BINARY_DIR}/Utilities/TTKConfig.cmake @ONLY IMMEDIATE)
