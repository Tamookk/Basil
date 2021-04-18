#include "pch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/Renderer.h"

namespace Basil
{
	// Set the render API currently in use
	Unique<RendererAPI> Renderer::rendererAPI = makeUnique<OpenGLRendererAPI>();

	// Screate the scene data struct
	Unique<Renderer::SceneData> Renderer::sceneData = makeUnique<Renderer::SceneData>();

	// Initialise renderer
	void Renderer::init()
	{
		rendererAPI->init();
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
		// Bind the shader and upload the uniform
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_ViewProjection", sceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_Transform", transform);

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
	void Renderer::drawIndexed(const Shared<VertexArray>& vao)
	{
		rendererAPI->drawIndexed(vao);
	}

	// Return the rendering API currently being used
	RendererAPI::API Renderer::getAPI()
	{
		return RendererAPI::getAPI();
	}
}