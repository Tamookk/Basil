/*
 * This header file contains a class for the OpenGL-specific implementation of
 * VertexArray.h.
 */
#pragma once

#include "VertexArray.h"

#include <glad/glad.h>

namespace Basil
{
	class OpenGLVertexArray : public VertexArray
	{
		public:
			OpenGLVertexArray();
			~OpenGLVertexArray();
			void bind() const override;
			void unbind() const override;
			void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo) override;
			void setIndexBuffer(const std::shared_ptr<IndexBuffer>& ibo) override;
			const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const override;
			const std::shared_ptr<IndexBuffer>& getIndexBuffer() const override;
		private:
			unsigned int vao;
			std::vector<std::shared_ptr<VertexBuffer>> vbos;
			std::shared_ptr<IndexBuffer> ibo;
	};
}
