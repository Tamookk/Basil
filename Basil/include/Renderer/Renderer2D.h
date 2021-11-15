/*
 * This header file declares the 2D renderer class.
 */
#pragma once

#include "Renderer/Camera.h"
#include "Renderer/EditorCamera.h"
#include "Renderer/OrthographicCamera.h"
#include "Renderer/Texture2D.h"
#include "Scene/Component.h"

namespace Basil
{
	class Renderer2D
	{
		public:
			struct Statistics
			{
				uint32_t drawCalls = 0;
				uint32_t quadCount = 0;
				uint32_t getTotalVertexCount();
				uint32_t getTotalIndexCount();
			};
			static void init();
			static void shutdown();
			static void beginScene(const Camera& camera, const glm::mat4& transform);
			static void beginScene(const OrthographicCamera& camera);
			static void beginScene(const EditorCamera& camera);
			static void endScene();
			static void startBatch();
			static void flush();
			static void nextBatch();
			
			static void drawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
			static void drawQuad(const glm::mat4& transform, const Shared<Texture2D>& texture, float textureScale = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);
			static void drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);
			static void drawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);
			static void drawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID = -1);
			static float getLineWidth();
			static void setLineWidth(float width);

			static void resetStats();
			static Statistics getStats();
	};
}
