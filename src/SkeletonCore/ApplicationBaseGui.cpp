#include "SkeletonCore/ApplicationBaseGui.h"

#include <Windows.h>


SKELETON_NAMESPACE_BEGIN(SkeletonCore)

void ApplicationBaseGui::ShowConsole()
{
	// Create a console if one doesn't already exist
	if(!IsWindow(GetConsoleWindow()))
	{
		AllocConsole();
	}

	ShowWindow(GetConsoleWindow(), SW_SHOW);
}

void ApplicationBaseGui::HideConsole()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

SKELETON_NAMESPACE_END()