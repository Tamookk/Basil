#include "pch.h"
#include "VertexBuffer.h"
#include "OpenGLVertexBuffer.h"

namespace Basil
{
	// Create a vertex buffer
	VertexBuffer* VertexBuffer::create(std::vector<float>& vertices)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::None:
				ASSERT(false, "No renderer API!");
				return nullptr;
			case RendererAPI::OpenGL:
				return new OpenGLVertexBuffer(vertices);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}