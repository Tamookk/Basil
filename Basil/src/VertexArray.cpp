#include "pch.h"
#include "VertexArray.h"
#include "OpenGLVertexArray.h"
#include "Renderer.h"

namespace Basil
{
	VertexArray* VertexArray::create()
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				ASSERT(false, "No renderer API!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return new OpenGLVertexArray();
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}