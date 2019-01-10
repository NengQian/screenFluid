cmake_minimum_required(VERSION 3.0)
project(cg1-ogl-revamp)

# set bin dir
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

# add GLFW and GLM and GLAD
SET(GLFW_BUILD_EXAMPLES OFF)
SET(GLFW_BUILD_TESTS OFF)
SET(GLFW_BUILD_DOCS OFF)
SET(GLFW_INSTALL OFF)
add_subdirectory(glfw)
include_directories(glfw/include)

add_subdirectory(glm)
add_subdirectory(glad-4.4-core)
add_subdirectory(soil2)


# set flags
add_definitions(-std=c++14)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# add sub-progs
include_directories(common)
add_subdirectory(minimal)
add_subdirectory(triangle-1)
add_subdirectory(triangle-2)
add_subdirectory(cube-1)
add_subdirectory(cube-2)
add_subdirectory(geoshader-1)
add_subdirectory(geoshader-2)
add_subdirectory(geoshader-3)
add_subdirectory(geoshader-4)
add_subdirectory(texture-1)
add_subdirectory(texture-2)
add_subdirectory(texture-3)
add_subdirectory(tesselation-1)
add_subdirectory(tesselation-2)
add_subdirectory(tesselation-3)
add_subdirectory(tesselation-4)
add_subdirectory(instancing-1)
add_subdirectory(fbo-1)
add_subdirectory(obj-1)
add_subdirectory(deferred-1)
add_subdirectory(my_particles)
add_subdirectory(particles-1)
add_subdirectory(particles-2)
add_subdirectory(blur-1)
add_subdirectory(shadows-1)
add_subdirectory(shadows-2)
