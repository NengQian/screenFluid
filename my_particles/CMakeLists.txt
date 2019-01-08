cmake_minimum_required(VERSION 3.0)
project(cg1-my_particles)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Setting EIGEN3_ROOT here lets us override which Eigen library merely3d chooses
set(EIGEN3_ROOT ${PROJECT_SOURCE_DIR}/extern/eigen_3.3.4)

# Setting CEREALS_ROOT here
set(CEREALS_ROOT ${PROJECT_SOURCE_DIR}/extern/cereal/include)

set(SOURCE_FILES
        math_types.hpp
        sim_loader.hpp
        sim_loader.cpp
        sim_record.hpp
        Particle.hpp
)

# prepare target
add_library(my_particlea_lib STATIC ${SOURCE_FILES})

add_executable(cg1-my_particles main.cc)
target_link_libraries(my_particlea_lib glm soil2 glfw glad ${GLFW_LIBRARIES})
target_include_directories(my_particlea_lib SYSTEM PUBLIC ${EIGEN3_ROOT} ${CEREALS_ROOT}  ${SOURCE_FILES})

target_link_libraries(cg1-my_particles my_particlea_lib)
