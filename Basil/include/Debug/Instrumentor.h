/*
 * Declares a class for writing profiling data to a JSON file.
 */
#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <thread>

namespace Basil
{
	struct ProfileResult
	{
		std::string name;
		long long start;
		long long end;
		uint32_t threadID;
	};

	struct InstrumentationSession
	{
		std::string name;
	};

	class Instrumentor
	{
		public:
			Instrumentor();
			void beginSession(const std::string& name, const std::string& filePath = "results.json");
			void endSession();
			void writeProfile(const ProfileResult& result);
			void writeHeader();
			void writeFooter();
			static Instrumentor& get();
		private:
			InstrumentationSession* currentSession;
			std::ofstream outputStream;
			int profileCount;
	};

	class InstrumentationTimer
	{
		public:
			InstrumentationTimer(const char* name);
			~InstrumentationTimer();
			void stop();
		private:
			const char* name;
			std::chrono::time_point<std::chrono::high_resolution_clock> startTimepoint;
			bool stopped;
	};

	#define PROFILE 1
	#if PROFILE
		#define PROFILE_BEGIN_SESSION(name, filepath) ::Basil::Instrumentor::get().beginSession(name, filepath)
		#define PROFILE_END_SESSION() ::Basil::Instrumentor::get().endSession()
		#define PROFILE_SCOPE(name) ::Basil::InstrumentationTimer timer##__LINE__(name);
		#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
	#else
		#define PROFILE_BEGIN_SESSION(name, filepath)
		#define PROFILE_END_SESSION()
		#define PROFILE_SCOPE(name)
		#define PROFILE_FUNCTION()
	#endif
}