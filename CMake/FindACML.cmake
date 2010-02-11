# find the ACML library


find_path(ACML_INCLUDE_DIR acml.h
 ${ACML_DIR}/include
)

if (WIN32)

  find_library(ACML_LIBRARY libacml_dll
    ${ACML_DIR}/lib
  )
  
else (WIN32)

  find_library(ACML_LIBRARY acml
    ${ACML_DIR}/lib
  )

endif (WIN32)

if (ACML_INCLUDE_DIR AND ACML_LIBRARY)
  set (ACML_FOUND "YES")
endif()


if (NOT ACML_FOUND)
  set( ACML_DIR "" CACHE PATH "Root of ACML install tree (optional)." )
  mark_as_advanced( ACML_DIR )
endif (NOT ACML_FOUND)
