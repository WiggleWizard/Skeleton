#include "SkeletonCore/ApplicationBase.h"

#include <EASTL/unique_ptr.h>


SKELETON_NAMESPACE_BEGIN(SkeletonCore)

ApplicationBase* ApplicationBase::_instance = nullptr;
ApplicationBase::CreateInstanceFunction ApplicationBase::CreateInstance = nullptr;
int ApplicationBase::_exitCode = 0;

int ApplicationBase::Main(int argc, const char* argv[])
{
	const ea::unique_ptr<ApplicationBase> app(CreateInstance());
	app->_instance = app.get();
	return app->InternalMain(argc, argv);
}

void ApplicationBase::SetExitCode(int exitCode)
{
	_exitCode = exitCode;
}

int ApplicationBase::InternalMain(int argc, const char* argv[])
{
	_rawCliArgs.argCount = argc;
	_rawCliArgs.args = argv;

	InternalInitialize();

	return _exitCode;
}

void ApplicationBase::InternalInitialize()
{
	Initialize();
}

SKELETON_NAMESPACE_END()
