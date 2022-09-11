#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
// For MSVC output logging
#include <spdlog/sinks/msvc_sink.h>

#include <EASTL/string.h>

#include <memory>


// Forward decl a spdlg sink that allows both stdout + debugger outputs
template<typename Factory = spdlog::synchronous_factory>
std::shared_ptr<spdlog::logger> stdout_color_mt_dbg_attach(const std::string& logger_name, spdlog::color_mode mode = spdlog::color_mode::automatic);

#define LOG_DECL(variableName) \
	std::shared_ptr<spdlog::logger> variableName;
#define LOG_IMPL(c, variableName, category) \
	std::shared_ptr<spdlog::logger> c::variableName = stdout_color_mt_dbg_attach(#category);
#define LOGGER(variableName, category) \
	static std::shared_ptr<spdlog::logger> variableName = stdout_color_mt_dbg_attach(#category);

// Decl with minimal impl for fmt to format raw eastl::string
template<>
struct fmt::formatter<eastl::string>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx);

    template<typename FormatContext>
    auto format(const eastl::string& number, FormatContext& ctx);
};

template<typename ParseContext>
constexpr auto fmt::formatter<eastl::string>::parse(ParseContext& ctx)
{
    return ctx.begin();
}

template<typename FormatContext>
auto fmt::formatter<eastl::string>::format(const eastl::string& s, FormatContext& ctx)
{
    return fmt::format_to(ctx.out(), "{}", s.c_str());
}