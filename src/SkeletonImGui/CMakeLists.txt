include("utility")
make_skeleton_module(
	MODULE_NAME ImGui
	SOURCES "ApplicationBaseImGui.cpp"
	DEPS EASTL spdlog glfw imgui opengl32)