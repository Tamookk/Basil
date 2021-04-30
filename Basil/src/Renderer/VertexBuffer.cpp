#include "pch.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/VertexBuffer.h"

namespace Basil
{
	// Create a vertex buffer
	Shared<VertexBuffer> VertexBuffer::create(std::vector<float>& vertices)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				ASSERT(false, "No renderer API.");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return makeShared<OpenGLVertexBuffer>(vertices);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

	Shared<VertexBuffer> VertexBuffer::create(uint32_t size)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return makeShared<OpenGLVertexBuffer>(size);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}