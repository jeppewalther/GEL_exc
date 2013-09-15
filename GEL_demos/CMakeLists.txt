cmake_minimum_required(VERSION 2.8)
PROJECT(GEL_DEMOS)


################################################################################
# Compile flags
################################################################################
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

################################################################################
# Common
################################################################################
include(${CMAKE_SOURCE_DIR}/cmake/common.cmake)


################################################################################
# Paths and Directories
################################################################################
# Custom CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)
set(GEL_PATH  ${CMAKE_SOURCE_DIR}/../GEL)
set(GEL_LIBRARY_PATH  ${GEL_PATH}/build/lib)
set(GEL_INCLUDE_DIRS ${GEL_PATH}/src)


LINK_DIRECTORIES(${GEL_LIBRARY_PATH})

################################################################################
# Packages
################################################################################
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Lapack REQUIRED)

################################################################################
# Further Parsing
################################################################################
add_subdirectory(${CMAKE_SOURCE_DIR}/Edge_Flipper)
add_subdirectory(${CMAKE_SOURCE_DIR}/Mesh_Viewer)
add_subdirectory(${CMAKE_SOURCE_DIR}/Surface_Reconstruction)


