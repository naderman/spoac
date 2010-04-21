#
# Installation directory is determined by looking at 3 sources of information in the following order,
# later sources overwrite earlier ones:
# 1. OS-dependent defaults (effective only the first time CMake runs or after CMakeCache is deleted)
# 2. Enviroment variable whose name is determined as <PROJECT_NAME>_INSTALL
# 3. CMake variable whose name is determined as <PROJECT_NAME>_install(same as the environment variable)
#
# E.g. in Linux, for a project called 'fruitcake'
# $ rm CMakeCache.txt; cmake .
#       /usr/local/
# $ export FRUITCAKE_INSTALL=/home/myname/install; cmake .
#       /home/myname/install
# $ export FRUITCAKE_INSTALL=/home/myname/install; cmake -DFRUITCAKE_INSTALL=/home/myname/opt .
#       /home/myname/opt
#
# Afterwards, it's ok to just use "cmake .", the previously set installation dir is held in cache.
#
# A manually set installation dir (e.i. with ccmake) is not touched until an environment variable or
# a command line variable is introduced.

#
# IS_SUPER_PROJECT is a flag which is defined if Gearbox is built as part of CMake "ueber-project".
# If it's set don't overwrite CMAKE_INSTALL_PREFIX because the ueber-project has already set it.
#

# special installation directories
set( GBX_BIN_INSTALL_SUFFIX bin )
set( GBX_INCLUDE_INSTALL_SUFFIX include/${PROJECT_NAME} )
set( GBX_SHARE_INSTALL_SUFFIX share/${PROJECT_NAME} )
set( GBX_CMAKE_INSTALL_SUFFIX ${GBX_SHARE_INSTALL_SUFFIX}/cmake )

if( GBX_PROC_64BIT )
    set( GBX_LIB_INSTALL_SUFFIX lib64/${PROJECT_NAME} )
    set( GBX_PKGCONFIG_INSTALL_SUFFIX lib64/pkgconfig )
else()
    set( GBX_LIB_INSTALL_SUFFIX lib/${PROJECT_NAME} )
    set( GBX_PKGCONFIG_INSTALL_SUFFIX lib/pkgconfig )
endif()

# by convention, we install cmake package-config files with the libraries
set( GBX_CMAKE_PKGCONFIG_INSTALL_SUFFIX ${GBX_LIB_INSTALL_SUFFIX} )

# now the acutal install directories
set( GBX_BIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${GBX_BIN_INSTALL_SUFFIX} )
set( GBX_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${GBX_INCLUDE_INSTALL_SUFFIX} )
set( GBX_SHARE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${GBX_SHARE_INSTALL_SUFFIX} )
set( GBX_CMAKE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${GBX_CMAKE_INSTALL_SUFFIX} )
set( GBX_CMAKE_PKGCONFIG_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${GBX_CMAKE_PKGCONFIG_INSTALL_SUFFIX} )
set( GBX_LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${GBX_LIB_INSTALL_SUFFIX} )
set( GBX_PKGCONFIG_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${GBX_PKGCONFIG_INSTALL_SUFFIX} )

message (STATUS ">>>>>>>>>>>>>>>>>>>>>>>>> ${CMAKE_INSTALL_PREFIX}")
#
# It's sometimes useful to refer to the top level of the project.
# CMake provides the right variables but, as all variables, they are poorly documented.
#
set( GBX_PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR} )
set( GBX_PROJECT_BINARY_DIR ${PROJECT_BINARY_DIR} )
