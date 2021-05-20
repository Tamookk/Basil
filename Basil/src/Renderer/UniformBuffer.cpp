#include "pch.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/UniformBuffer.h"

namespace Basil
{
	Shared<UniformBuffer> UniformBuffer::create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "No renderer API.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return makeShared<OpenGLUniformBuffer>(size, binding);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}