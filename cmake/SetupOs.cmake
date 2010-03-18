#
# Based on gearbox.
#

#
# Check the OS type.
# Sets the 'SPOAC_OS_INCLUDES' variable, for system-wide Includes that must be set.
#


# CMake does not distinguish Linux from other Unices.
string( REGEX MATCH Linux SPOAC_OS_LINUX ${CMAKE_SYSTEM_NAME})

# Rename CMake's variable to something which makes more sense.
if( QNXNTO )
    set( SPOAC_OS_QNX TRUE BOOL INTERNAL )
endif( QNXNTO )

# In windows we just mirror CMake's own variable
if( WIN32 )
    set( SPOAC_OS_WIN TRUE BOOL INTERNAL )
endif( WIN32 )

# In MacOS X we just mirror CMake's own variable
if( APPLE )
    set( SPOAC_OS_MAC TRUE BOOL INTERNAL )
endif( APPLE )


# From now on, use our own OS flags

if( SPOAC_OS_LINUX )
    message( STATUS "Running on Linux" )

    # 32 or 64 bit Linux
    # Set the library directory suffix accordingly
    IF (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        SET (SPOAC_PROC_64BIT TRUE BOOL INTERNAL)
       MESSAGE (STATUS "Linux x86_64 Target Detected")
    ELSEIF (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ppc64")
        MESSAGE (STATUS "Linux ppc64 Target Detected")
        SET (SPOAC_PROC_64BIT TRUE BOOL INTERNAL)
    ENDIF (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
endif( SPOAC_OS_LINUX )

if( SPOAC_OS_QNX )
    message( STATUS "Running on QNX" )
    add_definitions( -shared -fexceptions )
endif( SPOAC_OS_QNX )

if( SPOAC_OS_WIN )
    # CMake seems not to set this property correctly for some reason
    set( SPOAC_EXE_EXTENSION ".exe" )
    message( STATUS "Running on Windows" )
endif( SPOAC_OS_WIN )

if( SPOAC_OS_MAC )
    message( STATUS "Running on OS-X" )
endif( SPOAC_OS_MAC )
