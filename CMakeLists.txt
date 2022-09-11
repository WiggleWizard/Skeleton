cmake_minimum_required(VERSION 3.22)

project(Skeleton)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake;${PROJECT_SOURCE_DIR}/thirdparty")
set(SKELETON_SOURCE_DIR ${PROJECT_SOURCE_DIR})

add_subdirectory(thirdparty)
add_subdirectory(src)