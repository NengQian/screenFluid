cmake_minimum_required(VERSION 3.0)
project(cg1-ogl-minimal)

# prepare target
add_executable(cg1-min main.cc)
target_link_libraries(cg1-min glm soil2 glfw glad ${GLFW_LIBRARIES})
