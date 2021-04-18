#include "pch.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer/Renderer.h"
#include "Renderer/VertexArray.h"

namespace Basil
{
	Shared<VertexArray> VertexArray::create()
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				ASSERT(false, "No renderer API!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return makeShared<OpenGLVertexArray>();
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}