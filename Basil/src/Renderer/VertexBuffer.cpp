#include "pch.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/VertexBuffer.h"

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