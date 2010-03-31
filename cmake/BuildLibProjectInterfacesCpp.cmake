set( _lib_name ${SPOAC_PROJECT_NAME_CAP}Interfaces )
set( _lib_namespace ${SPOAC_PROJECT_NAME_LOWER} )
set( _lib_version ${SPOAC_PROJECT_VERSION} )

set( build TRUE )
#GBX_REQUIRE_OPTION( build LIB ${_lib_name} ON )

if( build )
    #
    # Defines macros for parsing Slice source files.
    #
    include( ${SPOAC_CMAKE_DIR}/Slice2Cpp.cmake )

    #
    # All Slice source files are defined in one place (loads SPOAC_SLICE_SOURCE_FILES)
    #
    include( ${PROJECT_SOURCE_DIR}/src/interfaces/slice/slice_sources.cmake )

    #
    # Work out the list of generated files from the list of slice sources
    #
    SPOAC_GENERATE_SLICE2CPP_RULES( _gen_sources _gen_headers ${SPOAC_SLICE_SOURCE_FILES} )

    include( ${SPOAC_CMAKE_DIR}/UseBasicRules.cmake )
    include( ${SPOAC_CMAKE_DIR}/UseIce.cmake )
    include( ${SPOAC_CMAKE_DIR}/UseLibProjectInterfaces.cmake )

    include_directories( ${PROJECT_BINARY_DIR}/src/interfaces/cpp )

    # IceStorm is not included in UseIce.cmake
    set( _dep_libs IceStorm )

    GBX_ADD_LIBRARY( ${_lib_name} DEFAULT ${_lib_version} ${_gen_sources} )
    target_link_libraries( ${_lib_name} ${_dep_libs} )

    # for satellite projects inside a super-project, need to wait until the Spoac library is build
    foreach( _gen_header ${_gen_headers} )
        set( slice_generated_headers_full ${slice_generated_headers_full} ${CMAKE_CURRENT_BINARY_DIR}/${_gen_header} )
    endforeach( _gen_header )

    #GBX_ADD_HEADERS( ${_lib_namespace} ${slice_generated_headers_full} )

endif( build )
