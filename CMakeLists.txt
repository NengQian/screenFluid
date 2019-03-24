cmake_minimum_required(VERSION 3.0)
project(screenFluid)

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
set(CMAKE_BUILD_TYPE "Release")
# add sub-progs
include_directories(common)

# Setting EIGEN3_ROOT here lets us override which Eigen library merely3d chooses
set(EIGEN3_ROOT ${PROJECT_SOURCE_DIR}/extern/eigen_3.3.4)
message("eigen3 root is" ${EIGEN3_ROOT})

# Setting CEREALS_ROOT here
set(CEREALS_ROOT ${PROJECT_SOURCE_DIR}/extern/cereal/include)

set(SOURCE_FILES
        src/math_types.hpp
        src/sim_loader.hpp
        src/sim_loader.cpp
        src/sim_record.hpp
        src/Particle.hpp
)





# prepare target
add_library(my_particlea_lib STATIC ${SOURCE_FILES})
target_include_directories(my_particlea_lib PUBLIC ${EIGEN3_ROOT} ${CEREALS_ROOT}  ${SOURCE_FILES})
target_link_libraries(my_particlea_lib glm soil2 glfw glad ${GLFW_LIBRARIES})


add_executable(screenFluid src/main.cc)
target_link_libraries(screenFluid my_particlea_lib)

