include(${SPOAC_CMAKE_DIR}/UseComponentRules.cmake)

link_libraries(SpoacLTM)
link_libraries(SpoacSTM)
link_libraries(SpoacCEA)
link_libraries(boost_unit_test_framework-mt)

add_executable( OACTest OACTest.cpp )
GBX_ADD_TEST( spoac_OAC OACTest )

add_executable( ActionTest ActionTest.cpp )
GBX_ADD_TEST( spoac_Action ActionTest )

add_executable( ActionStateMachineTest ActionStateMachineTest.cpp )
GBX_ADD_TEST( spoac_ActionStateMachine ActionStateMachineTest )

add_executable( ActivityControllerTest ActivityControllerTest.cpp )
GBX_ADD_TEST( spoac_ActivityController ActivityControllerTest )

add_executable( CEATest CEATest.cpp )
GBX_ADD_TEST( spoac_CEA CEATest )