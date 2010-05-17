ORCA_SET_LIBRARY_NAME(SpoacSTM)
set(LIB_INSTALL stm)

set(build TRUE)
GBX_REQUIRE_OPTION(build LIB ${LIB_NAME} ON)

set( dep_libs SpoacCommon )
GBX_REQUIRE_LIBS( build LIB ${LIB_NAME} ${dep_libs} )

if (build)

    include(${SPOAC_CMAKE_DIR}/UseComponentRules.cmake)

    file(GLOB hdrs *.h)
    file(GLOB srcs *.cpp)

    GBX_ADD_LIBRARY(${LIB_NAME} DEFAULT ${LIB_VERSION} ${srcs})
    target_link_libraries(${LIB_NAME} ${dep_libs})
    GBX_ADD_HEADERS(${LIB_INSTALL} ${hdrs})

    if (SPOAC_BUILD_TESTS)
        add_subdirectory(test)
    endif (SPOAC_BUILD_TESTS)

endif (build)
