#
# Based on Orca Robotics cmake file.
# http://orca-robotics.sourceforge.net
#
include(cmake/Assert.cmake)

set(SPOAC_CMAKE_DIR ${${PROJECT_NAME}_SOURCE_DIR}/cmake)
message(STATUS "Running CMake scripts in ${SPOAC_CMAKE_DIR}")

message(STATUS "Project name ${PROJECT_NAME}")
message(STATUS "Project version ${SPOAC_PROJECT_VERSION}")

# help with $ cmake --help-policy CMPxxxx
if (COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0011 NEW)
endif (COMMAND cmake_policy)

include(cmake/SetupOs.cmake)
include(cmake/SetupDirectories.cmake)
include(cmake/TargetUtils.cmake)

# Dependency 1: ZeroC Ice
include(${SPOAC_CMAKE_DIR}/FindIce.cmake)
GBX_ASSERT(ICE_FOUND
    "Looking for Ice - not found. Please install Ice, ** delete CMakeCache.txt **, then re-run CMake."
    "Looking for Ice - found in ${ICE_HOME}"
    1
)

# Test Ice installation
include(${SPOAC_CMAKE_DIR}/TestIce.cmake)
GBX_ASSERT(ICE_WORKS
    "Testing Ice - failed. Please check or reinstall it, ** delete CMakeCache.txt **, then re-run CMake."
    "Testing Ice - ok."
    1
)

set(ICE_BIN_DIR ${ICE_HOME}/bin)
set(ICE_LIB_DIR ${ICE_HOME}/lib)
set(ICE_INCLUDE_DIR ${ICE_HOME}/include)

# Defaults for big source code switches
# (these are defaults. after the user modifies these in ccmake, the values stay in cache)
option(SPOAC_BUILD_SERVICES "Enables compilation of all IceBox services" ON)
option(SPOAC_BUILD_JAVA     "Enables compilation of all Java interfaces and components" ON)
option(SPOAC_BUILD_PYTHON   "Enables compilation of all Python interfaces and components" ON)
option(SPOAC_BUILD_TESTS    "Enables compilation of all tests" ON)
option(SPOAC_BUILD_EXAMPLES "Enables compilation of all examples" ON)
option(SPOAC_BUILD_XML      "Enables generation of XML file for IceGrid" ON)

include(${SPOAC_CMAKE_DIR}/CheckCommonDependencies.cmake)

#
# Installation preferences
# see: \http://www.cmake.org/Wiki/CMake_RPATH_handling
#
# CMake defaults

# use, i.e. don't skip the full RPATH for the build tree
# set(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
# set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

# the RPATH to be used when installing
set(CMAKE_INSTALL_RPATH
        ${ICE_LIB_DIR}
        ${GBX_LINK_DIR}        # this is where satellite projects found Orca
        ${GBX_LIB_INSTALL_DIR}  # this is where Orca project will install libs
)
# message( STATUS CMAKE_INSTALL_RPATH=${CMAKE_INSTALL_RPATH} )

add_subdirectory(cmake)

add_subdirectory(src)
add_subdirectory(scripts)

message(STATUS "Ice version: ${ICE_VERSION}")
