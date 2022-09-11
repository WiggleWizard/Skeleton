#include "SkeletonCore/ApplicationBaseGui.h"

#include <EASTL/string.h>
#include <imgui.h>


struct GLFWwindow;

SKELETON_NAMESPACE_BEGIN(SkeletonImGui)

class ApplicationBaseImGui : public SkCore::ApplicationBaseGui
{
	using Super = SkCore::ApplicationBase;

public:
	/**
	 * \brief Override to change main window's title
	 * \return Reference to a string
	 */
	virtual ea::string& GetWindowTitle()
	{
		static ea::string title("");
		return title;
	}

	// Override these
	virtual void ModifyImGuiIo(ImGuiIO& io) {};
	virtual void Tick(double deltaTime) {}
	virtual void DrawImGui(double deltaTime) {}

protected:
	void InternalInitialize() final;
	void SetupWindow();

	void InitializeImGui();
	void DrawImGuiBegin();
	virtual void MainLoop();
	void InternalDrawImGui(double deltaTime);
	void DrawImGuiEnd();

	void MainLoopExit();

private:
	GLFWwindow* _glfwWindow = nullptr;
	ea::string _glslVersion = "#version 130";
	bool _bShowDemoWindow = false;
	ImVec4 _clearColor = { 0.1f, 0.1f, 0.1f, 1.f };
	bool _bShouldQuitMainLoop = false;
};

SKELETON_NAMESPACE_END()