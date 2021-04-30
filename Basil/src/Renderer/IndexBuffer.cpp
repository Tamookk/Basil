#include "pch.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Renderer.h"

namespace Basil
{
	// Create an index buffer
	Shared<IndexBuffer> IndexBuffer::create(std::vector<uint32_t>& indices)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				ASSERT(false, "No renderer API.");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return makeShared<OpenGLIndexBuffer>(indices);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}