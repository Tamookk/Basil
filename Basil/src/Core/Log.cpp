#include "pch.h"
#include "Core/Log.h"

namespace Basil
{
	Shared<spdlog::logger> Log::engineLogger;
	Shared<spdlog::logger> Log::clientLogger;

	// Initialisation function
	void Log::init()
	{
		// Set up log sinks
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(makeShared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(makeShared<spdlog::sinks::basic_file_sink_mt>("Basil.log", true));
		logSinks[0]->set_pattern("%^[%T] %n - %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n - %v");

		// Create loggers
		engineLogger = makeShared<spdlog::logger>("ENGINE", begin(logSinks), end(logSinks));
		spdlog::register_logger(engineLogger);
		engineLogger->set_level(spdlog::level::trace);
		engineLogger->flush_on(spdlog::level::trace);

		clientLogger = makeShared<spdlog::logger>("CLIENT", begin(logSinks), end(logSinks));
		spdlog::register_logger(clientLogger);
		clientLogger->set_level(spdlog::level::trace);
		clientLogger->flush_on(spdlog::level::trace);
	}
}