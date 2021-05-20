/*
 * This header file declares the base class for sending uniforms to shader uniform buffers.
 */
#pragma once

#include "Core/Core.h"

namespace Basil
{
	class UniformBuffer
	{
		public:
			virtual ~UniformBuffer() {}
			virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
			static Shared<UniformBuffer> create(uint32_t size, uint32_t binding);
	};
}
