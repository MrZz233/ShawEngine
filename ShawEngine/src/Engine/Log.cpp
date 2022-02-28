#include "sepch.h"
#include "Log.h"

namespace ShawEngine {
	std::shared_ptr<spdlog::logger> Log::_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::_ClientLogger;
	
	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		_CoreLogger = spdlog::stdout_color_mt("ShawEngine");
		_CoreLogger->set_level(spdlog::level::trace);
		_ClientLogger = spdlog::stdout_color_mt("Client");
		_ClientLogger->set_level(spdlog::level::trace);
	}
}

