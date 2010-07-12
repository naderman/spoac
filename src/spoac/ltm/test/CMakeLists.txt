include(${SPOAC_CMAKE_DIR}/UseComponentRules.cmake)

link_libraries(SpoacLTM)
link_libraries(boost_unit_test_framework-mt)

add_executable( LTMTest LTMTest.cpp )
GBX_ADD_TEST( spoac_LTM LTMTest )

