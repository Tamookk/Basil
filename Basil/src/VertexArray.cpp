#include "pch.h"
#include "VertexArray.h"
#include "OpenGLVertexArray.h"

namespace Basil
{
	VertexArray* VertexArray::create()
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::None:
				ASSERT(false, "No renderer API!");
				return nullptr;
			case RendererAPI::OpenGL:
				return new OpenGLVertexArray();
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}