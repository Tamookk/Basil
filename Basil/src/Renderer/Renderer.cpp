#include "pch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

namespace Basil
{
	// Set the render API currently in use
	Unique<RendererAPI> Renderer::rendererAPI = makeUnique<OpenGLRendererAPI>();

	// Screate the scene data struct
	Unique<Renderer::SceneData> Renderer::sceneData = makeUnique<Renderer::SceneData>();

	// Initialise renderer
	void Renderer::init()
	{
		PROFILE_FUNCTION();

		rendererAPI->init();
		Renderer2D::init();
	}

	// Shutdown renderer
	void Renderer::shutdown()
	{
		Renderer2D::shutdown();
	}

	// On window resize event
	void Renderer::onWindowResize(unsigned int width, unsigned int height)
	{
		rendererAPI->setViewport(0, 0, width, height);
	}

	// Begin a scene
	void Renderer::beginScene(OrthographicCamera& camera)
	{
		sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	// End a scene
	void Renderer::endScene()
	{

	}

	// Submit data to be rendered
	void Renderer::submit(const Shared<Shader>& shader, const Shared<VertexArray>& vao, const glm::mat4& transform)
	{
		PROFILE_FUNCTION();
		// Bind the shader and upload the uniform
		shader->bind();
		shader->setMat4("u_ViewProjection", sceneData->viewProjectionMatrix);
		shader->setMat4("u_Transform", transform);

		// Bind the vertex array and draw the data
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
	void Renderer::drawIndexed(const Shared<VertexArray>& vao, uint32_t count)
	{
		rendererAPI->drawIndexed(vao, count);
	}

	// Return the rendering API currently being used
	RendererAPI::API Renderer::getAPI()
	{
		return RendererAPI::getAPI();
	}
}