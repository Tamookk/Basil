/*
 * Declares a class for timing how long things take in Basil.
 */
#pragma once

#include <chrono>

namespace Basil
{
	template <typename Fn>
	class Timer
	{
		public:
			Timer(const char* name, Fn&& fn) : func(fn)
			{
				// Initialise variables
				this->name = name;
				stopped = false;
				startTimepoint = std::chrono::high_resolution_clock::now();
			}

			~Timer()
			{
				if (!stopped)
					stop();
			}

			void stop()
			{
				std::chrono::time_point<std::chrono::steady_clock> endTimepoint = std::chrono::high_resolution_clock::now();
				long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint).time_since_epoch().count();
				long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
				stopped = true;
				float duration = (end - start) * 0.001f;
				func({ name, duration });
			}
		private:
			const char* name;
			Fn func;
			std::chrono::time_point<std::chrono::steady_clock> startTimepoint;
			bool stopped;
	};
}