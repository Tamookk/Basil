#include "pch.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

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

		ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	// Constructor
	OpenGLVertexArray::OpenGLVertexArray()
	{
		PROFILE_FUNCTION();

		glCreateVertexArrays(1, &vao);
	}

	// Destructor
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &vao);
	}

	// Bind the VAO
	void OpenGLVertexArray::bind() const
	{
		PROFILE_FUNCTION();

		glBindVertexArray(vao);
	}

	// Unbind the VAO
	void OpenGLVertexArray::unbind() const
	{
		PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	// Add a VBO
	void OpenGLVertexArray::addVertexBuffer(const Shared<VertexBuffer>& vbo)
	{
		PROFILE_FUNCTION();

		// Check that the VBO has a layout
		ASSERT(vbo->getLayout().getElements().size(), "VBO has no layout.");

		// Bind the VAO and VBO
		bind();
		vbo->bind();

		// Specify the format of the data
		uint32_t index = 0;
		const BufferLayout& layout = vbo->getLayout();
		for (const BufferElement& element : layout)
		{
			switch (element.type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(
						index,
						element.getComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.type),
						element.normalised ? GL_TRUE : GL_FALSE,
						layout.getStride(),
						(const void*)element.offset
					);
					index++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribIPointer(
						index,
						element.getComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.type),
						layout.getStride(),
						(const void*)(intptr_t)element.offset
					);
					index++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					unsigned int count = element.getComponentCount();
					for (unsigned int i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(index);
						glVertexAttribPointer(
							index,
							count,
							ShaderDataTypeToOpenGLBaseType(element.type),
							element.normalised ? GL_TRUE : GL_FALSE,
							layout.getStride(),
							(const void*)(element.offset * count * i)
						);
						glVertexAttribDivisor(index, 1);
						index++;
						break;
					}
				}
				default:
					ASSERT(false, "Unknown ShaderDataType");
			}
			
		}

		// Add the VBO to the vector of VBOs
		vbos.push_back(vbo);
	}

	// Set the IBO
	void OpenGLVertexArray::setIndexBuffer(const Shared<IndexBuffer>& ibo)
	{
		PROFILE_FUNCTION();

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