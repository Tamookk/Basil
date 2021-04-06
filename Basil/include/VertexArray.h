/*
 * This header file declares a class used for creating and using
 * vertex array objects.
 */
#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace Basil
{
	class VertexArray
	{
		public:
			virtual ~VertexArray() {}
			virtual void bind() const = 0;
			virtual void unbind() const = 0;
			virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo) = 0;
			virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& ibo) = 0;
			virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const = 0;
			virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const = 0;
			static VertexArray* create();
	};
}