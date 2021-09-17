/*
 * This class is for game engine and client logging.
 */
#pragma once

#include "Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Basil
{
	class Log
	{
		public:
			static void init();
			inline static Shared<spdlog::logger>& getClientLogger() { return clientLogger; }
			inline static Shared<spdlog::logger>& getEngineLogger() { return engineLogger; }
		private:
			static Shared<spdlog::logger> clientLogger;
			static Shared<spdlog::logger> engineLogger;
	};
}

// Allow logging of GLM vectors, matrices, and quaternions
template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::qua<T, Q>& quaternion)
{
	return os << glm::to_string(quaternion);
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

