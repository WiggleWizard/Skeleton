#include "SkeletonImGui/ApplicationBaseImGui.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// For hiding and showing console window
// TODO: Wrap with windows directive
#include <Windows.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#include "SkeletonCore/Logging.h"
LOGGER(logger, ApplicationBaseImGui)


SKELETON_NAMESPACE_BEGIN(SkeletonImGui)

static void GLFWErrorCallback(int error, const char* description)
{
	//fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void ApplicationBaseImGui::InternalInitialize()
{
	SetupWindow();
	InitializeImGui();

	Super::InternalInitialize();

	MainLoop();
}

void ApplicationBaseImGui::SetupWindow()
{
	// Setup window
	glfwSetErrorCallback(GLFWErrorCallback);
	if(!glfwInit())
	{
		return;
	}

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	_glslVersion = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	_glslVersion = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	_glslVersion = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	_glfwWindow = glfwCreateWindow(1280, 720, GetWindowTitle().c_str(), NULL, NULL);
	if(_glfwWindow == nullptr)
	{
		return;
	}

	glfwMakeContextCurrent(_glfwWindow);
	glfwSwapInterval(1); // Enable vsync
}

void ApplicationBaseImGui::DrawImGuiBegin()
{
	// Poll and handle events (inputs, window resize, etc.)
	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	glfwPollEvents();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ApplicationBaseImGui::MainLoop()
{
	bool bWindowWantsToClose = false;
	while(!_bShouldQuitMainLoop && !bWindowWantsToClose)
	{
		DrawImGuiBegin();

		// TODO: Delta time
		Tick(0);
		InternalDrawImGui(0);
		DrawImGuiEnd();

		bWindowWantsToClose = glfwWindowShouldClose(_glfwWindow);
	}

	MainLoopExit();
}

void ApplicationBaseImGui::InternalDrawImGui(double deltaTime)
{
	if(_bShowDemoWindow)
		ImGui::ShowDemoWindow(&_bShowDemoWindow);

	DrawImGui(deltaTime);
}

void ApplicationBaseImGui::DrawImGuiEnd()
{
	// Rendering
	ImGui::Render();
	int displayWidth, displayHeight = 0;
	glfwGetFramebufferSize(_glfwWindow, &displayWidth, &displayHeight);
	glViewport(0, 0, displayWidth, displayHeight);
	glClearColor(_clearColor.x * _clearColor.w, _clearColor.y * _clearColor.w, _clearColor.z * _clearColor.w, _clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(_glfwWindow);
}

void ApplicationBaseImGui::MainLoopExit()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(_glfwWindow);
	glfwTerminate();
}

void ApplicationBaseImGui::InitializeImGui()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Allow children to modify IO
	ModifyImGuiIo(io);

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(_glfwWindow, true);
	ImGui_ImplOpenGL3_Init(_glslVersion.c_str());
}

SKELETON_NAMESPACE_END()
