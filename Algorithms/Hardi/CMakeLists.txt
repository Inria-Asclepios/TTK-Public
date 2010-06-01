ADD_LIBRARY(fodf
  sphericalHarmonics.cpp
  node.cpp
  fiber.cpp
  tract.cpp
  fibersTrackerHARDI.cpp
  vector3.cpp
  vertex3.cpp
  )

TARGET_LINK_LIBRARIES(fodf
  ${ITK_LIBRARIES}
  )

install(TARGETS fodf 
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )
