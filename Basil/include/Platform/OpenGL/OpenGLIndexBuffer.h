/*
 * This header file contains a class for the OpenGL-specific implementation of
 * IndexBuffer.h.
 */
#pragma once

#include "Renderer/IndexBuffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Basil
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
		public:
			OpenGLIndexBuffer(uint32_t* indices, uint32_t indexCount = 0);
			~OpenGLIndexBuffer() override;
			void bind() const override;
			void unbind() const override;
			uint32_t getSize() const override;
		private:
			unsigned int ibo;
			uint32_t size;
	};
}
