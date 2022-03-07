#pragma once

#include "Engine/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace ShawEngine {
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;

	};
}

#ifdef SE_DEBUG
	#define SE_CORE_TRACE(...)		::ShawEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define SE_CORE_INFO(...)		::ShawEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define SE_CORE_WARN(...)		::ShawEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define SE_CORE_ERROR(...)		::ShawEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define SE_CORE_CRITICAL(...)		::ShawEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

	#define SE_CLIENT_TRACE(...)	::ShawEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define SE_CLIENT_INFO(...)		::ShawEngine::Log::GetClientLogger()->info(__VA_ARGS__)
	#define SE_CLIENT_WARN(...)		::ShawEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define SE_CLIENT_ERROR(...)	::ShawEngine::Log::GetClientLogger()->error(__VA_ARGS__)
	#define SE_CLIENT_CRITICAL(...)	::ShawEngine::Log::GetClientLogger()->critical(__VA_ARGS__)

#else
	#define SE_CORE_TRACE
	#define SE_CORE_INFO
	#define SE_CORE_WARN
	#define SE_CORE_ERROR
	#define SE_CORE_CRITICAL
	#define SE_CORE_ASSERT

	#define SE_CLIENT_TRACE
	#define SE_CLIENT_INFO
	#define SE_CLIENT_WARN
	#define SE_CLIENT_ERROR
	#define SE_CLIENT_CRITICAL
	#define SE_CLIENT_ASSERT

#endif