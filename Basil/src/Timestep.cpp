#include "pch.h"
#include "Timestep.h"

namespace Basil
{
	// Constructor
	Timestep::Timestep(float time)
	{
		this->time = time;
	}

	// Get time in seconds
	float Timestep::getSeconds() const
	{
		return time;
	}

	// Get time in milliseconds
	float Timestep::getMilliseconds() const
	{
		return time * 1000.0f;
	}
}