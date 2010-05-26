include(${SPOAC_CMAKE_DIR}/UseComponentRules.cmake)

link_libraries(SpoacCommon)
link_libraries(boost_unit_test_framework-mt)

add_executable( DependencyManagerTest DependencyManagerTest.cpp )
GBX_ADD_TEST( spoac_DependencyManager DependencyManagerTest )
