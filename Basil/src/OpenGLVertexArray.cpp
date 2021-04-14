#include "pch.h"
#include "OpenGLVertexArray.h"

namespace Basil
{
	// Convert shader data type to OpenGL type
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}

		ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	// Constructor
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &vao);
	}

	// Destructor
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &vao);
	}

	// Bind the VAO
	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(vao);
	}

	// Unbind the VAO
	void OpenGLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	// Add a VBO
	void OpenGLVertexArray::addVertexBuffer(const Shared<VertexBuffer>& vbo)
	{
		// Check that the VBO has a layout
		ASSERT(vbo->getLayout().getElements().size(), "VBO has no layout!");

		// Bind the VAO and VBO
		bind();
		vbo->bind();

		// Specify the format of the data
		uint32_t index = 0;
		const BufferLayout& layout = vbo->getLayout();
		for (const BufferElement& element : layout)
		{
			glVertexAttribPointer(
				index,
				element.getComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalised ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(const void*)(intptr_t)element.offset
			);
			glEnableVertexAttribArray(index);
			index++;
		}

		// Add the VBO to the vector of VBOs
		vbos.push_back(vbo);
	}

	// Set the IBO
	void OpenGLVertexArray::setIndexBuffer(const Shared<IndexBuffer>& ibo)
	{
		// Bind the IBO and VAO
		ibo->bind();
		bind();

		// Set the IBO
		this->ibo = ibo;
	}

	// Get the VBOs
	const std::vector<Shared<VertexBuffer>>& OpenGLVertexArray::getVertexBuffers() const
	{
		return vbos;
	}

	// Get the IBO
	const Shared<IndexBuffer>& OpenGLVertexArray::getIndexBuffer() const
	{
		return ibo;
	}
}