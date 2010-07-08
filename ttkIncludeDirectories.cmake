#-----------------------------------------------------------------------------
# Include directories for other projects installed on the system.
SET(TTK_INCLUDE_DIRS_SYSTEM "")

IF (TTK_USE_GMM)  
  set(TTK_INCLUDE_DIRS_SYSTEM
    ${TTK_INCLUDE_DIRS_SYSTEM}
    ${GMM_INCLUDE_DIR}
  )
ENDIF (TTK_USE_GMM)  

IF (TTK_USE_MKL)  
  SET(TTK_INCLUDE_DIRS_SYSTEM
  ${TTK_INCLUDE_DIRS_SYSTEM}
  ${MKL_INCLUDE_DIR}
  )
ENDIF(TTK_USE_MKL)

IF (TTK_USE_ACML)  
  SET(TTK_INCLUDE_DIRS_SYSTEM
  ${TTK_INCLUDE_DIRS_SYSTEM}
  ${ACML_INCLUDE_DIR}  
  )
ENDIF(TTK_USE_ACML)

IF(Boost_FOUND)
  SET(TTK_INCLUDE_DIRS_SYSTEM
  ${TTK_INCLUDE_DIRS_SYSTEM}
  ${Boost_INCLUDE_DIR}
  )
ENDIF(Boost_FOUND)

#-----------------------------------------------------------------------------
# Include directories from the build tree.
SET(TTK_INCLUDE_DIRS_BUILD_TREE ${PROJECT_BINARY_DIR})

# These directories are always needed.
SET(TTK_INCLUDE_DIRS_BUILD_TREE 
${TTK_INCLUDE_DIRS_BUILD_TREE}
${TTK_SOURCE_DIR}
${TTK_SOURCE_DIR}/Common
${TTK_SOURCE_DIR}/Algorithms
${TTK_SOURCE_DIR}/Algorithms/Hardi
${TTK_SOURCE_DIR}/Registration
${TTK_SOURCE_DIR}/Commands
)


#-----------------------------------------------------------------------------
# Include directories from the install tree.

SET(TTK_INSTALL_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}/include/")

SET(TTK_INCLUDE_DIRS_INSTALL_TREE 
${TTK_INCLUDE_DIRS_INSTALL_TREE}
${TTK_INSTALL_INCLUDE_PATH}
)
