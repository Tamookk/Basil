#include "pch.h"
#include "Renderer.h"
#include "OpenGLRendererAPI.h"

namespace Basil
{
	// Set the render API currently in use
	RendererAPI* Renderer::rendererAPI = new OpenGLRendererAPI;

	// Begin a scene
	void Renderer::beginScene()
	{

	}

	// End a scene
	void Renderer::endScene()
	{

	}

	// Submit data to be rendered
	void Renderer::submit(const std::shared_ptr<VertexArray>& vao)
	{
		vao->bind();
		Renderer::drawIndexed(vao);
	}

	// Set the clear colour
	void Renderer::setClearColor(const glm::vec4& color)
	{
		rendererAPI->setClearColor(color);
	}

	// Clear the screen
	void Renderer::clear()
	{
		rendererAPI->clear();
	}

	// Draw the object(s) in the VAO
	void Renderer::drawIndexed(const std::shared_ptr<VertexArray>& vao)
	{
		rendererAPI->drawIndexed(vao);
	}

	// Return the rendering API currently being used
	RendererAPI::API Renderer::getAPI()
	{
		return RendererAPI::getAPI();
	}
}