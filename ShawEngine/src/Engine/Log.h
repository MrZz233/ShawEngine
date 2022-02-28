#pragma once
#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace ShawEngine {
	class SE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
			return _CoreLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
			return _ClientLogger;
		}

	private:
		static std::shared_ptr<spdlog::logger> _CoreLogger;
		static std::shared_ptr<spdlog::logger> _ClientLogger;

	};
}

#ifdef SE_DEBUG
	#define SE_CORE_TRACE(...)		::ShawEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define SE_CORE_INFO(...)		::ShawEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define SE_CORE_WARN(...)		::ShawEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define SE_CORE_ERROR(...)		::ShawEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define SE_CORE_FATAL(...)		::ShawEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)

	#define SE_CLIENT_TRACE(...)	::ShawEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define SE_CLIENT_INFO(...)		::ShawEngine::Log::GetClientLogger()->info(__VA_ARGS__)
	#define SE_CLIENT_WARN(...)		::ShawEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define SE_CLIENT_ERROR(...)	::ShawEngine::Log::GetClientLogger()->error(__VA_ARGS__)
	#define SE_CLIENT_FATAL(...)	::ShawEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)
#else
	#define SE_CORE_TRACE
	#define SE_CORE_INFO
	#define SE_CORE_WARN
	#define SE_CORE_ERROR
	#define SE_CORE_FATAL

	#define SE_CLIENT_TRACE
	#define SE_CLIENT_INFO
	#define SE_CLIENT_WARN
	#define SE_CLIENT_ERROR
	#define SE_CLIENT_FATAL
#endif