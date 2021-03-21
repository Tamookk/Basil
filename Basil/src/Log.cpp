#include "pch.h"
#include "Log.h"

namespace Basil
{
	std::shared_ptr<spdlog::logger> Log::clientLogger;
	std::shared_ptr<spdlog::logger> Log::engineLogger;

	// Initialisation function
	void Log::init()
	{
		// Set pattern for logging
		spdlog::set_pattern("%^[%T] %n - %v%$");

		// Create loggers
		clientLogger = spdlog::stderr_color_mt("CLIENT");
		engineLogger = spdlog::stdout_color_mt("ENGINE");

		// Set logging level
		clientLogger->set_level(spdlog::level::trace);
		engineLogger->set_level(spdlog::level::trace);
	}
}