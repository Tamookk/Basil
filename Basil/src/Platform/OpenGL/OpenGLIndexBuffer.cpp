#include "pch.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Basil
{
	// Constructor
	OpenGLIndexBuffer::OpenGLIndexBuffer(std::vector<uint32_t>& indices)
	{
		// Set size of index buffer
		size = (uint32_t)indices.size();

		// Create IBO and buffer data
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), &indices[0], GL_STATIC_DRAW);
	}
	
	// Destructor
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &ibo);
	}

	// Bind IBO
	void OpenGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	}

	// Unbind IBO
	void OpenGLIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Return size of the IBO
	uint32_t OpenGLIndexBuffer::getSize() const
	{
		return size;
	}
}
