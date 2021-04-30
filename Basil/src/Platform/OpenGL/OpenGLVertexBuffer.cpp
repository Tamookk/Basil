#include "pch.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Basil
{
	// Constructor
	OpenGLVertexBuffer::OpenGLVertexBuffer(std::vector<float>& vertices)
	{
		PROFILE_FUNCTION();

		// Create VBO and buffer data
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		PROFILE_FUNCTION();

		glCreateBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	// Destructor
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		PROFILE_FUNCTION();

		// Delete the VBO
		glDeleteBuffers(1, &vbo);
	}

	// Provide data to the buffer
	void OpenGLVertexBuffer::setData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	// Bind the buffer
	void OpenGLVertexBuffer::bind() const
	{
		PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	}

	// Unbind the buffer
	void OpenGLVertexBuffer::unbind() const
	{
		PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Get the buffer layout
	const BufferLayout& OpenGLVertexBuffer::getLayout() const
	{
		return layout;
	}

	// Set the buffer layout
	void OpenGLVertexBuffer::setLayout(const BufferLayout& layout)
	{
		this->layout = layout;
	}
}
