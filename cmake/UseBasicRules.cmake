#include_directories(
#    ${PROJECT_SOURCE_DIR}/src
#)

#
# Platform-specific compiler and linker flags
#
if( NOT SPOAC_OS_WIN )
    add_definitions( "-Wall" )
else()
    add_definitions( "-Wall" ) #-D_CRT_SECURE_NO_DEPRECATE
endif()

