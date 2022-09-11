#pragma once

#include "ApplicationBase.h"

// TODO: Wrap in windows directives
#include <Windows.h>

#define SKELETON_CORE_NAMESPACE SkeletonCore
#define SKELETON_APP_BASE_CLASSNAME SKELETON_CORE_NAMESPACE::ApplicationBase

#define SKELETON_CREATE_APPLICATION_DEFINE(ApplicationClass) \
	SKELETON_APP_BASE_CLASSNAME* CreateApplication() { return new ApplicationClass(); }

/**	Initializes Skeleton's application base.

	Basic "bootstraping" line to get booted into Skeleton as soon as the application can do so.
 */
#define START_SKELETON_APPLICATION(ApplicationClass) \
SKELETON_CREATE_APPLICATION_DEFINE(ApplicationClass) \
int main(int argc, const char* argv[]) \
{ \
	SKELETON_APP_BASE_CLASSNAME::CreateInstance = &CreateApplication; \
	return SKELETON_APP_BASE_CLASSNAME::Main(argc, argv); \
}

/**
 * \brief Bootstrapping macro to help launch into a GUI application (without any console attached)
 */
#define START_SKELETON_GUI_APPLICATION(ApplicationClass) \
START_SKELETON_APPLICATION(ApplicationClass) \
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) \
{ \
	main(__argc, (const char**)__argv); \
}