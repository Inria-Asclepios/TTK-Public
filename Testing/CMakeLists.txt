set(KIT TTK)

set(TTKTests
  itkGradientFileReadWriteTest.cxx
  )

create_test_sourcelist(Tests ${KIT}Tests.cxx
  ${TTKTests}
  )

add_executable(${KIT}Tests 
  ${Tests}
  )

target_link_libraries(${KIT}Tests 
  ITKTensor
  ${ITK_LIBRARIES}
  )

FOREACH (test ${TTKTests})
  GET_FILENAME_COMPONENT(TName ${test} NAME_WE)
  ADD_TEST(${TName} ${CXX_TEST_PATH}/${KIT}Tests ${TName}) 
ENDFOREACH (test) 

