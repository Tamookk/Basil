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
		Shared<Shader> textureShader;
		Shared<Texture2D> whiteTexture;
	};

	// Declare a pointer to the data storage
	static Renderer2DStorage* data;

	// Initialise function
	void Renderer2D::init()
	{
		PROFILE_FUNCTION();

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
		Shared<VertexBuffer> squareVbo = VertexBuffer::create(squareVertices);
		{
			BufferLayout layout =
			{
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_TexCoord", ShaderDataType::Float2 }
			};

			squareVbo->setLayout(layout);
		}

		// Create IBO
		Shared<IndexBuffer> squareIbo = IndexBuffer::create(squareIndices);

		// Initialise VAO and add VBO & IBO to it
		data->quadVertexArray = VertexArray::create();
		data->quadVertexArray->addVertexBuffer(squareVbo);
		data->quadVertexArray->setIndexBuffer(squareIbo);

		// Create white texture
		data->whiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		data->whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

		// Initialise shader
		data->textureShader = Shader::create("assets/shaders/Texture.glsl");
		data->textureShader->bind();
		data->textureShader->setInt("u_Texture", 0);
	}

	// Shutdown function
	void Renderer2D::shutdown()
	{
		PROFILE_FUNCTION();

		delete data;
	}

	// Begin a scene
	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		PROFILE_FUNCTION();

		// Bind shader and upload uniforms
		data->textureShader->bind();
		data->textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
	}

	// End a scene
	void Renderer2D::endScene()
	{
		PROFILE_FUNCTION();
	}

	// Draw a quad (3D position)
	void Renderer2D::drawQuad(const Transform& transform, const glm::vec4& color)
	{
		PROFILE_FUNCTION();

		// Calculate and upload transform - only do rotation if it is set
		glm::mat4 transformMatrix;
		if (transform.rotation == 0.0f)
			transformMatrix =
				glm::translate(glm::mat4(1.0f), glm::vec3({ transform.position.x, transform.position.y, transform.position.z }))
			  * glm::scale(glm::mat4(1.0f), { transform.scale.x, transform.scale.y, transform.scale.z });
		else
			transformMatrix =
				glm::translate(glm::mat4(1.0f), glm::vec3({ transform.position.x, transform.position.y, transform.position.z }))
			  * glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation), glm::vec3({ 0.0f, 0.0f, 1.0f }))
			  * glm::scale(glm::mat4(1.0f), { transform.scale.x, transform.scale.y, transform.scale.z });
		
		data->textureShader->setMat4("u_Transform", transformMatrix);

		// Bind white texture, upload texture scale uniform
		data->whiteTexture->bind();
		data->textureShader->setFloat("u_TexScale", 1.0f);
		
		// Upload color uniform
		data->textureShader->setFloat4("u_Color", color);

		// Bind VAO and draw
		data->quadVertexArray->bind();
		Renderer::drawIndexed(data->quadVertexArray);
	}

	// Draw a quad with a texture (3D position)
	void Renderer2D::drawQuad(const Transform& transform, float textureScale, const Shared<Texture2D>& texture, const glm::vec4& tintColor)
	{
		PROFILE_FUNCTION();

		// Calculate and upload transform
		glm::mat4 transformMatrix =
			glm::translate(glm::mat4(1.0f), glm::vec3({ transform.position.x, transform.position.y, transform.position.z }))
		  * glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation), glm::vec3({ 0.0f, 0.0f, 1.0f }))
		  * glm::scale(glm::mat4(1.0f), { transform.scale.x, transform.scale.y, transform.scale.z });
		data->textureShader->setMat4("u_Transform", transformMatrix);

		// Bind texture, upload texture scale uniform
		texture->bind();
		data->textureShader->setFloat("u_TexScale", textureScale);

		// Upload color uniform
		data->textureShader->setFloat4("u_Color", tintColor);

		// Bind VAO and draw quad
		data->quadVertexArray->bind();
		Renderer::drawIndexed(data->quadVertexArray);
	}
}