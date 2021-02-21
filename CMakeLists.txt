cmake_minimum_required(VERSION 3.7)

project(renderer)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories("sources/ExternalLibraries/eigen/Eigen")

set(SOURCES sources/main.cpp sources/camera.cpp sources/screen.cpp sources/renderer.cpp sources/buffer.h)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(main ${SOURCES})
target_link_libraries(main ${SDL2_LIBRARIES})