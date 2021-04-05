#include "pch.h"
#include "Renderer.h"

namespace Basil
{
	// Set renderer API currently in use
	RendererAPI Renderer::rendererAPI = RendererAPI::OpenGL;

	// Set the renderer API
	void Renderer::setAPI(RendererAPI api)
	{
		rendererAPI = api;
	}

	// Return in use renderer API
	RendererAPI Renderer::getAPI()
	{
		return rendererAPI;
	}
}