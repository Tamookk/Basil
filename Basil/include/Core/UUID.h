/*
 * This class manages UUIDs. For now they are randomly-generated uint64_t variables,
 * as the likelihood of two UUIDs being the same using this generation method
 * is 1 in 2^64 - 1. That's a big number.
 */
#pragma once

#include <xhash>

namespace Basil
{
	class UUID
	{
		public:
			UUID();
			UUID(uint64_t uuid);
			UUID(const UUID&) = default;
			operator uint64_t() const;
		private:
			uint64_t uuid;
		};
}

// Hashing operator for UUIDs
namespace std
{
	template<>
	struct hash<Basil::UUID>
	{
		std::size_t operator()(const Basil::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}