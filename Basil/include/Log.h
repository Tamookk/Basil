/*
 * This class is for game engine and client logging
 */
#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Basil
{
	class BASIL_API Log
	{
		public:
			static void init();
			inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return clientLogger; }
			inline static std::shared_ptr<spdlog::logger>& getEngineLogger() { return engineLogger; }
		private:
			static std::shared_ptr<spdlog::logger> clientLogger;
			static std::shared_ptr<spdlog::logger> engineLogger;
	};
}

// Define client logging macros
#define LOG_CLIENT_TRACE(...)	Basil::Log::getClientLogger()->trace(__VA_ARGS__);
#define LOG_CLIENT_INFO(...)	Basil::Log::getClientLogger()->info(__VA_ARGS__);
#define LOG_CLIENT_WARN(...)	Basil::Log::getClientLogger()->warn(__VA_ARGS__);
#define LOG_CLIENT_ERROR(...)	Basil::Log::getClientLogger()->error(__VA_ARGS__);
#define LOG_CLIENT_FATAL(...)	Basil::Log::getClientLogger()->critical(__VA_ARGS__);

// Define engine logging macros
#define LOG_TRACE(...)			Basil::Log::getEngineLogger()->trace(__VA_ARGS__);
#define LOG_INFO(...)			Basil::Log::getEngineLogger()->info(__VA_ARGS__);
#define LOG_WARN(...)			Basil::Log::getEngineLogger()->warn(__VA_ARGS__);
#define LOG_ERROR(...)			Basil::Log::getEngineLogger()->error(__VA_ARGS__);
#define LOG_FATAL(...)			Basil::Log::getEngineLogger()->critical(__VA_ARGS__);

