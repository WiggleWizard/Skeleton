function(make_skeleton_module)
	cmake_parse_arguments(
		PARSED_ARGS
		""
		"MODULE_NAME"
		"SOURCES;DEPS"
		${ARGN}
	)

	set(MODULE_TYPE "LIB")
	set(IS_HEADER_ONLY FALSE)
	if(NOT PARSED_ARGS_SOURCES)
		set(IS_HEADER_ONLY TRUE)
		set(MODULE_TYPE "INTERFACE")
	endif()
	
	message("-- Skeleton module ${PARSED_ARGS_MODULE_NAME} (${MODULE_TYPE}):")
	message("   - Deps: ${PARSED_ARGS_DEPS}")

	set(MODULE_NAME SkeletonModule${PARSED_ARGS_MODULE_NAME})
	set(MODULE_ALIAS Skeleton::Module::${PARSED_ARGS_MODULE_NAME})
	set(MODULE_SOURCES ${PARSED_ARGS_SOURCES})
	set(MODULE_DEP_LIBS ${PARSED_ARGS_DEPS})

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