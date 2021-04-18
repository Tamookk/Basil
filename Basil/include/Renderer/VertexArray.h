/*
 * This header file declares a class used for creating and using
 * vertex array objects.
 */
#pragma once

#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexBuffer.h"

namespace Basil
{
	class VertexArray
	{
		public:
			virtual ~VertexArray() {}
			virtual void bind() const = 0;
			virtual void unbind() const = 0;
			virtual void addVertexBuffer(const Shared<VertexBuffer>& vbo) = 0;
			virtual const std::vector<Shared<VertexBuffer>>& getVertexBuffers() const = 0;
			virtual void setIndexBuffer(const Shared<IndexBuffer>& ibo) = 0;
			virtual const Shared<IndexBuffer>& getIndexBuffer() const = 0;
			static Shared<VertexArray> create();
	};
}