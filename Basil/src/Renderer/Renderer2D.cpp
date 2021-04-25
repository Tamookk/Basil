#include "pch.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Basil
{
	// To define sizes of quad data
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec3 Color;
		glm::vec2 TexCoord;
	};

	// For storing renderer data
	struct Renderer2DData
	{
		const uint32_t maxQuads = 10000;
		const uint32_t maxVertices = maxQuads * 4;
		const uint32_t maxIndices = maxQuads * 6;

		Shared<VertexArray> quadVAO;
		Shared<VertexBuffer> quadVBO;
		Shared<Shader> textureShader;
		Shared<Texture2D> whiteTexture;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;
	};

	// Declare a pointer to the data storage
	static Renderer2DData data;

	// Initialise function
	void Renderer2D::init()
	{
		PROFILE_FUNCTION();

		// Create a VAO
		data.quadVAO = VertexArray::create();

		// Create and specify format of VBO
		data.quadVBO = VertexBuffer::create(data.maxVertices * sizeof(QuadVertex));
		{
			BufferLayout layout =
			{
				{ "a_Position", ShaderDataType::Float3 },
				{ "a_Color",	ShaderDataType::Float4 },
				{ "a_TexCoord", ShaderDataType::Float2 }
			};

			data.quadVBO->setLayout(layout);
		}

		// Add VBO to VAO
		data.quadVAO->addVertexBuffer(data.quadVBO);

		// Cread quad vertex buffer base
		data.quadVertexBufferBase = new QuadVertex[data.maxVertices];

		// Create quad indices
		uint32_t* quadIndices = new uint32_t[data.maxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < data.maxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 1;

			offset += 4;
		}

		// Create IBO and add to VAO
		Shared<IndexBuffer> quadIBO = IndexBuffer::create(quadIndices, data.maxIndices);
		data.quadVAO->setIndexBuffer(quadIBO);
		delete[] quadIndices;

		// Create white texture
		data.whiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

		// Initialise shader
		data.textureShader = Shader::create("assets/shaders/Texture.glsl");
		data.textureShader->bind();
		data.textureShader->setInt("u_Texture", 0);
	}

	// Shutdown function
	void Renderer2D::shutdown()
	{
		PROFILE_FUNCTION();
	}

	// Begin a scene
	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		PROFILE_FUNCTION();

		// Bind shader and upload uniforms
		data.textureShader->bind();
		data.textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

		data.quadIndexCount = 0;
		data.quadVertexBufferPtr = data.quadVertexBufferBase;
	}

	// End a scene
	void Renderer2D::endScene()
	{
		PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)data.quadVertexBufferPtr - (uint8_t*)data.quadVertexBufferBase;
		data.quadVBO->setData(data.quadVertexBufferBase, dataSize);

		flush();
	}

	// Draw indexed data (batching)
	void Renderer2D::flush()
	{
		Renderer::drawIndexed(data.quadVAO, data.quadIndexCount);
	}

	// Draw a quad (3D position)
	void Renderer2D::drawQuad(const Transform& transform, const glm::vec4& color)
	{
		PROFILE_FUNCTION();

		// Batching (no rotation)
		if (transform.rotation == 0.0f)
		{
			data.quadVertexBufferPtr->Position = { transform.position.x, transform.position.y, transform.position.z };
			data.quadVertexBufferPtr->Color = color;
			data.quadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
			data.quadVertexBufferPtr++;

			data.quadVertexBufferPtr->Position = { transform.position.x, transform.position.y, transform.position.z };
			data.quadVertexBufferPtr->Color = color;
			data.quadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
			data.quadVertexBufferPtr++;

			data.quadVertexBufferPtr->Position = { transform.position.x, transform.position.y, transform.position.z };
			data.quadVertexBufferPtr->Color = color;
			data.quadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
			data.quadVertexBufferPtr++;

			data.quadVertexBufferPtr->Position = { transform.position.x, transform.position.y, transform.position.z };
			data.quadVertexBufferPtr->Color = color;
			data.quadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
			data.quadVertexBufferPtr++;

			data.quadIndexCount += 6;
		}
		else
		{
			glm::mat4 transformMatrix =
				glm::translate(glm::mat4(1.0f), glm::vec3({ transform.position.x, transform.position.y, transform.position.z }))
			  * glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation), glm::vec3({ 0.0f, 0.0f, 1.0f }))
			  * glm::scale(glm::mat4(1.0f), { transform.scale.x, transform.scale.y, transform.scale.z });

			data.textureShader->setMat4("u_Transform", transformMatrix);

			// Bind white texture, upload texture scale uniform
			data.whiteTexture->bind();
			data.textureShader->setFloat("u_TexScale", 1.0f);

			// Upload color uniform
			data.textureShader->setFloat4("u_Color", color);

			// Bind VAO and draw
			data.quadVAO->bind();
			Renderer::drawIndexed(data.quadVAO);
		}
	}

	// Draw a quad with a texture (3D position)
	void Renderer2D::drawQuad(const Transform& transform, float textureScale, const Shared<Texture2D>& texture, const glm::vec4& tintColor)
	{
		PROFILE_FUNCTION();

		// Batching (no rotation)
		if (transform.rotation == 0.0f)
		{
			data.quadVertexBufferPtr->Position = { transform.position.x, transform.position.y, transform.position.z };
			data.quadVertexBufferPtr->Color = tintColor;
			data.quadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
			data.quadVertexBufferPtr++;

			data.quadVertexBufferPtr->Position = { transform.position.x, transform.position.y, transform.position.z };
			data.quadVertexBufferPtr->Color = tintColor;
			data.quadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
			data.quadVertexBufferPtr++;

			data.quadVertexBufferPtr->Position = { transform.position.x, transform.position.y, transform.position.z };
			data.quadVertexBufferPtr->Color = tintColor;
			data.quadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
			data.quadVertexBufferPtr++;

			data.quadVertexBufferPtr->Position = { transform.position.x, transform.position.y, transform.position.z };
			data.quadVertexBufferPtr->Color = tintColor;
			data.quadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
			data.quadVertexBufferPtr++;

			data.quadIndexCount += 6;
		}
		else
		{
			glm::mat4 transformMatrix =
				glm::translate(glm::mat4(1.0f), glm::vec3({ transform.position.x, transform.position.y, transform.position.z }))
				* glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation), glm::vec3({ 0.0f, 0.0f, 1.0f }))
				* glm::scale(glm::mat4(1.0f), { transform.scale.x, transform.scale.y, transform.scale.z });

			data.textureShader->setMat4("u_Transform", transformMatrix);

			// Bind white texture, upload texture scale uniform
			data.whiteTexture->bind();
			data.textureShader->setFloat("u_TexScale", 1.0f);

			// Upload color uniform
			data.textureShader->setFloat4("u_Color", tintColor);

			// Bind VAO and draw
			data.quadVAO->bind();
			Renderer::drawIndexed(data.quadVAO);
		}
	}
}