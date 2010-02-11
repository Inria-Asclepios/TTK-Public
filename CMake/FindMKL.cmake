# find the MKL library

find_path(MKL_INCLUDE_DIR mkl.h
 ${MKL_DIR}/include
)

if (UNIX)

  find_path(MKL_guide_LIBRARY guide
    ${MKL_DIR}/lib
  )
  find_path(MKL_ia32_LIBRARY mkl_ia32
    ${MKL_DIR}/lib
  )
  find_path(MKL_lapack_LIBRARY mkl_lapack
    ${MKL_DIR}/lib
  )
  find_path(MKL_lapack32_LIBRARY mkl_lapack32
    ${MKL_DIR}/lib
  )
  find_path(MKL_mkl_LIBRARY mkl
    ${MKL_DIR}/lib
  )

  if (MKL_INCLUDE_DIR      AND
      MKL_guide_LIBRARY    AND
      MKL_ia32_LIBRARY     AND
      MKL_lapack_LIBRARY   AND
      MKL_lapack32_LIBRARY AND
      MKL_mkl_LIBRARY)
      
      set(MKL_FOUND "YES")
      set(MKL_LIBRARIES
        ${MKL_guide_LIBRARY}
        ${MKL_ia32_LIBRARY}
        ${MKL_lapack_LIBRARY}
        ${MKL_lapack32_LIBRARY}
        ${MKL_mkl_LIBRARY}
	pthread
	)
  endif()
  
endif (UNIX)


if (WIN32)

  find_path(MKL_guide40_LIBRARY libguide40
    ${MKL_DIR}/lib
  )
  find_path(MKL_guide_LIBRARY libguide
    ${MKL_DIR}/lib
  )
  find_path(MKL_ia32_LIBRARY mkl_ia32
    ${MKL_DIR}/lib
  )
  find_path(MKL_lapack_LIBRARY mkl_lapack
    ${MKL_DIR}/lib
  )
  find_path(MKL_c_LIBRARY mkl_c
    ${MKL_DIR}/lib
  )
  find_path(MKL_solver_LIBRARY mkl_solver
    ${MKL_DIR}/lib
  )

  if (MKL_INCLUDE_DIR      AND
      MKL_guide40_LIBRARY  AND
      MKL_guide_LIBRARY    AND
      MKL_ia32_LIBRARY     AND
      MKL_lapack_LIBRARY   AND
      MKL_c_LIBRARY        AND
      MKL_solver_LIBRARY)
      
      set(MKL_FOUND "YES")
      set(MKL_LIBRARIES
        ${MKL_guide40_LIBRARY}
        ${MKL_guide_LIBRARY}
        ${MKL_ia32_LIBRARY}
        ${MKL_lapack_LIBRARY}
        ${MKL_c_LIBRARY}
	${MKL_solver_LIBRARY}
	)
  endif()
  
endif (WIN32)


if (APPLE)

  find_path(MKL_guide_LIBRARY libguide
    ${MKL_DIR}/lib
  )
  find_path(MKL_ia32_LIBRARY mkl_ia32
    ${MKL_DIR}/lib
  )
  find_path(MKL_lapack_LIBRARY mkl_lapack
    ${MKL_DIR}/lib
  )

  if (MKL_INCLUDE_DIR      AND
      MKL_guide_LIBRARY    AND
      MKL_ia32_LIBRARY     AND
      MKL_lapack_LIBRARY)
      
      set(MKL_FOUND "YES")
      set(MKL_LIBRARIES
        ${MKL_guide_LIBRARY}
        ${MKL_ia32_LIBRARY}
        ${MKL_lapack_LIBRARY}
	)
  endif()
  
endif (APPLE)

if (NOT MKL_FOUND)
  set(MKL_DIR "" CACHE PATH "Root of MKL install tree (optional)." )
  mark_as_advanced( MKL_DIR )
endif (NOT MKL_FOUND)
