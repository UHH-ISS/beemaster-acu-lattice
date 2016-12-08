cmake_minimum_required(VERSION 3.6)

# Project name (!= executable name!)
project(acu-impl)

# CMake and compiler flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")

# Add ./cmake to CMAKE_MODULE_PATH to 'source' it
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Link against librdb
#
# Find the library
#find_package(rocksdb REQUIRED)
# Add the headers to include directories
#include_directories(BEFORE ${RDB_INCLUDE_DIR})
# Add the library to the LINK_LIBS
#set(LINK_LIBS ${LINK_LIBS} ${RDB_LIBRARY})

# Link against libbroker
#
# Find the library
find_package(Broker REQUIRED)
# Add the headers to include directories
include_directories(BEFORE ${BROKER_INCLUDE_DIR})
# Add the library to the LINK_LIBS
set(LINK_LIBS ${LINK_LIBS} ${BROKER_LIBRARY})

# Link against libacu
#
# Find the library
find_package(Acu REQUIRED)
# Add the headers to include directories
include_directories(BEFORE ${ACU_INCLUDE_DIR})
# Add the library to the LINK_LIBS
set(LINK_LIBS ${LINK_LIBS} ${ACU_LIBRARY})

# Add our include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add sub directories
add_subdirectory(src)
add_subdirectory(test)
