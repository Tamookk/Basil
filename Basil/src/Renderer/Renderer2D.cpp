#include "pch.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

namespace Basil
{
	// For storing the VAO and shader
	struct Renderer2DStorage
	{
		Shared<VertexArray> quadVertexArray;
		Shared<Shader> flatColorShader;
	};

	// Declare a pointer to the data storage
	static Renderer2DStorage* data;

	// Initialise function
	void Renderer2D::init()
	{
		// Initialise data and create a VAO
		data = new Renderer2DStorage();
		data->quadVertexArray = VertexArray::create();

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
		Shared<VertexBuffer> squareVbo;
		squareVbo.reset(VertexBuffer::create(squareVertices));
		{
			BufferLayout layout =
			{
				{ "a_Position", ShaderDataType::Float3 }
			};

			squareVbo->setLayout(layout);
		}

		// Create IBO
		Shared<IndexBuffer> squareIbo;
		squareIbo.reset(IndexBuffer::create(squareIndices));

		// Initialise VAO and add VBO & IBO to it
		data->quadVertexArray = VertexArray::create();
		data->quadVertexArray->addVertexBuffer(squareVbo);
		data->quadVertexArray->setIndexBuffer(squareIbo);

		// Initialise shader
		data->flatColorShader = Shader::create("assets/shaders/FlatColor.glsl");
	}

	// Shutdown function
	void Renderer2D::shutdown()
	{
		delete data;
	}

	// Begin a scene
	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		// Bind shader and upload uniforms
		data->flatColorShader->bind();
		data->flatColorShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
		data->flatColorShader->setMat4("u_Transform", glm::mat4(1.0f));
	}

	// End a scene
	void Renderer2D::endScene()
	{

	}

	// Draw a quad (2D position)
	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		// Draw a quad with z-position set to 0
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	// Draw a quad (3D position)
	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		// Bind shader and upload color uniform
		data->flatColorShader->bind();
		data->flatColorShader->setFloat4("u_Color", color);

		// Bind VAO and draw
		data->quadVertexArray->bind();
		Renderer::drawIndexed(data->quadVertexArray);
	}
}