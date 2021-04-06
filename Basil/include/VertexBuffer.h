/*
 * This header file contains a class for creating vertex buffers.
 */
#pragma once

#include <cstdint>

#include "BufferLayout.h"
#include "Renderer.h"

namespace Basil
{
	class VertexBuffer
	{
		public:
			virtual ~VertexBuffer() {};
			virtual void bind() const = 0;
			virtual void unbind() const = 0;
			virtual const BufferLayout& getLayout() const = 0;
			virtual void setLayout(const BufferLayout& layout) = 0;
			static VertexBuffer* create(std::vector<float>& vertices);
	};
}