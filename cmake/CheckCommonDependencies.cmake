###########################################################
#                                                         #
# Look for dependencies required by several components    #
#                                                         #
###########################################################

message( STATUS "== GLOBAL DEPENDENCY CHECK ==" )

include( ${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake )
include( ${CMAKE_ROOT}/Modules/CheckIncludeFileCXX.cmake )

# Can we use pkg-config?
include(${SPOAC_CMAKE_DIR}/FindPkgConfig.cmake)

# Check for Qt
find_package( Qt4 )
# we do NOT want 4.1.x
if( QTVERSION MATCHES "4.1.*")
    set( QT4_FOUND FALSE )
endif( QTVERSION MATCHES "4.1.*")
if( QT4_FOUND )
    message( STATUS "Looking for Qt4 >= 4.2 - found")
#    message( STATUS "DEBUG:   version: ${QTVERSION}" )
#    message( STATUS "DEBUG:   Core library: ${QT_QTCORE_LIBRARY}" )
#    message( STATUS "DEBUG:   GUI library: ${QT_QTGUI_LIBRARY}" )
#    message( STATUS "DEBUG:   OpenGL library: ${QT_QTOPENGL_LIBRARY}" )
#    message( STATUS "DEBUG:   Includes in ${QT_INCLUDE_DIR}")
#    message( STATUS "DEBUG:   Libraries in ${QT_LIBRARY_DIR}")
#    message( STATUS "DEBUG:   Libraries ${QT_LIBRARIES}" )
else( QT4_FOUND )
    message( STATUS "Looking for Qt4 >= 4.2 - not found")
endif( QT4_FOUND )
# some libraries require v.4.2
# set( QT42_FOUND FALSE )
# if( QTVERSION MATCHES "4.2.*")
#     set( QT42_FOUND TRUE )
#     message( STATUS "Looking for Qt4 >= 4.2 - found")
# endif( QTVERSION MATCHES "4.2.*")

# Check for Java installation
include(${CMAKE_ROOT}/Modules/FindJava.cmake)
# message( STATUS "DEBUG:   Java runtime ${JAVA_RUNTIME}" )
# message( STATUS "DEBUG:   Java archive ${JAVA_ARCHIVE}" )
# message( STATUS "DEBUG:   Java compiler ${JAVA_COMPILE}" )
if( JAVA_RUNTIME )
    set( JAVA_FOUND 1 )
endif( JAVA_RUNTIME )
GBX_ASSERT( JAVA_FOUND "Looking for Java - not found" "Looking for Java - found" )

# Check for Python interpreter installation
include(${CMAKE_ROOT}/Modules/FindPythonInterp.cmake)
GBX_ASSERT( PYTHONINTERP_FOUND "Looking for the Python interpreter - not found" "Looking for the Python interpreter - found" )

