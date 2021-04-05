#include "pch.h"
#include "IndexBuffer.h"
#include "OpenGLIndexBuffer.h"

namespace Basil
{
	// Create an index buffer
	IndexBuffer* IndexBuffer::create(std::vector<uint32_t>& indices)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::None:
				ASSERT(false, "No renderer API!");
				return nullptr;
			case RendererAPI::OpenGL:
				return new OpenGLIndexBuffer(indices);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}