cmake_minimum_required(VERSION 3.0)
project(cg1-tess3)

# prepare target
add_executable(cg1-tess3 main.cc)
target_link_libraries(cg1-tess3 glm soil2 glfw glad ${GLFW_LIBRARIES})
