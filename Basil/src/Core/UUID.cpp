#include "pch.h"

#include <random>
#include <unordered_map>

#include "Core/UUID.h"

namespace Basil
{
	// Static class variables for random number generation
	static std::random_device randomDevice;
	static std::mt19937_64 engine(randomDevice());
	static std::uniform_int_distribution<uint64_t> uniformDistribution;

	// Constructor
	UUID::UUID()
	{
		uuid = uniformDistribution(engine);
	}

	// Constructor where you can set UUID
	UUID::UUID(uint64_t uuid)
	{
		this->uuid = uuid;
	}

	// Operator uint64_t
	UUID::operator uint64_t() const
	{
		return uuid;
	}
}