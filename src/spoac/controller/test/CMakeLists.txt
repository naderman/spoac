include(${SPOAC_CMAKE_DIR}/UseComponentRules.cmake)

link_libraries(SpoacController)
link_libraries(boost_unit_test_framework-mt)

add_executable( SequenceControllerTest SequenceControllerTest.cpp )
GBX_ADD_TEST( spoac_SequenceController SequenceControllerTest )

add_executable( IceNetworkControllerTest IceNetworkControllerTest.cpp )
GBX_ADD_TEST( spoac_IceNetworkController IceNetworkControllerTest )
