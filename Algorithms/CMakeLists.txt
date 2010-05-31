if(TTK_USE_HARDI)
  find_package(Boost REQUIRED)
  include_directories (${Boost_INCLUDE_DIR})
  if( Boost_FOUND )
    add_subdirectory(Hardi)
  endif( Boost_FOUND )
endif(TTK_USE_HARDI)

file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  file(GLOB __files3 "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
  install(FILES ${__files1} ${__files2} ${__files3}
    DESTINATION include
    COMPONENT   Development
  )
