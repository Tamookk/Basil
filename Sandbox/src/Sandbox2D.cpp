#include "Platform/OpenGL/OpenGLShader.h"
#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), cameraController(1280.0f / 720.0f) {}

void Sandbox2D::onAttach()
{
	// Create vertices and indices
	std::vector<float> squareVertices =
	{
		-0.6f, -0.6f, 0.0f,			// vertex 0 xyz
		-0.6f,  0.6f, 0.0f,			// vertex 1 xyz
		 0.6f, -0.6f, 0.0f,			// vertex 2 xyz
		 0.6f,  0.6f, 0.0f,			// vertex 3 xyz
	};

	std::vector<unsigned int> squareIndices =
	{
		0, 1, 2,
		2, 3, 1
	};

	// Create and specify format of VBO
	Basil::Shared<Basil::VertexBuffer> squareVbo;
	squareVbo.reset(Basil::VertexBuffer::create(squareVertices));
	{
		Basil::BufferLayout layout =
		{
			{ "a_Position", ShaderDataType::Float3 }
		};

		squareVbo->setLayout(layout);
	}

	// Create IBO
	Basil::Shared<Basil::IndexBuffer> squareIbo;
	squareIbo.reset(Basil::IndexBuffer::create(squareIndices));

	// Initialise VAO and add VBO & IBO to it
	vao = Basil::VertexArray::create();
	vao->addVertexBuffer(squareVbo);
	vao->setIndexBuffer(squareIbo);

	// Initialise shader
	shader = Basil::Shader::create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::onDetach() {}

void Sandbox2D::onUpdate(Basil::Timestep timeStep)
{
	// Update camera
	cameraController.onUpdate(timeStep);

	// Render
	Basil::Renderer::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Basil::Renderer::clear();
	Basil::Renderer::beginScene(cameraController.getCamera());
	std::dynamic_pointer_cast<Basil::OpenGLShader>(shader)->bind();
	std::dynamic_pointer_cast<Basil::OpenGLShader>(shader)->uploadUniformFloat4("u_Color", squareColor);
	Basil::Renderer::submit(shader, vao);
	Basil::Renderer::endScene();
}

void Sandbox2D::onEvent(Basil::Event& e)
{
	cameraController.onEvent(e);
}

void Sandbox2D::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	ImGui::End();
}