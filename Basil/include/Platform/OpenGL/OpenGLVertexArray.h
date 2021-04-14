/*
 * This header file contains a class for the OpenGL-specific implementation of
 * VertexArray.h.
 */
#pragma once

#include "Renderer/VertexArray.h"

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
			void addVertexBuffer(const Shared<VertexBuffer>& vbo) override;
			void setIndexBuffer(const Shared<IndexBuffer>& ibo) override;
			const std::vector<Shared<VertexBuffer>>& getVertexBuffers() const override;
			const Shared<IndexBuffer>& getIndexBuffer() const override;
		private:
			unsigned int vao;
			std::vector<Shared<VertexBuffer>> vbos;
			Shared<IndexBuffer> ibo;
	};
}
