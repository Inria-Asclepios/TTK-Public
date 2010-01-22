#-----------------------------------------------------------------------------
# Include directories for other projects installed on the system.
SET(TTK_INCLUDE_DIRS_SYSTEM "")

#-----------------------------------------------------------------------------
# Include directories from the build tree.
SET(TTK_INCLUDE_DIRS_BUILD_TREE ${PROJECT_BINARY_DIR})

# These directories are always needed.
SET(TTK_INCLUDE_DIRS_BUILD_TREE 
${TTK_INCLUDE_DIRS_BUILD_TREE}
${TTK_SOURCE_DIR}
)


#-----------------------------------------------------------------------------
# Include directories from the install tree.

SET(TTK_INSTALL_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}/include/")

SET(TTK_INCLUDE_DIRS_INSTALL_TREE 
${TTK_INCLUDE_DIRS_INSTALL_TREE}
${TTK_INSTALL_INCLUDE_PATH}
)
