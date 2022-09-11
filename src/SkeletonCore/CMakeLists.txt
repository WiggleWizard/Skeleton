include("utility")
make_skeleton_module(
	MODULE_NAME Core
	SOURCES
		"ApplicationBase.cpp"
		"ApplicationBaseGui.cpp"

		"EASTLMem.cpp"
		"Logging.cpp"
	DEPS EASTL spdlog)