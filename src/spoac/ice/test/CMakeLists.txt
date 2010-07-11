include(${SPOAC_CMAKE_DIR}/UseComponentRules.cmake)

link_libraries(SpoacIce)
link_libraries(boost_unit_test_framework-mt)

add_executable( IceHelperTest IceHelperTest.cpp )
GBX_ADD_TEST( spoac_IceHelper IceHelperTest )
