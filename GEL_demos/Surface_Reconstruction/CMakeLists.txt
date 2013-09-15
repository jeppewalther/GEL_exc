################################################################################
# Paths and Directories
################################################################################
# Generate runtime output in platform specific subfolders
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PLATFORM})

set(APPLICATION_HPP_DIR ".")
set(APPLICATION_CPP_DIR ".")

file(GLOB_RECURSE APPLICATION_HPPS ${APPLICATION_HPP_DIR}/*.h ${APPLICATION_HPP_DIR}/*.hpp)
file(GLOB_RECURSE APPLICATION_CPPS ${APPLICATION_CPP_DIR}/*.c ${APPLICATION_CPP_DIR}/*.cpp)

list(APPEND HPPS ${APPLICATION_HPPS})
list(APPEND CPPS ${APPLICATION_CPPS})



##############################################################################
# Add Executable
##############################################################################
add_executable(
    surfaceReconstruction
        ${CPPS}
        ${HPPS})

LINK_DIRECTORIES(${GEL_LIBRARY_PATH})

##############################################################################
# Include Directories
##############################################################################
include_directories(
        ${GEL_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIR}
        ${GEL_INCLUDE_DIRS}
)


##############################################################################
# Link Libraries
##############################################################################
target_link_libraries(
    surfaceReconstruction
        ${GEL_LIBRARY_PATH}/libGEL.a
        ${LAPACK_LIBRARIES}
        ${GLEW_LIBRARY}
        ${GLUT_LIBRARY}
        ${OPENGL_LIBRARIES})

