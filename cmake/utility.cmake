function(get_dep)
	cmake_parse_arguments(
		IN_ARGS
		""
		"DEP_NAME"
		""
		${ARGN})

	# Attempt to find an include for this dependency, if we don't find one however,
	# that's ok, we can move on to doing other stuff
	include("${IN_ARGS_DEP_NAME}" OPTIONAL RESULT_VARIABLE _MODULE_PATH)                                                                                                            
	if(NOT _MODULE_PATH)
		message(AUTHOR_WARNING "No cmake dependency include found for ${IN_ARGS_DEP_NAME}. Ignoring")
	endif()
endfunction()

function(make_skeleton_module)
	cmake_parse_arguments(
		IN_ARGS
		""
		"MODULE_NAME"
		"SOURCES;HEADERS;DEPS"
		${ARGN})

	set(MODULE_TYPE "LIB")
	set(IS_HEADER_ONLY FALSE)
	if(NOT IN_ARGS_SOURCES)
		set(IS_HEADER_ONLY TRUE)
		set(MODULE_TYPE "HEADER ONLY")
	endif()
	
	# Printing some info about each module
	message(STATUS "Skeleton module ${IN_ARGS_MODULE_NAME} (${MODULE_TYPE})")
	message("   - Dependancies:")
	foreach(DEP ${IN_ARGS_DEPS})
		message("         ${DEP}")
	endforeach()

	foreach(DEP ${IN_ARGS_DEPS})
		get_dep(DEP_NAME ${DEP})
	endforeach()

	set(MODULE_NAME SkeletonModule${IN_ARGS_MODULE_NAME})
	set(MODULE_ALIAS Skeleton::Module::${IN_ARGS_MODULE_NAME})
	set(MODULE_SOURCES ${IN_ARGS_SOURCES} ${IN_ARGS_HEADERS})
	set(MODULE_DEP_LIBS ${IN_ARGS_DEPS})

	project(${MODULE_NAME})

	if(NOT IS_HEADER_ONLY)
		add_library(${MODULE_NAME})

		target_sources(${MODULE_NAME} PRIVATE ${MODULE_SOURCES})
		target_include_directories(${MODULE_NAME} PUBLIC ${SKELETON_SOURCE_DIR}/include PRIVATE ${SKELETON_SOURCE_DIR}/include)
		target_link_libraries(${MODULE_NAME} PUBLIC ${MODULE_DEP_LIBS})
	else()
		add_library(${MODULE_NAME} INTERFACE)

		target_include_directories(${MODULE_NAME} INTERFACE ${SKELETON_SOURCE_DIR}/include)
		target_link_libraries(${MODULE_NAME} INTERFACE ${MODULE_DEP_LIBS})
	endif()

	add_library(${MODULE_ALIAS} ALIAS ${MODULE_NAME})
endfunction()