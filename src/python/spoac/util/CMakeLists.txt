set(build TRUE)

if (build)

    if (PYTHONINTERP_FOUND AND SPOAC_BUILD_PYTHON)

        message(STATUS "Planning to build Python library: util")

	file(GLOB srcs *.py)

        install( FILES ${srcs} DESTINATION ${GBX_SHARE_INSTALL_DIR}/python/spoac/util/ )


    else (PYTHONINTERP_FOUND AND SPOAC_BUILD_PYTHON)
        message(STATUS "Will not build Python library util: PYTHONINTERP_FOUND=${PYTHON_FOUND}, SPOAC_BUILD_PYTHON=${SPOAC_BUILD_PYTHON}")
    endif (PYTHONINTERP_FOUND AND SPOAC_BUILD_PYTHON)

endif (build)

