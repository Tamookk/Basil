#include "pch.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Basil
{
	// For storing the structure of a vertex
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	// For storing renderer data
	struct Renderer2DData
	{
		const uint32_t maxQuads = 10000;
		const uint32_t maxVertices = 4 * maxQuads;
		const uint32_t maxIndices = 6 * maxQuads;
		static const uint32_t maxTextureSlots = 32;

		Shared<VertexArray> quadVertexArray;
		Shared<VertexBuffer> quadVertexBuffer;
		Shared<Shader> textureShader;
		Shared<Texture2D> whiteTexture;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		std::array<Shared<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1;
	};

	// Declare a pointer to the data storage
	static Renderer2DData data;

	// Initialise function
	void Renderer2D::init()
	{
		PROFILE_FUNCTION();

		// Initialise data and create a VAO
		data.quadVertexArray = VertexArray::create();

		// Create vertex buffer
		data.quadVertexBuffer = VertexBuffer::create(data.maxVertices * sizeof(QuadVertex));
		data.quadVertexBuffer->setLayout({
				{ "a_Position",		ShaderDataType::Float3 },
				{ "a_Color",		ShaderDataType::Float4 },
				{ "a_TexCoord",		ShaderDataType::Float2 },
				{ "a_TexIndex",		ShaderDataType::Float },
				{ "a_TilingFactor", ShaderDataType::Float }
			});
		data.quadVertexArray->addVertexBuffer(data.quadVertexBuffer);
		data.quadVertexBufferBase = new QuadVertex[data.maxVertices];

		// Generate indices
		std::vector<uint32_t> quadIndices;
		quadIndices.reserve(data.maxIndices);
		uint32_t offset = 0;
		for (uint32_t i = 0; i < data.maxIndices; i += 6)
		{
			quadIndices.push_back(offset + 0);
			quadIndices.push_back(offset + 1);
			quadIndices.push_back(offset + 2);

			quadIndices.push_back(offset + 2);
			quadIndices.push_back(offset + 3);
			quadIndices.push_back(offset + 1);

			offset += 4;
		}

		// Create IBO
		Shared<IndexBuffer> squareIbo = IndexBuffer::create(quadIndices);
		data.quadVertexArray->setIndexBuffer(squareIbo);

		// Create white texture
		data.whiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[data.maxTextureSlots];
		for (uint32_t i = 0; i < data.maxTextureSlots; i++)
			samplers[i] = i;

		// Initialise shader
		data.textureShader = Shader::create("assets/shaders/Texture.glsl");
		data.textureShader->bind();
		data.textureShader->setIntArray("u_Textures", samplers, data.maxTextureSlots);

		data.textureSlots[0] = data.whiteTexture;
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

		data.textureSlotIndex = 1;
	}

	// End a scene
	void Renderer2D::endScene()
	{
		PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)data.quadVertexBufferPtr - (uint8_t*)data.quadVertexBufferBase;
		data.quadVertexBuffer->setData(data.quadVertexBufferBase, dataSize);

		flush();
	}

	// Flush
	void Renderer2D::flush()
	{
		// Bind textures
		for (uint32_t i = 0; i < data.textureSlotIndex; i++)
			data.textureSlots[i]->bind(i);

		// Draw
		Renderer::drawIndexed(data.quadVertexArray, data.quadIndexCount);
	}

	// Draw a quad (3D position)
	void Renderer2D::drawQuad(const Transform& transform, const glm::vec4& color)
	{
		PROFILE_FUNCTION();

		const float texIndex = 0.0f;
		const float tilingFactor = 1.0f;

		data.quadVertexBufferPtr->Position = { transform.position.x, transform.position.y, transform.position.z };
		data.quadVertexBufferPtr->Color = color;
		data.quadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		data.quadVertexBufferPtr->TexIndex = texIndex;
		data.quadVertexBufferPtr->TilingFactor = tilingFactor;
		data.quadVertexBufferPtr++;

		data.quadVertexBufferPtr->Position = { transform.position.x + transform.scale.x, transform.position.y, transform.position.z };
		data.quadVertexBufferPtr->Color = color;
		data.quadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		data.quadVertexBufferPtr->TexIndex = texIndex;
		data.quadVertexBufferPtr->TilingFactor = tilingFactor;
		data.quadVertexBufferPtr++;

		data.quadVertexBufferPtr->Position = { transform.position.x, transform.position.y + transform.scale.y, transform.position.z };
		data.quadVertexBufferPtr->Color = color;
		data.quadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		data.quadVertexBufferPtr->TexIndex = texIndex;
		data.quadVertexBufferPtr->TilingFactor = tilingFactor;
		data.quadVertexBufferPtr++;

		data.quadVertexBufferPtr->Position = { transform.position.x + transform.scale.x, transform.position.y + transform.scale.y, transform.position.z };
		data.quadVertexBufferPtr->Color = color;
		data.quadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		data.quadVertexBufferPtr->TexIndex = texIndex;
		data.quadVertexBufferPtr->TilingFactor = tilingFactor;
		data.quadVertexBufferPtr++;

		data.quadIndexCount += 6;

		/*
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
		*/
	}

	// Draw a quad with a texture (3D position)
	void Renderer2D::drawQuad(const Transform& transform, float textureScale, const Shared<Texture2D>& texture, const glm::vec4& tintColor)
	{
		PROFILE_FUNCTION();

		// Set color to white
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		
		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < data.textureSlotIndex; i++)
		{
			if (*data.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)data.textureSlotIndex;
			data.textureSlots[data.textureSlotIndex] = texture;
			data.textureSlotIndex++;
		}

		data.quadVertexBufferPtr->Position = { transform.position.x, transform.position.y, transform.position.z };
		data.quadVertexBufferPtr->Color = color;
		data.quadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		data.quadVertexBufferPtr->TexIndex = textureIndex;
		data.quadVertexBufferPtr->TilingFactor = textureScale;
		data.quadVertexBufferPtr++;

		data.quadVertexBufferPtr->Position = { transform.position.x + transform.scale.x, transform.position.y, transform.position.z };
		data.quadVertexBufferPtr->Color = color;
		data.quadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		data.quadVertexBufferPtr->TexIndex = textureIndex;
		data.quadVertexBufferPtr->TilingFactor = textureScale;
		data.quadVertexBufferPtr++;

		data.quadVertexBufferPtr->Position = { transform.position.x, transform.position.y + transform.scale.y, transform.position.z };
		data.quadVertexBufferPtr->Color = color;
		data.quadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		data.quadVertexBufferPtr->TexIndex = textureIndex;
		data.quadVertexBufferPtr->TilingFactor = textureScale;
		data.quadVertexBufferPtr++;

		data.quadVertexBufferPtr->Position = { transform.position.x + transform.scale.x, transform.position.y + transform.scale.y, transform.position.z };
		data.quadVertexBufferPtr->Color = color;
		data.quadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		data.quadVertexBufferPtr->TexIndex = textureIndex;
		data.quadVertexBufferPtr->TilingFactor = textureScale;
		data.quadVertexBufferPtr++;

		data.quadIndexCount += 6;

		/*
		// Calculate and upload transform
		glm::mat4 transformMatrix =
			glm::translate(glm::mat4(1.0f), glm::vec3({ transform.position.x, transform.position.y, transform.position.z }))
			* glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation), glm::vec3({ 0.0f, 0.0f, 1.0f }))
			* glm::scale(glm::mat4(1.0f), { transform.scale.x, transform.scale.y, transform.scale.z });
		data.textureShader->setMat4("u_Transform", transformMatrix);

		// Bind texture, upload texture scale uniform
		texture->bind();
		data.textureShader->setFloat("u_TexScale", textureScale);

		// Upload color uniform
		data.textureShader->setFloat4("u_Color", tintColor);

		// Bind VAO and draw quad
		data.quadVertexArray->bind();
		Renderer::drawIndexed(data.quadVertexArray);
		*/
	}
}