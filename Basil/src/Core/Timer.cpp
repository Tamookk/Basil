#include "pch.h"
#include "Core/Timer.h"

namespace Basil
{
	// Constructor
	Timer::Timer()
	{
		reset();
	}

	// Reset the timer
	void Timer::reset()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	// Get the elapsed time
	float Timer::elapsed()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() * 0.001f * 0.001f * 0.001f;
	}

	// Get the elapsed time in milliseconds
	float Timer::elapsedMillis()
	{
		return elapsed() * 1000.0f;
	}
}