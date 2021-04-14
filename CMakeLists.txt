cmake_minimum_required(VERSION 3.17)
project(render_dev)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(OpenGL_renderer)

add_executable(render_dev main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC renderer)