/*
 * This header file defines a class for creating index buffers.
 */
#pragma once

#include <cstdint>

#include "Renderer.h"

namespace Basil
{
	class IndexBuffer
	{
		public:
			virtual ~IndexBuffer() {};
			virtual void bind() const = 0;
			virtual void unbind() const = 0;
			virtual uint32_t getSize() const = 0;
			static IndexBuffer* create(std::vector<uint32_t>& indices);
	};
}