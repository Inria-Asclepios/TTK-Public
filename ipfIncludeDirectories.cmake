#-----------------------------------------------------------------------------
# Include directories for other projects installed on the system.
SET(IPF_INCLUDE_DIRS_SYSTEM "")


# These directories are always needed.
SET(IPF_INCLUDE_DIRS_BUILD_TREE 
${IPF_SOURCE_DIR}
)


#-----------------------------------------------------------------------------
# Include directories from the install tree.

SET(IPF_INSTALL_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}/include/")

SET(IPF_INCLUDE_DIRS_INSTALL_TREE 
${IPF_INSTALL_INCLUDE_PATH}
)
