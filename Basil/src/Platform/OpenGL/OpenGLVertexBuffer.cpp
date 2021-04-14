#include "pch.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Basil
{
	// Constructor
	OpenGLVertexBuffer::OpenGLVertexBuffer(std::vector<float>& vertices)
	{
		// Create VBO and buffer data
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	}

	// Destructor
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		// Delete the VBO
		glDeleteBuffers(1, &vbo);
	}

	// Bind the buffer
	void OpenGLVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	}

	// Unbind the buffer
	void OpenGLVertexBuffer::unbind() const
	{
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
