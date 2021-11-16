#include "pch.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Shader.h"
#include "Renderer/UniformBuffer.h"
#include "Renderer/VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

		// Editor only
		int EntityID;
	};

	// For storing the structure of a circle
	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		// Editor only
		int EntityID;
	};

	// For storing the structure of line vertices
	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor only
		int EntityID;
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
		Shared<Shader> quadShader;
		Shared<Texture2D> whiteTexture;

		Shared<VertexArray> circleVertexArray;
		Shared<VertexBuffer> circleVertexBuffer;
		Shared<Shader> circleShader;

		Shared<VertexArray> lineVertexArray;
		Shared<VertexBuffer> lineVertexBuffer;
		Shared<Shader> lineShader;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		uint32_t circleIndexCount = 0;
		CircleVertex* circleVertexBufferBase = nullptr;
		CircleVertex* circleVertexBufferPtr = nullptr;

		uint32_t lineVertexCount = 0;
		LineVertex* lineVertexBufferBase = nullptr;
		LineVertex* lineVertexBufferPtr = nullptr;

		float lineWidth = 2.0f;

		std::array<Shared<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1;

		glm::vec4 quadVertexPositions[4];

		Renderer2D::Statistics stats;

		struct CameraData
		{
			glm::mat4 viewProjection;
		};
		CameraData cameraBuffer;
		Shared<UniformBuffer> cameraUniformBuffer;
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
				{ "a_TilingFactor", ShaderDataType::Float },
				{ "a_EntityID",		ShaderDataType::Int }
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

		// == Circles ==
		// Create a VAO
		data.circleVertexArray = VertexArray::create();

		// Create a vertex buffer
		data.circleVertexBuffer = VertexBuffer::create(data.maxVertices * sizeof(CircleVertex));
		data.circleVertexBuffer->setLayout({
				{ "a_WorldPosition",	ShaderDataType::Float3 },
				{ "a_LocalPosition",	ShaderDataType::Float3 },
				{ "a_Color",			ShaderDataType::Float4 },
				{ "a_Thickness",		ShaderDataType::Float },
				{ "a_Fade",				ShaderDataType::Float },
				{ "a_EntityID",			ShaderDataType::Int }
			});
		data.circleVertexArray->addVertexBuffer(data.circleVertexBuffer);
		data.circleVertexArray->setIndexBuffer(squareIbo); // Use quad IBO
		data.circleVertexBufferBase = new CircleVertex[data.maxVertices];

		// == Lines ==
		// Create a VAO
		data.lineVertexArray = VertexArray::create();

		// Create a vertex buffer
		data.lineVertexBuffer = VertexBuffer::create(data.maxVertices * sizeof(LineVertex));
		data.lineVertexBuffer->setLayout({
				{ "a_Position",		ShaderDataType::Float3 },
				{ "a_Color",		ShaderDataType::Float4 },
				{ "a_EntityID",		ShaderDataType::Int }
			});
		data.lineVertexArray->addVertexBuffer(data.lineVertexBuffer);
		data.lineVertexBufferBase = new LineVertex[data.maxVertices];

		// Create white texture
		data.whiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[data.maxTextureSlots];
		for (uint32_t i = 0; i < data.maxTextureSlots; i++)
			samplers[i] = i;

		// Initialise shader
		data.quadShader = Shader::create("assets/shaders/2D_Quad.glsl");
		data.circleShader = Shader::create("assets/shaders/2D_Circle.glsl");
		data.lineShader = Shader::create("assets/shaders/2D_Line.glsl");

		// Set texture slot 0 to the white texture
		data.textureSlots[0] = data.whiteTexture;

		data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[2] = { -0.5f,  0.5f, 0.0f, 1.0f };
		data.quadVertexPositions[3] = {  0.5f,  0.5f, 0.0f, 1.0f };

		data.cameraUniformBuffer = UniformBuffer::create(sizeof(Renderer2DData::CameraData), 0);
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
		data.cameraBuffer.viewProjection = camera.getProjection() * glm::inverse(transform);
		data.cameraUniformBuffer->setData(&data.cameraBuffer, sizeof(Renderer2DData::CameraData));

		startBatch();
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		PROFILE_FUNCTION();

		data.cameraBuffer.viewProjection = camera.getViewProjectionMatrix();
		data.cameraUniformBuffer->setData(&data.cameraBuffer, sizeof(Renderer2DData::CameraData));

		startBatch();
	}

	void Renderer2D::beginScene(const EditorCamera& camera)
	{
		PROFILE_FUNCTION();

		data.cameraBuffer.viewProjection = camera.getViewProjection();
		data.cameraUniformBuffer->setData(&data.cameraBuffer, sizeof(Renderer2DData::CameraData));

		startBatch();
	}

	// End a scene
	void Renderer2D::endScene()
	{
		PROFILE_FUNCTION();
		
		flush();
	}

	// Start a batch
	void Renderer2D::startBatch()
	{
		data.quadIndexCount = 0;
		data.quadVertexBufferPtr = data.quadVertexBufferBase;

		data.circleIndexCount = 0;
		data.circleVertexBufferPtr = data.circleVertexBufferBase;

		data.lineVertexCount = 0;
		data.lineVertexBufferPtr = data.lineVertexBufferBase;

		data.textureSlotIndex = 1;
	}

	// Flush
	void Renderer2D::flush()
	{
		// Clear quad data
		if (data.quadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)data.quadVertexBufferPtr - (uint8_t*)data.quadVertexBufferBase);
			data.quadVertexBuffer->setData(data.quadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < data.textureSlotIndex; i++)
				data.textureSlots[i]->bind(i);

			// Draw
			data.quadShader->bind();
			Renderer::drawIndexed(data.quadVertexArray, data.quadIndexCount);

			// Increment draw calls
			data.stats.drawCalls++;
		}

		// Clear circle data
		if (data.circleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)data.circleVertexBufferPtr - (uint8_t*)data.circleVertexBufferBase);
			data.circleVertexBuffer->setData(data.circleVertexBufferBase, dataSize);

			// Bind shader
			data.circleShader->bind();
			Renderer::drawIndexed(data.circleVertexArray, data.circleIndexCount);

			// Increment draw calls
			data.stats.drawCalls++;
		}

		// Clear line data
		if (data.lineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)data.lineVertexBufferPtr - (uint8_t*)data.lineVertexBufferBase);
			data.lineVertexBuffer->setData(data.lineVertexBufferBase, dataSize);

			// Bind shader
			data.lineShader->bind();
			Renderer::setLineWidth(data.lineWidth);
			Renderer::drawLines(data.lineVertexArray, data.lineVertexCount);
			data.stats.drawCalls++;
		}
	}

	void Renderer2D::nextBatch()
	{
		flush();
		startBatch();
	}

	// Draw a quad (3D position)
	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		PROFILE_FUNCTION();
		
		const float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;
		constexpr glm::vec2 texCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };

		// Flush and reset if index limit reached
		if (data.quadIndexCount >= Renderer2DData::maxIndices)
			nextBatch();

		// Set vertex data
		for (int i = 0; i < 4; i++)
		{
			data.quadVertexBufferPtr->Position = transform * data.quadVertexPositions[i];
			data.quadVertexBufferPtr->Color = color;
			data.quadVertexBufferPtr->TexCoord = texCoords[i];
			data.quadVertexBufferPtr->TexIndex = textureIndex;
			data.quadVertexBufferPtr->TilingFactor = tilingFactor;
			data.quadVertexBufferPtr->EntityID = entityID;
			data.quadVertexBufferPtr++;
		}

		// Increment index and quad count
		data.quadIndexCount += 6;
		data.stats.quadCount++;
	}

	// Draw a quad with a texture (3D position)
	void Renderer2D::drawQuad(const glm::mat4& transform, const Shared<Texture2D>& texture, float textureScale, const glm::vec4& tintColor, int entityID)
	{
		PROFILE_FUNCTION();

		// Define texture coordinates
		constexpr glm::vec2 texCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };

		// Flush and reset if index limit reached
		if (data.quadIndexCount >= Renderer2DData::maxIndices)
			nextBatch();

		// Set texture index
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < data.textureSlotIndex; i++)
		{
			if (*data.textureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		// Add texture to next slot
		if (textureIndex == 0.0f)
		{
			if (data.textureSlotIndex >= Renderer2DData::maxTextureSlots)
				nextBatch();

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
			data.quadVertexBufferPtr->EntityID = entityID;
			data.quadVertexBufferPtr++;
		}

		// Increment index and quad count
		data.quadIndexCount += 6;
		data.stats.quadCount++;
	}

	// Draw a sprite
	void Renderer2D::drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		if (src.texture)
			drawQuad(transform, src.texture, src.tilingFactor, src.color, entityID);
		else
			drawQuad(transform, src.color, entityID);
	}

	// Draw a circle
	void Renderer2D::drawCircle(const glm::mat4& transform, const::glm::vec4& color, float thickness, float fade, int entityID)
	{
		PROFILE_FUNCTION();

		// Set vertex data
		for (int i = 0; i < 4; i++)
		{
			data.circleVertexBufferPtr->WorldPosition = transform * data.quadVertexPositions[i];
			data.circleVertexBufferPtr->LocalPosition = data.quadVertexPositions[i] * 2.0f;
			data.circleVertexBufferPtr->Color = color;
			data.circleVertexBufferPtr->Thickness = thickness;
			data.circleVertexBufferPtr->Fade = fade;
			data.circleVertexBufferPtr->EntityID = entityID;
			data.circleVertexBufferPtr++;
		}

		// Increment index and quad count
		data.circleIndexCount += 6;
		data.stats.quadCount++;
	}

	// Draw a line
	void Renderer2D::drawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		data.lineVertexBufferPtr->Position = p0;
		data.lineVertexBufferPtr->Color = color;
		data.lineVertexBufferPtr->EntityID = entityID;
		data.lineVertexBufferPtr++;

		data.lineVertexBufferPtr->Position = p1;
		data.lineVertexBufferPtr->Color = color;
		data.lineVertexBufferPtr->EntityID = entityID;
		data.lineVertexBufferPtr++;

		data.lineVertexCount += 2;
	}

	// Draw a rectangle given position and size
	void Renderer2D::drawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		// Generate points of the rectangle
		glm::vec3 p0 = glm::vec3{ position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z };
		glm::vec3 p1 = glm::vec3{ position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z };
		glm::vec3 p2 = glm::vec3{ position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z };
		glm::vec3 p3 = glm::vec3{ position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z };

		// Draw the rectangle
		drawLine(p0, p1, color, entityID);
		drawLine(p1, p2, color, entityID);
		drawLine(p2, p3, color, entityID);
		drawLine(p3, p0, color, entityID);
	}

	// Draw a rectangle given a transform
	void Renderer2D::drawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		// Note: these are out of order due to the way in which quad vertex positions are generated.
		// This works properly, but quads should be reworked in the future.
		glm::vec3 lineVertices[4];
		lineVertices[0] = transform * data.quadVertexPositions[0];
		lineVertices[1] = transform * data.quadVertexPositions[1];
		lineVertices[2] = transform * data.quadVertexPositions[3];
		lineVertices[3] = transform * data.quadVertexPositions[2];
		
		// Draw the rectangle
		for (int i = 0; i < 4; i++)
			drawLine(lineVertices[i], lineVertices[(i + 1) % 4], color, entityID);
	}

	// Get line width
	float Renderer2D::getLineWidth()
	{
		return data.lineWidth;
	}

	// Set line width
	void Renderer2D::setLineWidth(float width)
	{
		data.lineWidth = width;
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
}