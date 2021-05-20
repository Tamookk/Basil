/*
 * This header file declares a class for a basic timer.
 */
#pragma once

#include <chrono>

namespace Basil
{
	class Timer
	{
		public:
			Timer();
			void reset();
			float elapsed();
			float elapsedMillis();
		private:
			std::chrono::time_point<std::chrono::high_resolution_clock> start;
	};
}