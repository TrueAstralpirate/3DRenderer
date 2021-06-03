cmake_minimum_required(VERSION 3.7)

project(renderer)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories("sources/ExternalLibraries/eigen/Eigen")

set(SOURCES sources/run.cpp sources/camera.cpp sources/screen.cpp sources/renderer.cpp sources/buffer.h sources/object.cpp sources/world.cpp sources/objectParser.h sources/userControl.cpp sources/application.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(run ${SOURCES})
target_link_libraries(run ${SDL2_LIBRARIES})
