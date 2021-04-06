#include "pch.h"
#include "VertexBuffer.h"
#include "OpenGLVertexBuffer.h"
#include "Renderer.h"

namespace Basil
{
	// Create a vertex buffer
	VertexBuffer* VertexBuffer::create(std::vector<float>& vertices)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				ASSERT(false, "No renderer API!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return new OpenGLVertexBuffer(vertices);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}