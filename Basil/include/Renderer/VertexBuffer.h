/*
 * This header file contains a class for creating vertex buffers.
 */
#pragma once

#include <cstdint>

#include "Renderer/BufferLayout.h"

namespace Basil
{
	class VertexBuffer
	{
		public:
			virtual ~VertexBuffer() {};
			virtual void bind() const = 0;
			virtual void unbind() const = 0;
			virtual void setData(const void* data, uint32_t size) = 0;
			virtual const BufferLayout& getLayout() const = 0;
			virtual void setLayout(const BufferLayout& layout) = 0;
			static Shared<VertexBuffer> create(uint32_t size);
			static Shared<VertexBuffer> create(std::vector<float>& vertices);
	};
}