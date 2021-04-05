/*
 * This header file contains a class for the OpenGL-specific implementation of
 * VertexBuffer.h.
 */
#pragma once

#include "VertexBuffer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Basil
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
		public:
			OpenGLVertexBuffer(std::vector<float>& vertices);
			~OpenGLVertexBuffer() override;
			void bind() const override;
			void unbind() const override;
		private:
			unsigned int vbo;
	};
}
