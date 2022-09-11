cmake_minimum_required(VERSION 3.12.4)

project(Skeleton)

option(SKELETON_BUILD_SHARED "Build shared library" OFF)

set(SKELETON_SOURCES "")

# Modules
add_subdirectory(SkeletonCore)
add_subdirectory(SkeletonOS)
add_subdirectory(SkeletonImGui)

#add_library(Skeleton)
#target_link_libraries(Skeleton PRIVATE EASTL)
#target_sources(Skeleton PRIVATE "SkeletonCore/EASTLMem.cpp")

# TODO: SKELETON_BUILD_SHARED
if(SKELETON_BUILD_SHARED)
else()
	#add_library(Skeleton STATIC )
endif()