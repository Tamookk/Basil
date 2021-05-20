#include "pch.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Basil
{
	// Constructor
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
	{
		glCreateBuffers(1, &rendererID);
		glNamedBufferData(rendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, rendererID);
	}

	// Destructor
	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &rendererID);
	}

	// Set data
	void OpenGLUniformBuffer::setData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(rendererID, offset, size, data);
	}
}