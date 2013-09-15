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
    meshViewver
        ${CPPS}
        ${HPPS})


##############################################################################
# Include Directories
##############################################################################
include_directories(
        ${OPENGL_INCLUDE_DIRS}
        ${GLUT_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR}
        ${GEL_INCLUDE_DIRS}
)


##############################################################################
# Link Libraries
##############################################################################
target_link_libraries(
    meshViewver
        ${GEL_LIBRARY_PATH}/libGEL.a
        ${GLEW_LIBRARY}
        ${GLUT_LIBRARY}
        ${OPENGL_LIBRARIES})


