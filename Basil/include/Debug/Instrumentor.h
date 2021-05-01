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
		std::thread::id threadID;
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
			static Instrumentor& get();
		private:
			void writeHeader();
			void writeFooter();
			void internalEndSession();
			InstrumentationSession* currentSession;
			std::ofstream outputStream;
			int profileCount;
			std::mutex mutex;
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

	namespace InstrumentorUtils
	{
		template <size_t N>
		struct ChangeResult
		{
			char data[N];
		};

		template <size_t N, size_t K>
		constexpr auto cleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}

	#define PROFILE 0
	#if PROFILE
		#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
			#define FUNC_SIG __PRETTY_FUNCTION__
		#elif defined(__DMC__) && (__DMC__ >= 0x810)
			#define FUNC_SIG __PRETTY_FUNCTION__
		#elif (defined(__FUNCSIG__) || (_MSC_VER))
			#define FUNC_SIG __FUNCSIG__
		#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
			#define FUNC_SIG __FUNCTION__
		#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
			#define FUNC_SIG __FUNC__
		#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
			#define FUNC_SIG __func__
		#elif defined(__cplusplus) && (__cplusplus >= 201103)
			#define FUNC_SIG __func__
		#else
			#define FUNC_SIG "FUN_SIG unknown!"
		#endif
		
		#define PROFILE_BEGIN_SESSION(name, filepath) ::Basil::Instrumentor::get().beginSession(name, filepath)
		#define PROFILE_END_SESSION() ::Basil::Instrumentor::get().endSession()
		#define PROFILE_SCOPE(name) constexpr auto fixedName = ::Basil::InstrumentorUtils::cleanupOutputString(name, "__cdecl"); ::Basil::InstrumentationTimer timer##__LINE__(fixedName.data)
		#define PROFILE_FUNCTION() PROFILE_SCOPE(FUNC_SIG)
	#else
		#define PROFILE_BEGIN_SESSION(name, filepath)
		#define PROFILE_END_SESSION()
		#define PROFILE_SCOPE(name)
		#define PROFILE_FUNCTION()
	#endif
}