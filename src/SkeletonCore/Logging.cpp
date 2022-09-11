#include "SkeletonCore/Logging.h"

#ifdef _WIN32
	extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
#endif


template<typename Factory>
std::shared_ptr<spdlog::logger> stdout_color_mt_dbg_attach(const std::string& logger_name, spdlog::color_mode mode)
{
	std::shared_ptr<spdlog::logger> newLogger = Factory::template create<spdlog::sinks::stdout_color_sink_mt>(logger_name, mode);

#ifdef _WIN32
	if(IsDebuggerPresent())
		newLogger->sinks().push_back(std::make_shared<spdlog::sinks::msvc_sink_mt>());
#endif

	return newLogger;
}

template std::shared_ptr<spdlog::logger> stdout_color_mt_dbg_attach<spdlog::synchronous_factory>(const std::string& logger_name, spdlog::color_mode mode);