/*
 * This header file contains a class for the OpenGL-specific implementation of
 * VertexBuffer.h.
 */
#pragma once

#include "Renderer/VertexBuffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Basil
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
		public:
			OpenGLVertexBuffer(std::vector<float>& vertices);
			OpenGLVertexBuffer(uint32_t size);
			~OpenGLVertexBuffer() override;
			void setData(const void* data, uint32_t size);
			void bind() const override;
			void unbind() const override;
			const BufferLayout& getLayout() const override;
			void setLayout(const BufferLayout& layout) override;
		private:
			unsigned int vbo;
			BufferLayout layout;
	};
}
