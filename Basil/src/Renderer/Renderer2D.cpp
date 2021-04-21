#include "pch.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Basil
{
	// For storing the VAO and shader
	struct Renderer2DStorage
	{
		Shared<VertexArray> quadVertexArray;
		Shared<Shader> flatColorShader;
		Shared<Shader> textureShader;
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
			 0.0f,  0.0f,				// vertex 0 tex coord
			-0.6f,  0.6f, 0.0f,			// vertex 1 xyz
			 0.0,   1.0,				// vertex 1 tex coord
			 0.6f, -0.6f, 0.0f,			// vertex 2 xyz
			 1.0f,  0.0f,				// vertex 2 tex coord
			 0.6f,  0.6f, 0.0f,			// vertex 3 xyz
			 1.0f,  1.0f,				// vertex 3 tex coord
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
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_TexCoord", ShaderDataType::Float2 }
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

		// Initialise shaders
		data->flatColorShader = Shader::create("assets/shaders/FlatColor.glsl");
		data->textureShader = Shader::create("assets/shaders/Texture.glsl");
		data->textureShader->bind();
		data->textureShader->setInt("u_Texture", 0);
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

		data->textureShader->bind();
		data->textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
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

		// Calculate and set transform
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		data->flatColorShader->setMat4("u_Transform", transform);

		// Bind VAO and draw
		data->quadVertexArray->bind();
		Renderer::drawIndexed(data->quadVertexArray);
	}

	// Draw a quad with a texture (2D position)
	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D>& texture)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture);;
	}

	// Draw a quad with a texture (3D position)
	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D>& texture)
	{
		// Bind texture shader
		data->textureShader->bind();

		// Upload transform
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		data->textureShader->setMat4("u_Transform", transform);

		// Bind texture
		texture->bind();

		// Bind VAO and draw quad
		data->quadVertexArray->bind();
		Renderer::drawIndexed(data->quadVertexArray);
	}
}