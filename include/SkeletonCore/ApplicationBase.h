#pragma once

#include "SkeletonDefs.h"
#include "SkeletonCore/Aliases.h"

#include <EASTL/unique_ptr.h>


SKELETON_NAMESPACE_BEGIN(SkeletonCore)

#define SKELETON_APP_INSTANCE(ApplicationClass) \
	static ApplicationClass* ApplicationClass::GetInstance() \
	{ \
		return ApplicationBase::GetInstance<ApplicationClass>(); \
	}

struct ApplicationRawCLIArgs
{
	int argCount = 0;
	const char** args = nullptr;
};

class ApplicationBase
{
public:
	ApplicationBase() = default;
	virtual ~ApplicationBase() = default;

	// Dynamically swapped out at the start of the process
	using CreateInstanceFunction = ApplicationBase* (*)();
	static CreateInstanceFunction CreateInstance;

	static ApplicationBase* GetInstance() { return _instance; }
	template<class T>
	static T* GetInstance() { return dynamic_cast<T*>(_instance); }

	/**
	 * \brief Called from main application entry point
	 */
	static int Main(int argc, const char* argv[]);

	/**
	 * \brief Sets the exit code of the application
	 */
	static void SetExitCode(int exitCode);

protected:
	/**
	 * \brief Called from ApplicationBase::Main
	 *
	 * Do not override in application. Sets member variable _rawCliArgs with argc and argv.
	 */
	virtual int InternalMain(int argc, const char* argv[]);

	/**
	 * \brief Initializes all internal variables (if there are any)
	 *
	 * Do not override in application. Most Skeleton derived classes will override this then call
	 * Initialize when it's ready.
	 */
	virtual void InternalInitialize();

public:
	virtual void Initialize() {}

private:
	/**
	 * \brief Application singleton instance
	 */
	static ApplicationBase* _instance;
	static int _exitCode;
	ApplicationRawCLIArgs _rawCliArgs;
};

SKELETON_NAMESPACE_END()