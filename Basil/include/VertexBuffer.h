/*
 * This header file contains a class for creating vertex buffers.
 */
#pragma once

#include <cstdint>

#include "Renderer.h"

namespace Basil
{
	class VertexBuffer
	{
		public:
			virtual ~VertexBuffer() {};
			virtual void bind() const = 0;
			virtual void unbind() const = 0;
			static VertexBuffer* create(std::vector<float>& vertices);
	};
}