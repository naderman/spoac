include(${SPOAC_CMAKE_DIR}/UseComponentRules.cmake)

link_libraries(SpoacSTM)
link_libraries(boost_unit_test_framework-mt)

add_executable( ObjectTest ObjectTest.cpp )
GBX_ADD_TEST( spoac_Object ObjectTest )

add_executable( ObjectVectorTest ObjectVectorTest.cpp )
GBX_ADD_TEST( spoac_ObjectVector ObjectVectorTest )
