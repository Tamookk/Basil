#include "pch.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Basil
{
	// Constructor
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t indexCount)
	{
		PROFILE_FUNCTION();

		// Set size of index buffer
		size = indexCount;

		// Create IBO and buffer data
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indexCount, &indices[0], GL_STATIC_DRAW);
	}
	
	// Destructor
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		PROFILE_FUNCTION();

		glDeleteBuffers(1, &ibo);
	}

	// Bind IBO
	void OpenGLIndexBuffer::bind() const
	{
		PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	}

	// Unbind IBO
	void OpenGLIndexBuffer::unbind() const
	{
		PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Return size of the IBO
	uint32_t OpenGLIndexBuffer::getSize() const
	{
		return size;
	}
}
