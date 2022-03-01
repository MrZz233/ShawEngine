#include "sepch.h"
#include "Log.h"

namespace ShawEngine {
	std::shared_ptr<spdlog::logger> Log::m_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::m_ClientLogger;
	
	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_CoreLogger = spdlog::stdout_color_mt("ShawEngine");
		m_CoreLogger->set_level(spdlog::level::trace);
		m_ClientLogger = spdlog::stdout_color_mt("Client");
		m_ClientLogger->set_level(spdlog::level::trace);
	}
}

