include("utility")
make_skeleton_module(
	MODULE_NAME ImGui
	SOURCES
		"ApplicationBaseImGui.cpp"
		#"Components/ComponentRealtimePlot.cpp"
	DEPS EASTL spdlog glfw imgui implot opengl32)