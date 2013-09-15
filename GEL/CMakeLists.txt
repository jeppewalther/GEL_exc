# add stuff to use MFC in this executable
ADD_DEFINITIONS(-D_AFXDLL)
SET(CMAKE_MFC_FLAG 6)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(GEL)

FILE(GLOB CGLA ./src/CGLA/*.cpp ./src/CGLA/*.c ./src/CGLA/*.h)
FILE(GLOB GLGraphics ./src/GLGraphics/*.cpp ./src/GLGraphics/*.c ./src/GLGraphics/*.h)
FILE(GLOB HMesh ./src/HMesh/*.cpp ./src/HMesh/*.c ./src/HMesh/*.h)
FILE(GLOB Geometry ./src/Geometry/*.cpp ./src/Geometry/*.c ./src/Geometry/*.h)
FILE(GLOB Util ./src/Util/*.cpp ./src/Util/*.c ./src/Util/*.h)
FILE(GLOB LinAlg ./src/LinAlg/*.cpp ./src/LinAlg/*.c ./src/LinAlg/*.h)


source_group(CGLA FILES ${CGLA})
source_group(GLGraphics FILES ${GLGraphics})
source_group(HMesh FILES ${HMesh})
source_group(Geometry FILES ${Geometry})
source_group(Util FILES ${Util})
source_group(LinAlg FILES ${LinAlg})

# Set flags for GNUGCC/C++11 -- Propably Linux
IF(CMAKE_COMPILER_IS_GNUCC)
    message("GCC")
    #Set at module path.
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

    # Make sure we have the libraries
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(GLUT REQUIRED)
    find_package(Lapack REQUIRED)

    # Include directories for dependencies
    include_directories(${GLUT_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS} src)

    ADD_LIBRARY(GEL ${CGLA} ${HMesh} ${GLGraphics} ${Geometry} ${Util} ${LinAlg})

    #Link libs
    TARGET_LINK_LIBRARIES(GEL ${LAPACK_LIBRARIES} ${GLEW_LIBRARY} ${GLUT_LIBRARY} ${OPENGL_LIBRARIES} glut GLU Gl )

    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF()


# Append a d to all debug libraries
SET(CMAKE_DEBUG_POSTFIX "d")

# Now define it so it can be used as a path in test programs
IF(GEL_DATA_DIR)
  ADD_DEFINITIONS(-DGEL_DATA_DIR=${GEL_DATA_DIR})
ENDIF(GEL_DATA_DIR)
IF(NOT LIBRARY_OUTPUT_PATH)
  SET(LIBRARY_OUTPUT_PATH ${GEL_BINARY_DIR}/lib CACHE INTERNAL "Single output directory for building all libraries.")
ENDIF(NOT LIBRARY_OUTPUT_PATH)
IF(NOT EXECUTABLE_OUTPUT_PATH)
  SET(EXECUTABLE_OUTPUT_PATH ${GEL_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
ENDIF(NOT EXECUTABLE_OUTPUT_PATH)
