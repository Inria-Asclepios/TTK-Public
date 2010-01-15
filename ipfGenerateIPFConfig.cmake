#-----------------------------------------------------------------------------
# Settings specific to the build tree.

# The install-only section is empty for the build tree.
SET(IPF_CONFIG_INSTALL_ONLY)

# The "use" file.
SET(IPF_USE_FILE ${IPF_BINARY_DIR}/UseIPF.cmake)

# The build settings file.
SET(IPF_BUILD_SETTINGS_FILE ${IPF_BINARY_DIR}/IPFBuildSettings.cmake)

# Library directory.
SET(IPF_LIBRARY_DIRS_CONFIG ${LIBRARY_OUTPUT_PATH})

# Runtime library directory.
SET(IPF_RUNTIME_LIBRARY_DIRS_CONFIG ${LIBRARY_OUTPUT_PATH})

# Determine the include directories needed.
SET(IPF_INCLUDE_DIRS_CONFIG
  ${IPF_INCLUDE_DIRS_BUILD_TREE}
  ${IPF_INCLUDE_DIRS_SYSTEM}
)


#-----------------------------------------------------------------------------
# Configure IPFConfig.cmake for the build tree.
CONFIGURE_FILE(${IPF_SOURCE_DIR}/IPFConfig.cmake.in
               ${IPF_BINARY_DIR}/IPFConfig.cmake @ONLY IMMEDIATE)


#-----------------------------------------------------------------------------
# Settings specific to the install tree.

# The "use" file.
SET(IPF_USE_FILE ${CMAKE_INSTALL_PREFIX}/lib/UseIPF.cmake)

# The build settings file.
SET(IPF_BUILD_SETTINGS_FILE ${CMAKE_INSTALL_PREFIX}/lib/IPFBuildSettings.cmake)

# Include directories.
SET(IPF_INCLUDE_DIRS_CONFIG
  ${CMAKE_INSTALL_PREFIX}/include
  ${IPF_INCLUDE_DIRS_SYSTEM}
)

# Link directories.
IF(CYGWIN AND BUILD_SHARED_LIBS)
  # In Cygwin programs directly link to the .dll files.
  SET(IPF_LIBRARY_DIRS_CONFIG ${CMAKE_INSTALL_PREFIX}/bin/)
ELSE(CYGWIN AND BUILD_SHARED_LIBS)
  SET(IPF_LIBRARY_DIRS_CONFIG ${CMAKE_INSTALL_PREFIX}/lib/)
ENDIF(CYGWIN AND BUILD_SHARED_LIBS)

# Runtime directories.
IF(WIN32)
  SET(IPF_RUNTIME_LIBRARY_DIRS_CONFIG ${CMAKE_INSTALL_PREFIX}/bin/)
ELSE(WIN32)
  SET(IPF_RUNTIME_LIBRARY_DIRS_CONFIG ${CMAKE_INSTALL_PREFIX}/lib/)
ENDIF(WIN32)


#-----------------------------------------------------------------------------
# Configure IPFConfig.cmake for the install tree.
CONFIGURE_FILE(${IPF_SOURCE_DIR}/IPFConfig.cmake.in
               ${IPF_BINARY_DIR}/Utilities/IPFConfig.cmake @ONLY IMMEDIATE)
