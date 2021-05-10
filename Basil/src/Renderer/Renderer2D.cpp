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
		static const uint32_t maxQuads = 20000;
		static const uint32_t maxVertices = 4 * maxQuads;
		static const uint32_t maxIndices = 6 * maxQuads;
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

		glm::vec4 quadVertexPositions[4];

		Renderer2D::Statistics stats;
	};

	// Declare a pointer to the data storage
	static Renderer2DData data;


	// -- Statistics struct functions -- //
	// Return the total vertex count
	uint32_t Renderer2D::Statistics::getTotalVertexCount()
	{
		return Statistics::quadCount * 4;
	}

	// Return the total index count
	uint32_t Renderer2D::Statistics::getTotalIndexCount()
	{
		return Statistics::quadCount * 6;
	}


	// -- Renderer2D functions -- //
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

		// Set texture slot 0 to the white texture
		data.textureSlots[0] = data.whiteTexture;

		data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[2] = { -0.5f,  0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[3] = {  0.5f,  0.5f, 0.0f, 1.0f };
	}

	// Shutdown function
	void Renderer2D::shutdown()
	{
		PROFILE_FUNCTION();

		delete[] data.quadVertexBufferBase;
	}

	// Begin a scene
	void Renderer2D::beginScene(const Camera& camera, const glm::mat4& transform)
	{
		PROFILE_FUNCTION();

		// Generate view-projection matrix
		glm::mat4 viewProj = camera.getProjection() * glm::inverse(transform);

		// Bind shader and upload uniforms
		data.textureShader->bind();
		data.textureShader->setMat4("u_ViewProjection", viewProj);

		// Initialise buffer variables
		data.quadIndexCount = 0;
		data.quadVertexBufferPtr = data.quadVertexBufferBase;

		// Set texture slot index
		data.textureSlotIndex = 1;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		PROFILE_FUNCTION();

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

		uint32_t dataSize = (uint32_t)((uint8_t*)data.quadVertexBufferPtr - (uint8_t*)data.quadVertexBufferBase);
		data.quadVertexBuffer->setData(data.quadVertexBufferBase, dataSize);
		
		flush();
	}

	// Flush
	void Renderer2D::flush()
	{
		// Skip if there is nothing to render
		if (data.quadIndexCount == 0)
			return;

		// Bind textures
		for (uint32_t i = 0; i < data.textureSlotIndex; i++)
			data.textureSlots[i]->bind(i);

		// Draw
		Renderer::drawIndexed(data.quadVertexArray, data.quadIndexCount);

		// Increment draw calls
		data.stats.drawCalls++;
	}

	// Draw a quad (3D position)
	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		PROFILE_FUNCTION();

		// Flush and reset if index limit reached
		if (data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		const float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;
		constexpr glm::vec2 texCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };

		// Set vertex data
		for (int i = 0; i < 4; i++)
		{
			data.quadVertexBufferPtr->Position = transform * data.quadVertexPositions[i];
			data.quadVertexBufferPtr->Color = color;
			data.quadVertexBufferPtr->TexCoord = texCoords[i];
			data.quadVertexBufferPtr->TexIndex = textureIndex;
			data.quadVertexBufferPtr->TilingFactor = tilingFactor;
			data.quadVertexBufferPtr++;
		}

		// Increment index and quad count
		data.quadIndexCount += 6;
		data.stats.quadCount++;
	}

	// Draw a quad with a texture (3D position)
	void Renderer2D::drawQuad(const glm::mat4& transform, const Shared<Texture2D>& texture, float textureScale, const glm::vec4& tintColor)
	{
		PROFILE_FUNCTION();

		// Flush and reset if index limit reached
		if (data.quadIndexCount >= Renderer2DData::maxIndices)
			flushAndReset();

		// Define texture coordinates
		constexpr glm::vec2 texCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };

		// Set texture index
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < data.textureSlotIndex; i++)
		{
			if (*data.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		// Add texture to next slot
		if (textureIndex == 0.0f)
		{
			if (data.textureSlotIndex >= Renderer2DData::maxTextureSlots)
				flushAndReset();

			textureIndex = (float)data.textureSlotIndex;
			data.textureSlots[data.textureSlotIndex] = texture;
			data.textureSlotIndex++;
		}

		// Set vertex data
		for (int i = 0; i < 4; i++)
		{
			data.quadVertexBufferPtr->Position = transform * data.quadVertexPositions[i];
			data.quadVertexBufferPtr->Color = tintColor;
			data.quadVertexBufferPtr->TexCoord = texCoords[i];
			data.quadVertexBufferPtr->TexIndex = textureIndex;
			data.quadVertexBufferPtr->TilingFactor = textureScale;
			data.quadVertexBufferPtr++;
		}

		// Increment index and quad count
		data.quadIndexCount += 6;
		data.stats.quadCount++;
	}

	// Reset statistics
	void Renderer2D::resetStats()
	{
		memset(&data.stats, 0, sizeof(Renderer2D::Statistics));
	}

	// Get statistics
	Renderer2D::Statistics Renderer2D::getStats()
	{
		return data.stats;
	}

	// Flush and reset renderer data
	void Renderer2D::flushAndReset()
	{
		// End scene
		endScene();

		// Reset renderer data
		data.quadIndexCount = 0;
		data.quadVertexBufferPtr = data.quadVertexBufferBase;
		data.textureSlotIndex = 1;
	}
}